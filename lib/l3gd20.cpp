#include "l3gd20.h"
#include "bit.h"
#include <math.h>

L3GD20::L3GD20(SPI* spi)
{
    m_spi = spi;
}

L3GD20::~L3GD20()
{
    m_spi = 0;
}

bool L3GD20::Initialize()
{
    SetPowerDownMode(NORMAL);
    SetAxisEnabled(X_AXIS, true);
    SetAxisEnabled(Y_AXIS, true);
    SetAxisEnabled(Z_AXIS, true);
    ReadFullScale();
}

bool L3GD20::SetPowerDownMode(POWER_DOWN_MODE mode)
{
    if(!m_spi) return false;

    m_wbuf[0] = SPI_READ | CTRL_REG1;
    if(m_spi->xfer(m_wbuf, 1, m_rbuf, 1) < 0) return false;

    m_wbuf[0] = SPI_WRITE | CTRL_REG1;
    m_wbuf[1] = m_rbuf[0];
    if(mode) SETBIT(m_wbuf[1], 3);
    else CLEARBIT(m_wbuf[1], 3);

    return (m_spi->xfer(m_wbuf, 2, 0, 0) > 0);
}

bool L3GD20::SetAxisEnabled(AXIS axis, bool enabled)
{
    if(!m_spi) return false;

    m_wbuf[0] = SPI_READ | CTRL_REG1;
    if(m_spi->xfer(m_wbuf, 1, m_rbuf, 1) < 0) return false;

    m_wbuf[0] = SPI_WRITE | CTRL_REG1;
    m_wbuf[1] = m_rbuf[0];
    if(enabled) SETBIT(m_wbuf[1], axis);
    else CLEARBIT(m_wbuf[1], axis);

    return (m_spi->xfer(m_wbuf, 2, 0, 0) > 0);
}

int L3GD20::ReadFullScale()
{
    if(!m_spi) return -1;

    m_wbuf[0] = SPI_READ | CTRL_REG4;
    if(m_spi->xfer(m_wbuf, 1, m_rbuf, 1) < 0) return false;

    int index = (m_rbuf[0] >> 4) & 0x3;
    if( index < 0 || index > 3) return false;

    m_fs = m_fullScaleValues[index];
    m_sens = m_sensValues[index];
    return true;
}

int L3GD20::GetID()
{
    if(!m_spi) return -1;
    m_wbuf[0] = SPI_READ | WHO_AM_I;
    int val = m_spi->xfer(m_wbuf, 1, m_rbuf, 1);
    if(val > 0) return m_rbuf[0];
    else return val;
}

bool L3GD20::CheckID()
{
    int id = GetID();
    return (id == DEVICE_ID);
}

bool L3GD20::Read(const int axis, double *val)
{
    if(!m_spi) return false;
    *val = 0;

    m_wbuf[0] = SPI_READ | SPI_INC_ADDR | axis;
    int ret = m_spi->xfer(m_wbuf, 1, m_rbuf, 2);
    if(ret < 0) return false;

    *val = (~((m_rbuf[1] << 8) | m_rbuf[0]) + 1) / m_sens;
    return true;
}

bool L3GD20::Read(double *xval, double *yval, double *zval)
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
