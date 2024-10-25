#include <Arduino.h>
#include <vector>
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

// Define notes to play in drum mode
int drums[] = {36, 38, 37, 43, 45, 42, 46, 49}; 


// Define notes to be played by each button - numbers represent how many semitones above tonic.
int scales[][8] = {
  {0, 2, 4, 5, 7, 9, 11, 12},   // Major
  {0, 2, 3, 5, 7, 8, 10, 12},   // Minor
  {0, 3, 5, 7, 10, 12, 15, 17},  // Pentatonic
};

// Constants to define scales
const int MAJOR = 0;
const int MINOR = 1;
const int PENTATONIC = 2;

// Currently selected scale
int currentScale = MAJOR;


// Constants to define modes
const int DRUM_MODE = 0;
const int SINGLE_NOTE = 1;
const int TRIAD_CHORD = 2;
const int POWER_CHORD = 3;


const int NUM_MODES = 4;

int currentMode = DRUM_MODE;


// Vector to store chord's notes for playing/ending
std::vector<int> notes;


void silence() {
  // Stop all currently playing notes
  for (int i = 0; i < 128; i++) {
    BLEMidiServer.noteOff(0, i, 127);
    delay(3);
  }
}

void playOrEndNotes(int i, bool noteOn) {
  notes.clear();
  int rootNote = key + scales[currentScale][i];

  // Drum mode: Use drum note
  if (currentMode == DRUM_MODE) {
    notes = {drums[i]};
  }
  // Power chord: I, V, VIII (0-7-12)
  else if (currentMode == POWER_CHORD) {
    notes = {rootNote - 12, rootNote - 5, rootNote};
  } 

  // Triad chords: Major (0-4-7), Minor (0-3-7), and Diminished (0-3-6)
  else if (currentMode == TRIAD_CHORD && currentScale != PENTATONIC) {
    int thirdPos = (i + 2) % 7;
    int fifthPos = (i + 4) % 7;

    int thirdNote = i > thirdPos ? key + scales[currentScale][thirdPos] + 12 : key + scales[currentScale][thirdPos];
    int fifthNote = i > fifthPos ? key + scales[currentScale][fifthPos] + 12 : key + scales[currentScale][fifthPos];
    
    notes = {rootNote - 12, thirdNote - 12, fifthNote - 12, rootNote};
  }

  // Default to single note
  else {
    notes = {rootNote};
  }
  // Play or end each note
  for (int note : notes) {
    if (noteOn) BLEMidiServer.noteOn(0, note, 127);
    else BLEMidiServer.noteOff(0, note, 0);
  } 
}

void changeMode() {
  currentMode = (currentMode + 1) % NUM_MODES;
  Serial.printf("New mode: %d\n", currentMode);
  silence();
  playOrEndNotes(0, true);
  delay(1000);
  playOrEndNotes(0, false);}

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
    playOrEndNotes(i, newState[i]);
  }
  // Delay for debouncing
  delay(10);
}

