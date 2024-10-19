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
    uint8_t touchReading = touchRead(pins[i]);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(touchReading);
    Serial.print(", ");
  }
  Serial.println("\n");
  delay(1000);
}

