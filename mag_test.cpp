#include "spi.h"
#include "lis3mdl.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Magnetometer Test\n");

    SPI spi;
    spi.open(1,0);
    spi.setMode(SPI_MODE_0);
    spi.setSpeed(1000000);
    spi.setLSBFirst(false);

    LIS3MDL lis3mdl(&spi);
    if(!lis3mdl.Initialize())
    {
        printf("Failed to initialize.\n");
        return -1;
    }

    if(!lis3mdl.CheckID())
    {
        printf("Failed ID check.\n");
        return -1;
    }

    double x, y, z, heading;
    while(1)
    {
        lis3mdl.Read(&x, &y, &z);
        printf("X: %f Y: %f Z: %f\n\n", x, y, z);

        //lis3mdl.ReadHeading(&heading);
        //printf("Heading: %f\n", heading);
        sleep(1);
    }

    spi.close();
    return 0;
}
