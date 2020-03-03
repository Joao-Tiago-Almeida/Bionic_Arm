/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo servo1; 
Servo servo2; 
Servo servo3; 
Servo servo4; 
Servo servo5; 
Servo servo6; 


void setup() {
  int num = 12;
  servo1.attach(num);
  servo2.attach(num-1);
  servo3.attach(num-2);
  servo4.attach(num-3);
  servo5.attach(num-4);
  servo6.attach(num-5);  
  
}
void loop() {
  servo1.write(0); //polegar 
  delay(200);
  servo2.write(0); //indicador
  delay(200);
  servo3.write(0); //meio
  delay(200);
  servo4.write(0); //anelar
  delay(200);
  servo5.write(0); //mindinho
  delay(200);
  servo6.write(100); //pulso 
  delay(100);
}
