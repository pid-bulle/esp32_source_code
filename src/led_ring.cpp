#include "led_ring.h"

// ---- Static state ----
uint8_t LedRing::_r = 0;
uint8_t LedRing::_g = 0;
uint8_t LedRing::_b = 0;
uint8_t LedRing::_brightness = 255;

LedRing* LedRing::_instances[4] = { nullptr };
uint8_t LedRing::_instanceCount = 0;

LedRing::LedRing(uint8_t pin, uint16_t ledCount)
    : _ledCount(ledCount),
      _strip(ledCount, pin, NEO_GRB + NEO_KHZ800)
{
}

void LedRing::begin() {
    _strip.begin();
    _strip.setBrightness(_brightness);
    _strip.show();

    // Register instance
    if (_instanceCount < 4) {
        _instances[_instanceCount++] = this;
    }
}

void LedRing::clear() {
    _strip.clear();
    _strip.show();
}

// ---- Instance apply ----
void LedRing::apply() {
    _strip.setBrightness(_brightness);

    uint32_t color = _strip.Color(_r, _g, _b);
    for (uint16_t i = 0; i < _ledCount; i++) {
        _strip.setPixelColor(i, color);
    }
    _strip.show();
}

// ---- Global setters ----
void LedRing::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _r = r;
    _g = g;
    _b = b;
    applyAll();
}

void LedRing::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    applyAll();
}

// ---- Global color shortcuts ----
void LedRing::red() {
    setColor(255, 0, 0);
}

void LedRing::yellow() {
    setColor(255, 150, 0);
}

void LedRing::green() {
    setColor(0, 255, 0);
}

// ---- Apply to ALL instances ----
void LedRing::applyAll() {
    for (uint8_t i = 0; i < _instanceCount; i++) {
        _instances[i]->apply();
    }
}

// ---- Global animation ----
void LedRing::animateTrafficLight() {
    red();
    delay(1000);

    yellow();
    delay(1000);

    green();
    delay(1000);
}
