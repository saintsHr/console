set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
    SRCS
        "src/input.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        gpio
)