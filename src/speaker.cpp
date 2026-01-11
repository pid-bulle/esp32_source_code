#include "speaker.h"

// ---------- Note frequencies ----------
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_C6  1047

#define NOTE_DS5  622
#define NOTE_F5   698
#define NOTE_G5   784

#define REST 0

// ---------- Melodies (NOTE, DURATION) ----------

// Win melody (happy upward)
static const int winMelody[] = {
  NOTE_C4, 8, NOTE_E4, 8, NOTE_G4, 8,
  NOTE_C5, 4, NOTE_E5, 4, NOTE_C6, 2
};

// Loss melody (descending)
static const int lossMelody[] = {
  NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8,
  NOTE_G4, 4, NOTE_E4, 4, NOTE_C4, 2
};

Speaker::Speaker(uint8_t pin) : _pin(pin) {}

void Speaker::begin() {
  // nothing to initialize yet
}

void Speaker::playTone(uint32_t freq, uint32_t duration) {
  if (freq == REST) {
    delay(duration);
    return;
  }

  ledcAttach(_pin, freq, _resolution);
  ledcWrite(_pin, _volume);

  delay(duration);

  ledcDetach(_pin);
}

void Speaker::playMelody(const int *melody, int length, int tempo) {
  int wholenote = (60000 * 4) / tempo;

  for (int i = 0; i < length; i += 2) {
    int note = melody[i];
    int divider = melody[i + 1];

    int noteDuration = wholenote / divider;
    playTone(note, noteDuration * 0.9);
    delay(noteDuration * 0.1);
  }
}

// ---------- Public sound effects ----------

// 🔴 HALT: loud continuous buzzer
void Speaker::soundHalt() {
  ledcAttach(_pin, 1000, _resolution);
  ledcWrite(_pin, 255);   // maximum volume
  delay(800);
  ledcDetach(_pin);
}

// 🟢 GO: short chime
void Speaker::soundGo() {
  playTone(NOTE_C5, 150);
  playTone(NOTE_E5, 150);
}

// 🏆 WIN: happy melody
void Speaker::soundWin() {
  playMelody(winMelody, sizeof(winMelody) / sizeof(int), 120);
}

// 💀 LOSS: sad tune
void Speaker::soundLoss() {
  playMelody(lossMelody, sizeof(lossMelody) / sizeof(int), 90);
}

void Speaker::detach() {
  // Ensure speaker is silent
  ledcWrite(_pin, 0);

  // Release PWM hardware from the pin
  ledcDetach(_pin);
}