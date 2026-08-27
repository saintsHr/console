set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "../components/gpio/include"
)