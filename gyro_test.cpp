#include "spi.h"
#include "l3gd20.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Gyro Test\n");

    SPI spi;
    spi.open(1,0);
    spi.setMode(SPI_MODE_0);
    spi.setSpeed(1000000);
    spi.setLSBFirst(false);

    L3GD20 l3gd20(&spi);
    if(!l3gd20.Initialize())
    {
        printf("Failed to initialize.\n");
        return -1;
    }

    if(!l3gd20.CheckID())
    {
        printf("Failed ID check.\n");
        return -1;
    }

    double x, y, z;
    while(1)
    {
        l3gd20.Read(&x, &y, &z);
        printf("X: %f Y: %f Z: %f\n\n", x, y, z);

        sleep(1);
    }

    spi.close();
    return 0;
}
