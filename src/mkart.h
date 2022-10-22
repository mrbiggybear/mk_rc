#pragma once
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#if not defined(_VARIANT_ARDUINO_DUE_X_) && not defined(_VARIANT_ARDUINO_ZERO_) && not defined(__SAMD51__)
#include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#define PACKET_ACC_LEN (15)
#define PACKET_GYRO_LEN (15)
#define PACKET_MAG_LEN (15)
#define PACKET_QUAT_LEN (19)
#define PACKET_BUTTON_LEN (5)
#define PACKET_COLOR_LEN (6)
#define PACKET_LOCATION_LEN (15)
// ToDo: test motor operatitrue with class
#define UP_1 "516"
#define UP_0 "507"
#define DWN_1 "615"
#define DWN_0 "606"
#define LFT_1 "714"
#define LFT_0 "705"
#define RHT_1 "813"
#define RHT_0 "804"
#define A1_1 "11:"
#define A1_0 "10;"
#define B1_1 "219"
#define B1_0 "20:"
#define A2_1 "318"
#define A2_0 "309"
#define B2_1 "417"
#define B2_0 "408"

const bool NO = true;

//    READ_BUFSIZE            Size of the read buffer for incoming packets
#define READ_BUFSIZE (20)

/* Buffer to hold incoming characters */
uint8_t packetbuffer[READ_BUFSIZE + 1];

#include "BluefruitConfig.h"

const int max_motor_conn = 4;
String command;

enum wheel_mode
{
  DRIVE,
  HOVER
};

const uint8_t pins[6] = {
    21, // L forward
    20, // L backward
    18, // R forward
    19, // R backward

    9, // drive mode
    8  // hover mode
};

class mkart
{
  uint32_t mode = 0;
  uint32_t ult = 0; // should check on occasions

public:
  mkart()
  {
    ble_setup();

    setup_pins();
  }

  void setup_pins()
  {
    for (int pin_idx = 0u; pins < max_motor_conn; pin_idx++)
    {
      char msg[128];
      sprintf(msg, "pin : %d\n", pin_idx);
      Serial.println(msg);

      pinMode(pin_idx, OUTPUT);
      if (digitalRead(pin_idx))
      {
        digitalWrite(pin_idx, !ON);
      };
    }
  }

  void go(bool forward)
  {
    if (forward)
    {
      digitalWrite(pins[0], ON);
      digitalWrite(pins[2], ON);
      return;
    }
    digitalWrite(pins[1], ON);
    digitalWrite(pins[3], ON);
  }

  void stop_all()
  {
    Serial.println("Stopping...");
    for (int pin_idx = 0u; pin_idx < max_motor_conn; pin_idx++)
    {
      digitalWrite(pins[pin_idx], !ON);
    }
  }

  void forward() { go(ON); }

  void backward() { go(!ON); }

  void right()
  {
    digitalWrite(pins[0], ON);
    digitalWrite(pins[3], ON);
  }

  void left()
  {
    digitalWrite(pins[1], ON);
    digitalWrite(pins[2], ON);
  }

  void test_motors()
  {
    Serial.print("forward ");
    forward();
    delay(3000);
    stop_all();
    Serial.print("backward ");
    backward();
    delay(3000);
    stop_all();
    Serial.print("left ");
    left();
    delay(3000);
    stop_all();
    Serial.print("right ");
    right();
    delay(3000);
    stop_all();

    // set_mode(DRIVE);
    // set_mode(HOVER);
    stop_all();
  }

  void button_controller(Adafruit_BLE *ble, uint16_t timeout)
  {
    uint8_t stream_index = 0;
    uint16_t origtimeout = timeout, replyidx = 0;

    memset(packetbuffer, 0, READ_BUFSIZE);
    String command = "";
    while (timeout--)
    {
      while (ble->available())
      {
        stream_index++;
        // Serial.println("Reading packet");
        command += char(ble->read());
      }

      if (
          (command.begin() < command.end()) &&
          command.substring(0, 2) == "!B")
      {
        stop_all();

        command = command.substring(2, 5);
        if (timeout == 0)
        {
          break;
        }

        if (command == UP_1)
        {
          Serial.println("forward");
          forward();
        }

        if (command == UP_0)
        {
          break;
        }

        if (command == DWN_1)
        {
          Serial.println("backward");
          backward();
        }

        if (command == DWN_0)
        {
          break;
        }

        if (command == LFT_1)
        {
          Serial.println("left");
          left();
        }

        if (command == LFT_0)
        {
          break;
        }

        if (command == RHT_1)
        {
          Serial.println("right");
          right();
        }

        if (command == RHT_0)
        {
          break;
        }

        if (command == A1_1)
        {
          Serial.println("forward");
          forward();
        }

        if (command == A1_0)
        {
          break;
        }

        if (command == B1_1)
        {
          Serial.println("backward");
          backward();
        }

        if (command == B1_0)
        {
          break;
        }

        if (command == A2_1)
        {
          Serial.println("forward");
          forward();
        }

        if (command == A2_0)
        {
          break;
        }

        if (command == B2_1)
        {
          Serial.println("backward");
          backward();
        }

        if (command == B2_0)
        {
          break;
        }
        Serial.println();
      }

      if (replyidx >= 20)
        break;

      replyidx++;
      timeout = origtimeout;

      delay(10);
    }
  }
};
