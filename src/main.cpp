#include <Arduino.h>
#include <BLEMidi.h>

// GPIO pins to use for 8 touch pads
int pins[] = {T6, T4, T5, T7, T8, T3, T2, T9};

// Touch reading thresholds below which touchRead value means sensor is touched
int thresholds[] = {27, 29, 33, 28, 27, 24, 24, 30};

// Button for mode change
int buttonPin = T0;
int buttonThreshold = 52;

// Arrays to store sensor states
bool oldState[8] = {false};
bool newState[8] = {false};

// MIDI notes to send
int defaultDrum[8] = {36, 38, 37, 43, 45, 42, 46, 49};
int majorScale[8] = {60, 62, 64, 65, 67, 69, 71, 72};
bool drumMode = false;


void setup() {
  Serial.begin(115200);
  BLEMidiServer.begin("Zoe's foot keyboard");
}

void loop() {
  if 

   // Read and store the current state of the pushbutton values
  for (int i = 0; i < 8; i++) newState[i] = touchRead(pins[i]) < thresholds[i];

  for (int i = 0; i < 8; i++) {
    // No action required if sensor unchanged
    if (oldState[i] == newState[i]) continue;
    // Update oldState with changed value
    oldState[i] = newState[i];
    if (!BLEMidiServer.isConnected()) break;
    // Play/end note if pressed/released
    if (newState[i]) BLEMidiServer.noteOn(0, majorScale[i], 127);
    else BLEMidiServer.noteOff(0, majorScale[i], 127);
  }
  // Delay for debouncing
  delay(10);
}

