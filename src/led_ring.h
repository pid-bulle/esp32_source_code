#ifndef LED_RING_H
#define LED_RING_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedRing {
public:
    LedRing(uint8_t pin, uint16_t ledCount);

    void begin();
    void clear();
    void show();

    // Generic color control
    void setAll(uint8_t r, uint8_t g, uint8_t b);
    void setAll(uint32_t color);
    uint32_t color(uint8_t r, uint8_t g, uint8_t b);

    // Traffic light colors
    void red();
    void yellow();
    void green();

    // Traffic light animation
    void animateTrafficLight();

private:
    uint16_t _ledCount;
    Adafruit_NeoPixel _strip;
};

#endif // LED_RING_H
