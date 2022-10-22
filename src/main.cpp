#include <Arduino.h>
#include <string.h>

#include "mkart.h"

mkart *kart = {};

void setup()
{
  // open serial
  Serial.begin(115200);
  Serial1.begin(115200);

  // dealy start
  delay(5000);

  Serial.println("Creating kart...");
  kart = new mkart();
  Serial.println("Setup Done.");

  delay(2000);

  Serial.println("Testing Motors...");
  // kart->test_motors();
}

void loop()
{
  // Handle new data as it arrives
  kart->button_controller(&ble, BLE_READPACKET_TIMEOUT);
  // The stopping of the motors on release of button
  // only works if release command is issued by app.
  // this is not always received.
  delay(80);
}
