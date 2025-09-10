cmake_minimum_required(VERSION 3.15)
project(cclarify_test C)

find_package(cclarify CONFIG REQUIRED)

add_executable(cclarify-tests
    ${CMAKE_CURRENT_SOURCE_DIR}/../dist/munit/munit.c
    ${PROJECT_SOURCE_DIR}/src/tests.c)
target_include_directories(cclarify-tests PUBLIC ${cclarify_SOURCE_DIR}/dist/munit)
target_link_libraries(cclarify-tests cclarify::cclarify)
