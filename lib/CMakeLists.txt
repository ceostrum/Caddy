cmake_minimum_required (VERSION 2.6)

add_library(SPI spi.cpp)
add_library(LIS3MDL lis3mdl.cpp)

install (TARGETS SPI DESTINATION bin)
install (FILES spi.h DESTINATION include)

install (TARGETS LIS3MDL DESTINATION bin)
install (FILES lis3mdl.h DESTINATION include)
