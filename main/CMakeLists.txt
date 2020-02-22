set(COMPONENT_SRCS "main.c")

set(COMPONENT_ADD_INCLUDEDIRS include)

set(COMPONENT_REQUIRES
    esp-sr
    hardware_driver
    )

register_component()



