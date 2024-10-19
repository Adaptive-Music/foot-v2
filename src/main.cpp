#include <Arduino.h>
#include <BLEMidi.h>

// GPIO pins to use for 8 touch pads
int pins[] = {
  4, // T0
  // 0, // T1 not working
  2, // T2
  15, // T3
  13, // T4
  12, // T5
  14, // T6
  27, // T7
  33, // T8
  // 32, // T9 not used
  };

// Arrays to store sensor states
bool oldState[8] = {false};
bool newState[8] = {false};

// MIDI notes to send
int defaultDrum[8] = {36, 38, 37, 43, 45, 42, 46, 49};
bool drumMode = false;

// Touch reading threshold below which touchRead value means sensor is touched
const int threshold = 50;




void setup() {
  Serial.begin(115200);
  BLEMidiServer.begin("Zoe's foot keyboard");
}

void loop() {

   // Read and store the current state of the pushbutton values
  for (int i = 0; i < 8; i++) newState[i] = touchRead(pins[i]) < threshold;

  for (int i = 0; i < 8; i++) {
    // No action required if sensor unchanged
    if (oldState[i] == newState[i]) continue;
    // Update oldState with changed value
    oldState[i] = newState[i];
    if (!BLEMidiServer.isConnected()) break;
    // Play/end note if pressed/released
    if (newState[i]) BLEMidiServer.noteOn(0, defaultDrum[i], 127);
    else BLEMidiServer.noteOff(0, defaultDrum[i], 127);
  }
  // Delay for debouncing
  delay(10);
}

