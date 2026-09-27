#include <Arduino.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
  Serial.println("LED ON");
  delay(1000);
}