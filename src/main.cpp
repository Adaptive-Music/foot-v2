#include <Arduino.h>
#include <BLEMidi.h>

// Touch sensor pins
int pins[] = {4, 0, 2, 15, 13, 12, 14, 27, 33, 32};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Touch sensor readings");
}

void loop() {
  for (int i = 0; i < 10; i++) {
    Serial.printf("%d(%d): %03d, ", i, pins[i], touchRead(pins[i]));
  }
  Serial.println();
  delay(200);
}

