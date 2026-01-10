#pragma once
#include <Arduino.h>

class Speaker {
public:
  explicit Speaker(uint8_t pin);

  void begin();

  // Sound effects
  void soundHalt();   // loud buzzer
  void soundGo();     // short chime
  void soundWin();    // melody
  void soundLoss();   // tune

private:
  uint8_t _pin;
  uint8_t _resolution = 8;
  uint32_t _volume = 200;  // loud by default

  void playTone(uint32_t freq, uint32_t duration);
  void playMelody(const int *melody, int length, int tempo);
};
