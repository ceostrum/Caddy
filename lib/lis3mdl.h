/*
 * LIS3MDL.h
 *
 * Created on: April 20, 2014
 *      Author: Chris Ostrum
 */

#ifndef LIS3MDL_H_
#define LIS3MDL_H_

#include "spi.h"

#define SPI_READ        (0x80)
#define SPI_WRITE       (0x00)
#define SPI_INC_ADDR    (0x40)

#define WHO_AM_I        (0x0F)
#define DEVICE_ID       (0x3D)
#define CTRL_REG1       (0x20)
#define CTRL_REG2       (0x21)
#define CTRL_REG3       (0x22)
#define CTRL_REG4       (0x23)
#define CTRL_REG5       (0x24)
#define STATUS_REG      (0x27)
#define OUT_X_L         (0x28)
#define OUT_Y_L         (0x2A)
#define OUT_Z_L         (0x2C)
#define TEMP_OUT_L      (0x2E)

#define TEMP_EN         (0x80)

#define PI              (3.14159265389)


class LIS3MDL
{
public:
    LIS3MDL(SPI * spi);
    ~LIS3MDL();

    bool Initialize();
    bool CheckID();
    bool EnableTemperatureSensor();

    bool Read(const int axis, double *val);
    bool Read(double *xval, double *yval, double *zval);
    bool ReadHeading(double *heading);

private:
    SPI *m_spi;
    uint8_t m_wbuf[10];
    uint8_t m_rbuf[10];

    uint8_t m_fs;
    uint8_t m_fullScaleValues[4] = {4, 8, 12, 16};

    double m_sens;
    double m_sensValues[4] = {6842.0, 3421.0, 2281.0, 1711.0};

    int GetID();
    bool SetContinousConversionMode();
    bool ReadMagneticCharacteristics();
};
#endif
