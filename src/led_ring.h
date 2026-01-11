#ifndef LED_RING_H
#define LED_RING_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedRing {
public:
    LedRing(uint8_t pin, uint16_t ledCount);

    void begin();
    void clear();

    // ---- GLOBAL CONTROL (no object needed) ----
    static void setColor(uint8_t r, uint8_t g, uint8_t b);
    static void setBrightness(uint8_t brightness);

    static void red();
    static void yellow();
    static void green();

    static void applyAll();
    static void animateTrafficLight();

private:
    uint16_t _ledCount;
    Adafruit_NeoPixel _strip;

    void apply(); // apply static state to THIS instance

    // ---- Global shared state ----
    static uint8_t _r;
    static uint8_t _g;
    static uint8_t _b;
    static uint8_t _brightness;

    // ---- Instance registry ----
    static LedRing* _instances[4];   // supports up to 4 rings
    static uint8_t _instanceCount;
};

#endif
