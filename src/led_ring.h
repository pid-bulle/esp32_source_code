#ifndef LED_RING_H
#define LED_RING_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedRing {
public:
    LedRing(uint8_t pin, uint16_t ledCount);

    void begin();
    void clear();
    void apply();   // apply static color + brightness to LEDs

    // ---- Static global LED state ----
    static void setColor(uint8_t r, uint8_t g, uint8_t b);
    static void setBrightness(uint8_t brightness);

    // Traffic light colors
    static void red();
    static void yellow();
    static void green();

    // Traffic light animation (blocking)
    void animateTrafficLight();

private:
    uint16_t _ledCount;
    Adafruit_NeoPixel _strip;

    // ---- Static shared state ----
    static uint8_t _r;
    static uint8_t _g;
    static uint8_t _b;
    static uint8_t _brightness;
};

#endif // LED_RING_H
