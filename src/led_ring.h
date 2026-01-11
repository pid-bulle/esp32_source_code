#pragma once
#include <Arduino.h>
#include <FastLED.h>

class LedRing {
public:
  LedRing(uint8_t dataPin);

  void begin();

  // Animations
  void countdown();        // Red → Yellow → Green (1s each)
  void showColor(CRGB c);  // solid center + animated ring

private:
  static const uint8_t NUM_LEDS = 7;
  static const uint8_t CENTER_LED = 0;

  uint8_t _pin;
  CRGB leds[NUM_LEDS];

  void animateRing(CRGB color, uint16_t durationMs);
};
