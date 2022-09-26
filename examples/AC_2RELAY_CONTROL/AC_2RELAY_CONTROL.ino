/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more
*information：https://docs.m5stack.com/en/module/2Relay%2013.2%20Module
* 获取更多资料请访问：https://docs.m5stack.com/en/module/2Relay%2013.2%20Module
*
* describe: Module 2Relay.
* date：2022/09/26
*******************************************************************************
*/

#include <M5Stack.h>
#include "MODULE_AC_2RELAY.h"
#include <M5GFX.h>

M5GFX display;
M5Canvas canvas(&display);

MODULE_AC_2RELAY RELAY;

void drwaRect() {
    canvas.fillSprite(0);
    canvas.drawString("Module AC 2Relay", 160, 20);
    canvas.drawString("A REVERSE", 220, 80);
    canvas.drawString("B STEP", 220, 120);
    canvas.drawString("C RUNING", 220, 160);
    canvas.drawString("FW VERSION:" + String(RELAY.getVersion()), 160, 200);

    for (uint8_t i = 0; i < 2; i++) {
        if (RELAY.getRelayState(i) == 1) {
            canvas.fillRect(20, 80 + 70 * i, 100, 20);
        } else {
            canvas.drawRect(20, 80 + 70 * i, 100, 20);
        }
    }
    canvas.pushSprite(0, 0);
}

void setup() {
    M5.begin();  // Init M5Stack.  初始化M5Stack
    while (!RELAY.begin(&Wire, 21, 22, MODULE_AC_2RELAY_ADDR)) {
        Serial.println("AC 2RELAY INIT ERROR");
        delay(1000);
    };
    Serial.println("AC 2RELAY INIT SUCCESS");
    display.begin();
    canvas.setColorDepth(1);
    canvas.setFont(&fonts::Orbitron_Light_24);
    canvas.setTextSize(1);
    canvas.setTextDatum(MC_DATUM);
    canvas.createSprite(display.width(), display.height());
    canvas.setPaletteColor(1, ORANGE);
    canvas.pushSprite(0, 0);
}

uint8_t mode = 0;

bool delayBtn(unsigned long start) {
    while (1) {
        M5.update();
        if (M5.BtnA.wasPressed()) {
            mode = 0;
            return true;
        }
        if (M5.BtnB.wasPressed()) {
            mode = 1;
            return true;
        }
        if (M5.BtnC.wasPressed()) {
            mode = 2;
            return true;
        }
        if ((millis() - start) > 500) {
            return false;
        }
    }
}

void loop() {
    switch (mode) {
        case 0:
            RELAY.setAllRelay(true);
            drwaRect();
            if (delayBtn(millis())) {
                RELAY.setAllRelay(false);
                break;
            };
            RELAY.setAllRelay(false);
            drwaRect();
            if (delayBtn(millis())) {
                RELAY.setAllRelay(false);
                break;
            };
            break;
        case 1:
            for (uint8_t i = 0; i < 2; i++) {
                while (!delayBtn(millis()))
                    ;
                if (mode != 1) break;
                RELAY.setRelay(i, true);
                drwaRect();
            }
            for (uint8_t i = 0; i < 2; i++) {
                while (!delayBtn(millis()))
                    ;
                if (mode != 1) break;
                RELAY.setRelay(i, false);
                drwaRect();
            }
            break;
        case 2:
            for (uint8_t i = 0; i < 2; i++) {
                RELAY.setRelay(i, true);
                drwaRect();
                if (delayBtn(millis())) {
                    break;
                };
            }
            for (uint8_t i = 0; i < 2; i++) {
                RELAY.setRelay(i, false);
                drwaRect();
                if (delayBtn(millis())) {
                    break;
                };
            }
            break;
    }
}
