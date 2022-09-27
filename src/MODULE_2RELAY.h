#ifndef _MODULE_2RELAY_H
#define _MODULE_2RELAY_H

#include "Arduino.h"
#include "Wire.h"
#include "pins_arduino.h"

#define MODULE_2RELAY_ADDR            0x25
#define MODULE_2RELAY_REG             0x00
#define MODULE_2RELAY_VERSION_REG     0xFE
#define MODULE_2RELAY_ADDR_CONFIG_REG 0xFF

class MODULE_2RELAY {
   private:
    uint8_t _addr;
    TwoWire* _wire;
    uint8_t _scl;
    uint8_t _sda;
    uint8_t _speed;
    bool writeBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
    bool readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);

   public:
    bool begin(TwoWire* wire = &Wire, uint8_t sda = SDA, uint8_t scl = SCL,
               uint8_t addr = MODULE_2RELAY_ADDR);
    bool setRelay(uint8_t index, bool state);
    bool setAllRelay(bool state);
    bool getRelayState(uint8_t index);
    bool reverseRelay();
    bool setDeviceAddr(uint8_t addr);
    uint8_t getVersion();
};

#endif
