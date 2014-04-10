#include "spi.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    int version = 7;
    printf("Version %d\n", version);
    SPI spi;
    uint8_t writeCommand = 0xE8; // 0b11101000;
    uint8_t read[10];
    int len = 4;

    spi.open(1,0);
    spi.setMode(SPI_MODE_0);
    spi.setSpeed(500000);
    spi.setDelay(10000);

    spi.write(&writeCommand, 1);
    usleep(20000);
    spi.read(read, 2);
    //spi.xfer1(&writeCommand, read, len);
    printf("Read: ");
    for(int i = 0; i < len; i++)
      printf("%x", read[i]);
    return 0;
}
