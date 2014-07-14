/*
 * bluetooth.h
 *
 * Created on: June 08, 2014
 *      Author: Chris Ostrum
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

enum LED
{
    LED2 = 4,
    LED3 = 1
};

class Bluetooth
{
public:
    Bluetooth();
    ~Bluetooth();

    bool Initialize();
    void DisableLED(LED led);
    void EnableLED(LED led);

private:
};

#endif
