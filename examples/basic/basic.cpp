#include <Arduino.h>
#include <M5Unified.h>

#include "LazyBLE.h"

// Declare for later use
BLECharacteristic *characteristic;

void setup()
{
  auto config = M5.config();
  M5.begin(config);

  // Set up BLE
  LB.begin("Basic Use");
  BLEService *service = LB.createService("0000");
  characteristic = LB.addCharacteristic(service, "0001");
  LB.startAdvertising();
}

void loop()
{
  static int count = 0;
  delay(1000);

  // Send a value
  characteristic->setValue(++count);
}
