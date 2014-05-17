/*
 * LIS3MDL.h
 *
 * Created on: April 20, 2014
 *      Author: Chris Ostrum
 */

#ifndef L3GD20_H_
#define L3GD20_H_

#include "spi.h"

#define SPI_READ        (0x80)
#define SPI_WRITE       (0x00)
#define SPI_INC_ADDR    (0x40)

#define WHO_AM_I        (0x0F)
#define DEVICE_ID       (0xD4)
#define CTRL_REG1       (0x20)
#define CTRL_REG2       (0x21)
#define CTRL_REG3       (0x22)
#define CTRL_REG4       (0x23)
#define CTRL_REG5       (0x24)
#define STATUS_REG      (0x27)
#define OUT_X_L         (0x28)
#define OUT_Y_L         (0x2A)
#define OUT_Z_L         (0x2C)

enum POWER_DOWN_MODE
{
    POWER_DOWN,
    SLEEP,
    NORMAL
};

enum AXIS
{
    Y_AXIS,
    X_AXIS,
    Z_AXIS
};

class L3GD20
{
public:
    L3GD20(SPI* spi);
    ~L3GD20();

    bool Initialize();
    bool CheckID();
    bool SetPowerDownMode(POWER_DOWN_MODE mode);
    bool SetAxisEnabled(AXIS axis, bool enabled);

    bool Read(const int axis, double *val);
    bool Read(double *xval, double *yval, double *zval);

private:
    SPI* m_spi;
    uint8_t m_wbuf[10];
    uint8_t m_rbuf[10];

    uint8_t m_fs;
    uint8_t m_fullScaleValues[4] = {250, 500, 2000, 2000};

    double m_sens;
    double m_sensValues[4] = {8.75, 17.5, 70.0, 70.0};

    int GetID();
    int ReadFullScale();
};

#endif
