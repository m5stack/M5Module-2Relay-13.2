#ifndef _MODULE_2RELAY_H
#define _MODULE_2RELAY_H

#include "Arduino.h"
#include "Wire.h"
#include "pins_arduino.h"

#if __has_include(<M5Unified.h>)
#include <M5Unified.h>
#define MODULE_2RELAY_HAS_M5_I2C 1
#endif

#define MODULE_2RELAY_ADDR            0x25
#define MODULE_2RELAY_REG             0x00
#define MODULE_2RELAY_VERSION_REG     0xFE
#define MODULE_2RELAY_ADDR_CONFIG_REG 0xFF

class MODULE_2RELAY {
   private:
    uint8_t _addr = MODULE_2RELAY_ADDR;
    TwoWire* _wire = nullptr;
    uint8_t _scl = SCL;
    uint8_t _sda = SDA;
#if defined(MODULE_2RELAY_HAS_M5_I2C)
    m5::I2C_Class* _m5_i2c = nullptr;
    uint32_t _speed = 100000;
#endif
    bool writeBytes(uint8_t addr, uint8_t reg, const uint8_t* buffer,
                    uint8_t length);
    bool readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);

   public:
    bool begin(TwoWire* wire = &Wire, uint8_t sda = SDA, uint8_t scl = SCL,
               uint8_t addr = MODULE_2RELAY_ADDR);
#if defined(MODULE_2RELAY_HAS_M5_I2C)
    bool begin(m5::I2C_Class* i2c, uint8_t addr = MODULE_2RELAY_ADDR,
               uint32_t speed = 100000);
#endif
    bool setRelay(uint8_t index, bool state);
    bool setAllRelay(bool state);
    bool getRelayState(uint8_t index);
    bool reverseRelay();
    bool setDeviceAddr(uint8_t addr);
    uint8_t getVersion();
};

#endif
