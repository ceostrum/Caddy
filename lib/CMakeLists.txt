add_library(SPI spi.cpp)

install (TARGETS SPI DESTINATION bin)
install (FILES spi.h DESTINATION include)
