set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
    SRCS
        "main.cpp"
    REQUIRES
        gpio
        pwm
        spi
        st7735
)