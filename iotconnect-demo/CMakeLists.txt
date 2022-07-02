cmake_minimum_required(VERSION 3.0.2)

project(iotconnect-demo)

include_directories(config PRIVATE)
add_library(iotconnect-demo iotconnect_app.c)
link_libraries(iotconnect-demo)
