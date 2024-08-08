#ifndef LAZYBLE_H_
#define LAZYBLE_H_

#include <Arduino.h>
#include <BLESecurity.h>
#include <BLEServer.h>

#include <vector>

namespace lazyble
{
  class LazyBLE
  {
  private:
    BLEServer *_server;
    std::vector<std::pair<bool, BLEService *>> _services;
    String _deviceName;
    bool _enablePasskey = false;
    uint32_t _passKey;

  public:
    LazyBLE();

    void begin(String deviceName, BLEServerCallbacks *callbacks = new BLEServerCallbacks());
    void begin(String deviceName, bool enablePasskey, BLEServerCallbacks *callbacks = new BLEServerCallbacks());

    BLEService *createService(String serviceUUID);

    BLECharacteristic *addCharacteristic(BLEService *service, String characteristicUUID, uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    BLECharacteristic *addCharacteristic(BLEService *service, String characteristicUUID, String description, uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

    void startAdvertising();

    String getAddress() const;
    uint32_t getPassKey() const;
  };
}

extern lazyble::LazyBLE LB;

#endif
