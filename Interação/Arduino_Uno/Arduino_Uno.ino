#include <Wire.h>

void setup() {
  Wire.begin();
}

byte movimento = 0;

void seleciona_movimento(byte num){
  Wire.beginTransmission(8);
  Wire.write(num);
  Wire.endTransmission();
}

void loop() {
  seleciona_movimento(movimento);
  delay(1000);
}
