set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
    SRCS
        "src/renderer.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        st7735
)