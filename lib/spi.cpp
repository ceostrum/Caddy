/*
 * SPI.cpp
 *
 *  Created on: Jun 2, 2013
 *      Author: jacek
 */

#include "spi.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH_LEN  40
#define SPI_DEVICE_PATH_BASE "/dev/spidev"

SPI::SPI()
{
    active = false;
    mode = 0;
    bpw = 0;
    speed = 0;
    delay = 0;
    fd = -1;
    lsb_first = false;
}

int SPI::open(int bus, int channel)
{
    if (active) close();
    if (bus < 0 || channel < 0) return -ENODEV;

    char path[MAX_PATH_LEN];
    if (snprintf(path, MAX_PATH_LEN, "%s%d.%d", SPI_DEVICE_PATH_BASE, bus,
                 channel) >= MAX_PATH_LEN) return -EINVAL;

    if ((fd = ::open(path, O_RDWR, 0)) < 0)
    {
        printf("open(%s) failed\n", path);
        return fd;
    }

    uint8_t tmp;
    uint32_t tmp32;
    int r;
    if ((r = ioctl(fd, SPI_IOC_RD_MODE, &tmp)) < 0)
    {
        printf("ioctl(fd, SPI_IOC_RD_MODE, &tmp) failed\n");
        return r;
    }
    mode = tmp;

    if ((r = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp)) < 0)
    {
        printf("ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp) failed\n");
        return r;
    }
    bpw = tmp;

    if ((r = ioctl(fd, SPI_IOC_RD_LSB_FIRST, &tmp)) < 0)
    {
        printf("ioctl(fd, SPI_IOC_WR_LSB_FIRST, &tmp) failed\n");
        return r;
    }
    this->lsb_first = lsb_first;

    if ((r = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp32)) < 0)
    {
        printf("ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp) failed\n");
        return r;
    }
    speed = tmp32;

    active = true;
    return 1;
}

int SPI::close()
{
    if (!active) return -ENODEV;
    printf("SPI::close()\n");
    mode = 0;
    bpw = 0;
    speed = 0;
    active = false;
    int tmpfd = fd;
    fd = -1;
    return ::close(tmpfd);
}

int SPI::setMode(uint8_t mode)
{
    mode &= SPI_CPHA | SPI_CPOL;
    mode = (this->mode & ~(SPI_CPHA | SPI_CPOL)) | mode;

    int r = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (r < 0) return r;

    r = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (r < 0) return r;

    this->mode = mode;

    return 1;
}

int SPI::setClockPolarity(uint8_t pol)
{
    pol &= SPI_CPOL;
    uint8_t mode = (this->mode & ~(SPI_CPOL)) | pol;
    return setMode(mode);
}

int SPI::setClockPhase(uint8_t phase)
{
    phase &= SPI_CPHA;
    uint8_t mode = (this->mode & ~(SPI_CPHA)) | phase;
    return setMode(mode);
}

int SPI::setLSBFirst(bool lsb_first)
{
    int r;
    if (!active) return -ENODEV;
    if ((r = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb_first)) < 0) return r;
    this->lsb_first = lsb_first;
    return 1;
}

int SPI::setBitsPerWord(int bits)
{
    int r;
    if (!active) return -ENODEV;
    if ((r = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) < 0) return r;
    bpw = bits;
    return 1;
}

int SPI::setSpeed(uint32_t speed)
{
    int r;
    if (!active) return -ENODEV;
    r = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (r < 0)
    {
        printf("ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed): %s", strerror(r));
        return r;
    }

    uint32_t tmp;
    r = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp);
    if (r < 0)
    {
        printf("ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed): %s", strerror(r));
        return r;
    }
    this->speed = tmp;
    return 1;
}

int SPI::setDelay(uint32_t delay)
{
    this->delay = delay;
}

int SPI::write(uint8_t wbuf[], int len)
{
    return ::write(fd, wbuf, len);
}

int SPI::read(uint8_t rbuf[], int len)
{
    memset(rbuf, 0, len);
    return ::read(fd, rbuf, len);
}

int SPI::xfer(uint8_t wbuf[], int wlen, uint8_t rbuf[], int rlen)
{
    struct spi_ioc_transfer xfer[2] = {0};

    xfer[0].tx_buf = (__u64 ) wbuf;
    xfer[0].len = wlen;

    xfer[1].rx_buf = (__u64 ) rbuf;
    xfer[1].len = rlen;

    int r = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
    if (r < 0)
    {
        printf("ioctl(fd, SPI_IOC_MESSAGE(N), xfer): %s (len=%d)\n",
                strerror(r), wlen);
        return r;
    }

    return r;
}

SPI::~SPI()
{
    printf("SPI::~SPI()\n");
    close();
}
