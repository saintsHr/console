cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(EXTRA_COMPONENT_DIRS "components/drivers")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(console)