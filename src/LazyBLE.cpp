#include "LazyBLE.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <BLECharacteristic.h>
#include <BLEAdvertising.h>
#include <BLEDescriptor.h>
#include <BLEUUID.h>
#include <BLE2902.h>

#include <random>

lazyble::LazyBLE LB;

namespace lazyble
{
  LazyBLE::LazyBLE()
  {
  }

  void LazyBLE::begin(String deviceName, BLEServerCallbacks *callbacks)
  {
    _deviceName = deviceName;
    BLEDevice::init(_deviceName.c_str());
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);

    _server = BLEDevice::createServer();
    _server->setCallbacks(callbacks);

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->setScanResponse(true);
  }

  void LazyBLE::begin(String deviceName, bool enablePasskey, BLEServerCallbacks *callbacks)
  {
    _enablePasskey = true;
    begin(deviceName, callbacks);
  }

  BLEService *LazyBLE::createService(String serviceUUID)
  {
    BLEService *service = _server->createService(serviceUUID.c_str());

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(service->getUUID());

    _services.push_back(std::pair<bool, BLEService *>(false, service));

    return service;
  }

  BLECharacteristic *LazyBLE::addCharacteristic(BLEService *service, String characteristicUUID, uint32_t properties)
  {
    BLECharacteristic *characteristic = service->createCharacteristic(characteristicUUID.c_str(), properties);

    if (properties & BLECharacteristic::PROPERTY_NOTIFY)
    {
      BLE2902 *descriptor = new BLE2902();
      characteristic->addDescriptor(descriptor);
    }

    return characteristic;
  }

  BLECharacteristic *LazyBLE::addCharacteristic(BLEService *service, String characteristicUUID, String description, uint32_t properties)
  {
    BLECharacteristic *characteristic = addCharacteristic(service, characteristicUUID, properties);

    auto userDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    userDescriptor->setValue(description.c_str());
    characteristic->addDescriptor(userDescriptor);

    return characteristic;
  }

  void LazyBLE::startAdvertising()
  {
    for (auto &pair : _services)
    {
      if (!pair.first)
      {
        pair.second->start();
        pair.first = true;
      }
    }

    if (_enablePasskey)
    {
      std::mt19937 mt(micros());
      std::uniform_int_distribution<uint32_t> dist(100000, 999999);
      _passKey = dist(mt);

      BLESecurity *security = new BLESecurity();
      security->setStaticPIN(_passKey);
      security->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
    }

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->start();
  }

  String LazyBLE::getAddress() const
  {
    return BLEDevice::getAddress().toString().c_str();
  }

  uint32_t LazyBLE::getPassKey() const
  {
    return _passKey;
  }
}
