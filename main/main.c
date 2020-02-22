/* 
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include "driver/i2s.h"

#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "esp_agc.h"
#include "esp_ns.h"

#define BITS_PS        16
#define I2S_NUM        0
#define BUF_COUNT      4//4
#define BUF_LENGTH     480 //1024
#define AGC_FRAME_BYTES     320

// WakeNet
static const esp_wn_iface_t *wakenet = &WAKENET_MODEL;
static const model_coeff_getter_t *model_coeff_getter = &WAKENET_COEFF;

void wakenetTask(void *arg)
{
    model_iface_data_t *model_data = arg;
    int frequency = wakenet->get_samp_rate(model_data);
    int audio_chunksize = (wakenet->get_samp_chunksize(model_data))*2;
    int16_t *buffer = malloc(audio_chunksize * sizeof(int16_t));
    assert(buffer);
    unsigned int chunks = 0;
    int mn_chunks = 0;
    bool detect_flag = 0;

    printf("%d",audio_chunksize);
 i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,    //master, RX, PDM
        .sample_rate = frequency,
        .bits_per_sample = BITS_PS,                                    
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S, //pcm data format
        .dma_buf_count = BUF_COUNT,                   
        .dma_buf_len = BUF_LENGTH,                  
        .use_apll = 0,                               //apll disabled
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1     //interrupt level 1(lowest priority)
    };

    const i2s_pin_config_t pin_config = {
    .bck_io_num = GPIO_NUM_14,
    .ws_io_num = GPIO_NUM_15,
    .data_out_num = -1,
    .data_in_num = GPIO_NUM_32
  };

    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);
    printf("Start speaking\n");

    void *agc_handle = esp_agc_open(3, frequency);
    set_agc_config(agc_handle, 35, 1, 0);
    int16_t *agc_in  = malloc(AGC_FRAME_BYTES);
    int16_t *agc_out = malloc(AGC_FRAME_BYTES);
    //int16_t *aux = malloc(AGC_FRAME_BYTES);
    ns_handle_t ns_inst = ns_create(20); //20ms

    while (1) {
      i2s_read_bytes(I2S_NUM, (char *)buffer, audio_chunksize * sizeof(int16_t), portMAX_DELAY);


      for (size_t i = 0; i < 3; i++)
      {
        memcpy(agc_in, buffer + i*AGC_FRAME_BYTES, AGC_FRAME_BYTES);
        int siza = esp_agc_process(agc_handle, agc_in, agc_out, AGC_FRAME_BYTES/2, frequency);
        //ns_process(ns_inst, aux, agc_out);
        //printf("El Siza: %d\n",siza);
        memcpy(buffer + i*AGC_FRAME_BYTES, agc_out, AGC_FRAME_BYTES);
      }

     // rb_read(rec_rb, (uint8_t *)buffer, audio_chunksize * sizeof(int16_t), portMAX_DELAY);
        int r[2];

        for (size_t i = 0; i < 2; i++)
        { //Este for se utiliza para introducir muestras de 30ms en el wakenet.
          r[i] = wakenet->detect(model_data, buffer + i*audio_chunksize/2);
        }
        
        
        if (r[0]||r[1]) {
            float ms = (chunks * audio_chunksize * 1000.0) / frequency;
            printf("%.2f: %s DETECTED.\n", (float)ms / 1000.0, wakenet->get_word_name(model_data, 1));
            detect_flag = 1;
            
        } 
        chunks++;
    }
    vTaskDelete(NULL);
}

void app_main()
{

    model_iface_data_t *model_data = wakenet->create(model_coeff_getter, DET_MODE_90);
    // set wake word detection threshold
    wakenet->set_det_threshold(model_data, 0.8, 1);
    

    xTaskCreate(&wakenetTask, "wakenet", 2 * 1024, (void*)model_data, 5, NULL);
}