#include <Servo.h>

Servo servo1;

void setup() {
  
    //servo1.attach(8, 0, 90);
    servo1.attach(8);
    //servo1.write(90);
    //relax2();
}
void loop() {
  relax2();
    
}

void relax1(){
    servo1.write(0);
    delay(1000);
    servo1.write(180);
    delay(1000);
    servo1.write(60);
    delay(1000);
    servo1.write(120);
    delay(1000);
    servo1.write(30);
    delay(1000);
    servo1.write(150);
    delay(1000);
    servo1.write(90);
    delay(1000);
}

void relax2(){
    servo1.write(0);
    delay(2000);
    servo1.write(180);
    delay(2000);
    servo1.write(90);
    delay(2000);

}
