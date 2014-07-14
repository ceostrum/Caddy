cmake_minimum_required (VERSION 2.6)

add_library(SPI spi.cpp)
add_library(LIS3MDL lis3mdl.cpp)
add_library(L3GD20 l3gd20.cpp)
add_library(BT bluetooth.cpp)

install (TARGETS SPI DESTINATION bin)
install (FILES spi.h DESTINATION include)

install (TARGETS LIS3MDL DESTINATION bin)
install (FILES lis3mdl.h DESTINATION include)

install (TARGETS L3GD20 DESTINATION bin)
install (FILES l3gd20.h DESTINATION include)

install (TARGETS BT DESTINATION bin)
install (FILES bluetooth.h DESTINATION include)
