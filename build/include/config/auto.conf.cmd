deps_config := \
	/home/moofarry/esp/esp-mdf/esp-idf/components/app_trace/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/aws_iot/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/bt/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/driver/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/esp32/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/esp_adc_cal/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/esp_event/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/esp_http_client/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/esp_http_server/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/ethernet/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/fatfs/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/freemodbus/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/freertos/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/heap/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/libsodium/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/log/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/lwip/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/mbedtls/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/mdns/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/mqtt/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/nvs_flash/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/openssl/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/pthread/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/spi_flash/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/spiffs/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/tcpip_adapter/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/vfs/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/wear_levelling/Kconfig \
	/home/moofarry/esp/esp-mdf/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/moofarry/esp/esp-skainet/components/esp-sr/Kconfig.projbuild \
	/home/moofarry/esp/esp-mdf/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/moofarry/esp/esp-skainet/components/hardware_driver/Kconfig.projbuild \
	/home/moofarry/esp/esp-mdf/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/moofarry/esp/esp-mdf/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
