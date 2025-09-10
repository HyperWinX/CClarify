cmake_minimum_required(VERSION 3.15)
project(cclarify_tests CXX)

find_package(cclarify CONFIG REQUIRED)

get_target_property(CCLARIFY_SRC_DIR cclarify::cclarify IMPORTED_LOCATION)

add_executable(cclarify-tests
  src/tests.c
  ${CCLARIFY_SRC_DIR}/dist/munit/munit.c)
target_include_directories(cclarify-tests ${CCLARIFY_SRC_DIR}/dist/munit)
target_link_libraries(cclarify-tests hello::hello)
