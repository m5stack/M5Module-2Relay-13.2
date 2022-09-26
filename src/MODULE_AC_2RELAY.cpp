#include "MODULE_AC_2RELAY.h"

bool MODULE_AC_2RELAY::begin(TwoWire *wire, uint8_t sda, uint8_t scl,
                             uint8_t addr) {
    _wire = wire;
    _addr = addr;
    _sda  = sda;
    _scl  = scl;
    _wire->begin(_sda, _scl);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

bool MODULE_AC_2RELAY::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                                  uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->write(buffer, length);
    if (_wire->endTransmission() == 0) return true;
    return false;
}

bool MODULE_AC_2RELAY::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                                 uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    if (_wire->requestFrom(addr, length)) {
        for (uint8_t i = 0; i < length; i++) {
            buffer[index++] = _wire->read();
        }
        return true;
    }
    return false;
}

bool MODULE_AC_2RELAY::getRelayState(uint8_t index) {
    uint8_t data = 0;
    if (readBytes(_addr, MODULE_AC_2RELAY_REG + index, &data, 1)) {
        return data ? true : false;
    }
    return false;
}

bool MODULE_AC_2RELAY::setRelay(uint8_t index, bool state) {
    uint8_t data = state ? 0xff : 0x00;
    if (writeBytes(_addr, MODULE_AC_2RELAY_REG + index, &data, 1)) {
        return data ? true : false;
    }
    return false;
}

bool MODULE_AC_2RELAY::setAllRelay(bool state) {
    uint8_t data[2] = {0};
    if (state) {
        memset(data, 0xff, 2);
    }
    if (writeBytes(_addr, MODULE_AC_2RELAY_REG, data, 2)) {
        return true;
    }
    return false;
}

bool MODULE_AC_2RELAY::reverseRelay() {
    uint8_t data[2] = {0};
    if (readBytes(_addr, MODULE_AC_2RELAY_REG, data, 2)) {
        for (uint8_t i = 0; i < 2; i++) {
            data[i] = ~data[i];
        }
        if (writeBytes(_addr, MODULE_AC_2RELAY_REG, data, 2)) {
            return true;
        }
    }
    return false;
}

uint8_t MODULE_AC_2RELAY::getVersion() {
    uint8_t data = 0;
    readBytes(_addr, MODULE_AC_2RELAY_VERSION_REG, &data, 1);
    return data;
}

bool MODULE_AC_2RELAY::setDeviceAddr(uint8_t addr) {
    uint8_t data = addr;
    if (writeBytes(_addr, MODULE_AC_2RELAY_ADDR_CONFIG_REG, &data, 1)) {
        _addr = addr;
        return true;
    } else {
        return false;
    }
}
