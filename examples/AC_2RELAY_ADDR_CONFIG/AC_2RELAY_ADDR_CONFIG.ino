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

void setup() {
    M5.begin();  // Init M5Stack.  初始化M5Stack
    while (!RELAY.begin(&Wire, 21, 22, MODULE_AC_2RELAY_ADDR)) {
        Serial.println("AC 2RELAY INIT ERROR");
        M5.Lcd.println("AC 2RELAY INIT ERROR");
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
    canvas.drawString("Module AC 2Relay", 160, 40);
    canvas.drawString("Click BtnA Update", 160, 100);
    canvas.drawString("Addr to 0x66", 160, 160);
    canvas.pushSprite(0, 0);
}

void loop() {
    if (M5.BtnA.wasPressed()) {
        if (RELAY.setDeviceAddr(0x66)) {
            canvas.drawString("Success!", 160, 200);
            canvas.pushSprite(0, 0);
        };
    }
    M5.update();
}
