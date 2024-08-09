# Lazy BLE

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/shamofu/library/LazyBLE.svg)](https://registry.platformio.org/libraries/shamofu/LazyBLE)

Just a BLE wrapper for my own use.

This is currently under development and may not work properly until it reaches `v1.0.0`. Therefore, it is not recommended for use at this time.

## Usage
1. Declare `BLECharacteristic*`s as global variables.
2. In your `setup()`,
   1. Call `LB.begin(deviceName)` to initialize BLE.
   2. Create `BLEService` instances using `LB.createService(serviceUUID)`.
   3. Add `BLECharacteristic`s to the service using `LB.addCharacteristic(service, characteristicUUID)`, and assign the returned values to the variables you declared before.
   4. Start advertising using `LB.startAdvertising()`.
3. In your `loop()`, you can use the characteristics as you like.

For more details, check out the examples.

## Tips
If you want to try the ***basic*** example, replace `board` under `[env]` in `platformio.ini` with the board you have, and then execute `pio run -e basic -t upload`.

Note that I am using the M5 series, so the examples are specific to them.

## License
MIT
