cmake_minimum_required (VERSION 2.6)
project (spi_test)
include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory (lib)

add_executable(spi_test spi_test.cpp)
target_link_libraries(spi_test SPI)
