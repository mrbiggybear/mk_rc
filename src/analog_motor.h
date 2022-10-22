
// test for analog contfol for DC motors
// DRV8833 Module

#include "mkart.h"

analog_poll()
{
    // will poll for input value change and attempts to adjust motor speed using analog output.

    while (true)
    {
        // analog input to analog output for motors.
        if (float pot_reading = analogRead())
        {
        }
    }
}
