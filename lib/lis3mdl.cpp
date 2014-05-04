#include "lis3mdl.h"
#include <math.h>

LIS3MDL::LIS3MDL(SPI* spi)
{
    m_spi = spi;
}

LIS3MDL::~LIS3MDL()
{
    m_spi = 0;
}

bool LIS3MDL::Initialize()
{
    SetContinousConversionMode();
    ReadMagneticCharacteristics();
}

bool LIS3MDL::SetContinousConversionMode()
{
    m_wbuf[0] = SPI_READ | CTRL_REG3;
    if(m_spi->xfer(m_wbuf, 1, m_rbuf, 1) < 0) return false;

    m_wbuf[0] = SPI_WRITE | CTRL_REG3;
    m_wbuf[1] = (m_rbuf[0] & 0xFC);
    return (m_spi->xfer(m_wbuf, 2, 0, 0) > 0);
}

bool LIS3MDL::EnableTemperatureSensor()
{
    m_wbuf[0] = SPI_READ | CTRL_REG1;
    if(m_spi->xfer(m_wbuf, 1, m_rbuf, 1) < 0) return false;

    m_wbuf[0] = SPI_WRITE | CTRL_REG1;
    m_wbuf[1] = m_rbuf[0] | TEMP_EN;
    return (m_spi->xfer(m_wbuf, 2, 0, 0) > 0);
}

bool LIS3MDL::ReadMagneticCharacteristics()
{
    m_wbuf[0] = SPI_READ | CTRL_REG2;
    if(m_spi->xfer(m_wbuf, 1, m_rbuf, 1) < 0) return false;

    int index = (0x60 & m_rbuf[0]);
    if( index < 0 && index > 3) return false;

    m_fs = m_fullScaleValues[index];
    m_sens = m_sensValues[index];
    return true;
}

int LIS3MDL::GetID()
{
    if(!m_spi) return -1;
    m_wbuf[0] = SPI_READ | WHO_AM_I;
    int val = m_spi->xfer(m_wbuf, 1, m_rbuf, 1);
    if(val > 0) return m_rbuf[0];
    else return val;
}

bool LIS3MDL::CheckID()
{
    int id = GetID();
    return (id == DEVICE_ID);
}

bool LIS3MDL::Read(const int axis, double *val)
{
    if(!m_spi) return false;
    *val = 0;

    m_wbuf[0] = SPI_READ | SPI_INC_ADDR | axis;
    int ret = m_spi->xfer(m_wbuf, 1, m_rbuf, 2);
    if(ret < 0) return false;

    *val = (~((m_rbuf[1] << 8) | m_rbuf[0]) + 1) / m_sens;
    return true;
}

bool LIS3MDL::Read(double *xval, double *yval, double *zval)
{
    if(!m_spi) return false;
    *xval = *yval = *zval = 0;

    m_wbuf[0] = SPI_READ | SPI_INC_ADDR | OUT_X_L;
    int ret = m_spi->xfer(m_wbuf, 1, m_rbuf, 6);
    if(ret < 0) return false;

    *xval = (~((m_rbuf[1] << 8) | m_rbuf[0]) + 1) / m_sens;
    *yval = (~((m_rbuf[3] << 8) | m_rbuf[2]) + 1) / m_sens;
    *zval = (~((m_rbuf[5] << 8) | m_rbuf[4]) + 1) / m_sens;

    return true;
}

bool LIS3MDL::ReadHeading(double *heading)
{
    double x, y, z;
    if(!Read(&x, &y, &z)) return false;
    *heading = atan2(y, x) * 180.0 / PI;
   return true;
}
