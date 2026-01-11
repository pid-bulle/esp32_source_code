#include "led_ring.h"

// ---- Static field definitions ----
uint8_t LedRing::_r = 0;
uint8_t LedRing::_g = 0;
uint8_t LedRing::_b = 0;
uint8_t LedRing::_brightness = 255;

LedRing::LedRing(uint8_t pin, uint16_t ledCount)
    : _ledCount(ledCount),
      _strip(ledCount, pin, NEO_GRB + NEO_KHZ800)
{
}

void LedRing::begin() {
    _strip.begin();
    _strip.setBrightness(_brightness);
    _strip.show();
}

void LedRing::clear() {
    _strip.clear();
    _strip.show();
}

void LedRing::apply() {
    _strip.setBrightness(_brightness);

    uint32_t color = _strip.Color(_r, _g, _b);
    for (uint16_t i = 0; i < _ledCount; i++) {
        _strip.setPixelColor(i, color);
    }
    _strip.show();
}

// ---- Static state setters ----
void LedRing::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _r = r;
    _g = g;
    _b = b;
}

void LedRing::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

// ---- Traffic light presets ----
void LedRing::red() {
    setColor(255, 0, 0);
}

void LedRing::yellow() {
    setColor(255, 150, 0);
}

void LedRing::green() {
    setColor(0, 255, 0);
}

// ---- Animation ----
void LedRing::animateTrafficLight() {
    red();
    apply();
    delay(1000);

    yellow();
    apply();
    delay(1000);

    green();
    apply();
    delay(1000);
}
