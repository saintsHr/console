set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
    SRCS
        "src/st7735.cpp"
    INCLUDE_DIRS
        "include"
)