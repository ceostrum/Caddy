#include "spi.h"
#include <stdio.h>

int main()
{
    printf("SPST Switches Test\n");

    SPI spi;
    spi.open(1,0);
    spi.setMode(SPI_MODE_0);
    spi.setSpeed(1000000);
    spi.setLSBFirst(false);

    for(uint8_t i = 0; i < 8; i++)
    {
        printf("%0b: ", i);
        spi.xfer(&i, 1, 0, 0);
    }

    spi.close();
    return 0;
}
