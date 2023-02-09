cmake_minimum_required(VERSION 3.16.0)
project(amt21_driver)

set(CMAKE_CXX_STANDARD 17)



add_library(amt21_driver include/amt21_driver.h src/amt21_driver.cc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)



add_executable(program tests/main.cc)
add_executable(error_codes tests/error_code_validation.cpp)

target_link_libraries(program amt21_driver)