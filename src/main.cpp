#include <Arduino.h>
#include <BLEMidi.h>

// Touch sensor pins
int pins[] = {T6, T4, T5, T7, T8, T3, T2, T9, T0};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Touch sensor readings");
}

void loop() {
  for (int i = 0; i < 9; i++) {
    Serial.printf("%d: %03d  ", i, touchRead(pins[i]));
  }
  Serial.println();
  delay(200);
}

