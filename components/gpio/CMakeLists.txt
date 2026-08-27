set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
    SRCS "src/gpio.cpp"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES "esp_driver_gpio"
)