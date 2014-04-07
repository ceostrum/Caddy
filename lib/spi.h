/*
 * SPI.h
 *
 *  Created on: Jun 2, 2013
 *      Author: jacek
 */

#include <stdint.h>
#include <linux/spi/spidev.h>

#ifndef SPI_H_
#define SPI_H_

class SPI
{
public:

    SPI();
    virtual ~SPI();

    int open(int bus, int channel);
    int close();
    int read(uint8_t read[], int len);
    int write(uint8_t write[], int len);
    int xfer1(uint8_t write[], uint8_t read[], int len);

    int setMode(uint8_t mode);
    int setClockPolarity(uint8_t pol);
    int setClockPhase(uint8_t phase);
    int setLSBFirst(bool lsb_first);
    int setBitsPerWord(int bits);
    int setSpeed(uint32_t speed);

private:
    uint8_t mode;
    uint8_t bpw;
    bool lsb_first;
    uint32_t speed;
    bool active;
    int fd;
};

#endif
