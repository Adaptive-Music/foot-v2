#include <Arduino.h>
#include <BLEMidi.h>

// GPIO pins to use for 8 touch pads
int pins[] = {T6, T4, T5, T7, T8, T3, T2, T9};

// Touch reading thresholds below which touchRead value means sensor is touched
int thresholds[] = {27, 31, 33, 28, 27, 24, 24, 30};

// Button for mode change
int buttonPin = T0;
int buttonThreshold = 52;

// Arrays to store sensor states
bool oldState[8] = {false};
bool newState[8] = {false};

// Define key - initially set to C4 (Middle C)
int key = 60;

// Define notes to be played by each button - numbers represent how many semitones above tonic (except for drum).
int scales[][8] = {
  {36, 38, 37, 43, 45, 42, 46, 49}, // Drum
  {0, 2, 4, 5, 7, 9, 11, 12},   // Major
  {0, 2, 3, 5, 7, 8, 10, 12},   // Minor
  {0, 3, 5, 7, 10, 12, 15, 17},  // Pentatonic
};


// // MIDI notes to send
// int defaultDrum[8] = {36, 38, 37, 43, 45, 42, 46, 49};
// int majorScale[8] = {60, 62, 64, 65, 67, 69, 71, 72};

const int DRUM_MODE = 0;
const int SINGLE_NOTE = 1;

const int NUM_MODES = 2;

int currentMode = DRUM_MODE;


void silence() {
  // Stop all currently playing notes
  for (int i = 0; i < 128; i++) {
    BLEMidiServer.noteOff(0, i, 127);
    delay(5);
  }
}

void changeMode() {
  silence();
  currentMode = (currentMode + 1) % NUM_MODES;
  Serial.printf("New mode: %d\n", currentMode);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  BLEMidiServer.begin("Zoe's foot keyboard");
}

void loop() {
  if (touchRead(buttonPin) < buttonThreshold) {
    changeMode();
    return;
  }

   // Read and store the current state of the pushbutton values
  for (int i = 0; i < 8; i++) newState[i] = touchRead(pins[i]) < thresholds[i];

  for (int i = 0; i < 8; i++) {
    // No action required if sensor unchanged
    if (oldState[i] == newState[i]) continue;
    // Update oldState with changed value
    oldState[i] = newState[i];
    if (!BLEMidiServer.isConnected()) continue;
    // Play/end note if pressed/released
    int note = scales[currentMode][i] + (currentMode == DRUM_MODE? 0 : key); 
    Serial.println(note);
    if (newState[i]) BLEMidiServer.noteOn(0, note, 127);
    else BLEMidiServer.noteOff(0, note, 127);
  }
  // Delay for debouncing
  delay(10);
}

