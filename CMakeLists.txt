cmake_minimum_required (VERSION 2.6)

project (mag_test)
include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory (lib)

add_executable(mag_test mag_test.cpp)
target_link_libraries(mag_test SPI LIS3MDL)
