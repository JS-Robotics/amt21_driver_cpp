cmake_minimum_required(VERSION 3.16.0)
project(amt21_driver VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(amt21_driver SHARED src/amt21_driver.cc)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(program tests/main.cc)
add_executable(error_codes tests/error_code_validation.cpp)

target_link_libraries(program amt21_driver)

set_target_properties(amt21_driver PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(amt21_driver PROPERTIES PUBLIC_HEADER include/amt21_driver.h)
target_include_directories(amt21_driver PRIVATE include)
target_include_directories(amt21_driver PRIVATE src)


include(GNUInstallDirs)
set(CMAKE_INSTALL_PREFIX "/usr") # TODO The library is currently set to install in /usr instead of /usr/local such that ros2 can find the package.

install(TARGETS amt21_driver
        EXPORT amt21_driverConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


configure_file(tools/amt21_driver.pc.in amt21_driver.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/amt21_driver.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

install(EXPORT amt21_driverConfig DESTINATION share/amt21_driver/cmake)

export(TARGETS ${PROJECT_NAME} FILE amt21_driverConfig.cmake)