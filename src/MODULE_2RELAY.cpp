#include "MODULE_2RELAY.h"

bool MODULE_2RELAY::begin(TwoWire *wire, uint8_t sda, uint8_t scl,
                          uint8_t addr) {
    if (wire == nullptr) {
        return false;
    }

    _wire = wire;
#if defined(MODULE_2RELAY_HAS_M5_I2C)
    _m5_i2c = nullptr;
#endif
    _addr = addr;
    _sda  = sda;
    _scl  = scl;
    if (!_wire->begin((int)_sda, (int)_scl)) {
        return false;
    }
    delay(10);
    _wire->beginTransmission(_addr);
    return _wire->endTransmission() == 0;
}

#if defined(MODULE_2RELAY_HAS_M5_I2C)
bool MODULE_2RELAY::begin(m5::I2C_Class* i2c, uint8_t addr,
                          uint32_t speed) {
    if (i2c == nullptr || !i2c->isEnabled()) {
        return false;
    }

    _m5_i2c = i2c;
    _wire = nullptr;
    _addr = addr;
    _speed = speed;
    return _m5_i2c->scanID(_addr, _speed);
}
#endif

bool MODULE_2RELAY::writeBytes(uint8_t addr, uint8_t reg,
                               const uint8_t *buffer,
                               uint8_t length) {
    if (buffer == nullptr || length == 0) {
        return false;
    }

#if defined(MODULE_2RELAY_HAS_M5_I2C)
    if (_m5_i2c != nullptr) {
        bool success = _m5_i2c->start(addr, false, _speed);
        if (success) {
            success = _m5_i2c->write(reg);
        }
        if (success) {
            success = _m5_i2c->write(buffer, length);
        }
        const bool stopped = _m5_i2c->stop();
        return success && stopped;
    }
#endif
    if (_wire == nullptr) {
        return false;
    }

    _wire->beginTransmission(addr);
    if (_wire->write(reg) != 1 || _wire->write(buffer, length) != length) {
        _wire->endTransmission();
        return false;
    }
    return _wire->endTransmission() == 0;
}

bool MODULE_2RELAY::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                              uint8_t length) {
    if (buffer == nullptr || length == 0) {
        return false;
    }

#if defined(MODULE_2RELAY_HAS_M5_I2C)
    if (_m5_i2c != nullptr) {
        bool success = _m5_i2c->start(addr, false, _speed);
        if (success) {
            success = _m5_i2c->write(reg);
        }
        const bool writeStopped = _m5_i2c->stop();
        if (!success || !writeStopped) {
            return false;
        }

        success = _m5_i2c->start(addr, true, _speed);
        if (success) {
            success = _m5_i2c->read(buffer, length, true);
        }
        const bool readStopped = _m5_i2c->stop();
        return success && readStopped;
    }
#endif
    if (_wire == nullptr) {
        return false;
    }

    _wire->beginTransmission(addr);
    if (_wire->write(reg) != 1 || _wire->endTransmission() != 0) {
        return false;
    }

    const size_t received = _wire->requestFrom(addr, (size_t)length);
    if (received != length) {
        while (_wire->available()) {
            _wire->read();
        }
        return false;
    }

    for (uint8_t i = 0; i < length; i++) {
        if (!_wire->available()) {
            return false;
        }
        const int value = _wire->read();
        if (value < 0) {
            return false;
        }
        buffer[i] = (uint8_t)value;
    }
    return true;
}

bool MODULE_2RELAY::getRelayState(uint8_t index) {
    if (index >= 2) {
        return false;
    }

    uint8_t data = 0;
    if (readBytes(_addr, MODULE_2RELAY_REG + index, &data, 1)) {
        return data ? true : false;
    }
    return false;
}

bool MODULE_2RELAY::setRelay(uint8_t index, bool state) {
    if (index >= 2) {
        return false;
    }

    uint8_t data = state ? 0xff : 0x00;
    return writeBytes(_addr, MODULE_2RELAY_REG + index, &data, 1);
}

bool MODULE_2RELAY::setAllRelay(bool state) {
    uint8_t data[2] = {0};
    if (state) {
        memset(data, 0xff, 2);
    }
    return writeBytes(_addr, MODULE_2RELAY_REG, data, 2);
}

bool MODULE_2RELAY::reverseRelay() {
    uint8_t data[2] = {0};
    for (uint8_t i = 0; i < 2; i++) {
        if (!readBytes(_addr, MODULE_2RELAY_REG + i, &data[i], 1)) {
            return false;
        }
        data[i] = ~data[i];
    }
    return writeBytes(_addr, MODULE_2RELAY_REG, data, 2);
}

uint8_t MODULE_2RELAY::getVersion() {
    uint8_t data = 0;
    readBytes(_addr, MODULE_2RELAY_VERSION_REG, &data, 1);
    return data;
}

bool MODULE_2RELAY::setDeviceAddr(uint8_t addr) {
    uint8_t data = addr;
    if (writeBytes(_addr, MODULE_2RELAY_ADDR_CONFIG_REG, &data, 1)) {
        _addr = addr;
        return true;
    } else {
        return false;
    }
}
