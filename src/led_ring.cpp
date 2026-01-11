#include "led_ring.h"

LedRing::LedRing(uint8_t pin)
    : strip(NUM_LEDS, pin, NEO_GRB + NEO_KHZ800),
      countdownActive(false),
      countdownStartTime(0),
      countdownPhase(0) {
}

void LedRing::begin() {
    strip.begin();
    strip.setBrightness(50); // Adjust brightness (0-255)
    clear();
    show();
}

void LedRing::setAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
}

void LedRing::setLed(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < NUM_LEDS) {
        strip.setPixelColor(index, strip.Color(r, g, b));
    }
}

void LedRing::clear() {
    strip.clear();
}

void LedRing::show() {
    strip.show();
}

void LedRing::startCountdown() {
    countdownActive = true;
    countdownStartTime = millis();
    countdownPhase = 0;
}

bool LedRing::isCountdownRunning() {
    return countdownActive;
}

bool LedRing::updateCountdown() {
    if (!countdownActive) {
        return true; // Already complete
    }

    unsigned long elapsed = millis() - countdownStartTime;
    unsigned long phaseTime = elapsed % 1000; // Time within current 1-second phase
    float progress = phaseTime / 1000.0f;     // 0.0 to 1.0 progress in current phase

    // Determine current phase based on elapsed time
    uint8_t currentPhase = elapsed / 1000;

    if (currentPhase > 2) {
        // Animation complete - show solid green briefly then stop
        setAll(0, 255, 0);
        show();
        countdownActive = false;
        return true;
    }

    countdownPhase = currentPhase;

    // Colors for each phase
    uint8_t r, g, b;
    switch (countdownPhase) {
        case 0: // Red
            r = 255; g = 0; b = 0;
            break;
        case 1: // Yellow
            r = 255; g = 180; b = 0;
            break;
        case 2: // Green
            r = 0; g = 255; b = 0;
            break;
        default:
            r = 0; g = 0; b = 0;
            break;
    }

    // Set center LED to solid color
    setLed(CENTER_LED, r, g, b);

    // Update ring animation
    updateRingAnimation(r, g, b, progress);

    show();
    return false;
}

void LedRing::updateRingAnimation(uint8_t r, uint8_t g, uint8_t b, float progress) {
    // Create a rotating fading effect around the ring
    // The "head" of the animation rotates around, with a fading tail

    float headPosition = progress * RING_COUNT; // Position of brightest LED (0-6)

    for (uint8_t i = 0; i < RING_COUNT; i++) {
        uint8_t ledIndex = RING_START + i;

        // Calculate distance from head (wrapping around)
        float distance = headPosition - i;
        if (distance < 0) {
            distance += RING_COUNT;
        }

        // Fade factor based on distance from head
        // Head is brightest, tail fades out over ~4 LEDs
        float fadeFactor;
        if (distance < 0.5f) {
            fadeFactor = 1.0f; // Head LED at full brightness
        } else if (distance < 4.0f) {
            // Gradual fade for tail
            fadeFactor = 1.0f - (distance / 4.0f);
        } else {
            fadeFactor = 0.1f; // Dim background glow
        }

        // Apply fade and set LED color
        uint8_t fr = fadeValue(r, fadeFactor);
        uint8_t fg = fadeValue(g, fadeFactor);
        uint8_t fb = fadeValue(b, fadeFactor);

        setLed(ledIndex, fr, fg, fb);
    }
}

uint8_t LedRing::fadeValue(uint8_t value, float factor) {
    return (uint8_t)(value * factor);
}
