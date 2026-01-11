#include "led_ring.h"

LedRing::LedRing(uint8_t pin, uint16_t ledCount)
    : _ledCount(ledCount),
      _strip(ledCount, pin, NEO_GRB + NEO_KHZ800)
{
}

void LedRing::begin() {
    _strip.begin();
    _strip.show(); // LEDs off
}

void LedRing::clear() {
    _strip.clear();
    _strip.show();
}

void LedRing::show() {
    _strip.show();
}

void LedRing::setAll(uint8_t r, uint8_t g, uint8_t b) {
    setAll(_strip.Color(r, g, b));
}

void LedRing::setAll(uint32_t color) {
    for (uint16_t i = 0; i < _ledCount; i++) {
        _strip.setPixelColor(i, color);
    }
    _strip.show();
}

uint32_t LedRing::color(uint8_t r, uint8_t g, uint8_t b) {
    return _strip.Color(r, g, b);
}

// 🚦 Traffic light colors
void LedRing::red() {
    setAll(255, 0, 0);
}

void LedRing::yellow() {
    setAll(255, 50, 0); // warm yellow (better than pure 255,255,0)
}

void LedRing::green() {
    setAll(0, 255, 0);
}

// 🚦 Traffic light animation
void LedRing::animateTrafficLight() {
    red();
    delay(1000);

    yellow();
    delay(1000);

    green();
    delay(1000);
}
