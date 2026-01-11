#ifndef LED_RING_H
#define LED_RING_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedRing {
public:
    LedRing(uint8_t pin);

    void begin();

    // Set all LEDs to a color
    void setAll(uint8_t r, uint8_t g, uint8_t b);

    // Set individual LED (0 = center, 1-6 = ring)
    void setLed(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

    // Clear all LEDs
    void clear();

    // Update the LED strip (call after setting colors)
    void show();

    // Non-blocking countdown animation: red -> yellow -> green
    // Call this in loop(), returns true when animation is complete
    bool updateCountdown();

    // Start the countdown animation
    void startCountdown();

    // Check if countdown is running
    bool isCountdownRunning();

private:
    static const uint8_t NUM_LEDS = 7;
    static const uint8_t CENTER_LED = 0;
    static const uint8_t RING_START = 1;
    static const uint8_t RING_COUNT = 6;

    Adafruit_NeoPixel strip;

    // Countdown state
    bool countdownActive;
    unsigned long countdownStartTime;
    uint8_t countdownPhase; // 0=red, 1=yellow, 2=green

    // Animation helpers
    void updateRingAnimation(uint8_t r, uint8_t g, uint8_t b, float progress);
    uint8_t fadeValue(uint8_t value, float factor);
};

#endif
