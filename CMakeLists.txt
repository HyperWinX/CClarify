cmake_minimum_required(VERSION 3.15)
project(hyper-cclarify C)

message(STATUS "Using C standard: ${CMAKE_C_STANDARD}")

add_library(cclarify src/cclarify.c)
target_include_directories(cclarify PUBLIC include)

set_target_properties(cclarify PROPERTIES PUBLIC_HEADER "include/cclarify/cclarify.h")
install(TARGETS cclarify)
