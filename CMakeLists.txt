cmake_minimum_required (VERSION 2.6)

project (test)
include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory (lib)

add_executable(hello_world hello_world.cpp)

add_executable(mag_test mag_test.cpp)
target_link_libraries(mag_test SPI LIS3MDL)

add_executable(gyro_test gyro_test.cpp)
target_link_libraries(gyro_test SPI L3GD20)

add_executable(spst_test spst_test.cpp)
target_link_libraries(spst_test SPI)

add_executable(bt_test bluetooth_test.cpp)
target_link_libraries(bt_test BT)
