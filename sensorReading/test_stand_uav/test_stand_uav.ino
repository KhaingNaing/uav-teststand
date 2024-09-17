/*

*/
#include <Servo.h>
#define ESC_PIN 9
#define potentiometer A0
// digital
#define LOAD_CELL 7
#define RPM_PIN 8
#define DEBUG 1 
Servo servo;

void setup() {
  Serial.begin(9600);

  servo.attach(ESC_PIN);
  pinMode(ESC_PIN, OUTPUT);
  pinMode(LOAD_CELL, INPUT);
  pinMode(RPM_PIN, INPUT);
  // motor in stop position 
  servo.write(90);
  // delay for the motor to stop 
  delay(7000);
}
void testRPM(){
  
}

void loop() {
  // write two null bytes to signal start of data 
  Serial.write(0x00);
  Serial.write(0x00);
  // increase the angle from 0 to 180 
    for(int angle = 0; angle =+ 10; angle <= 180) {
      servo.write(angle);
      // one second delay
      delay(1000);
      int rpm_value = analogRead(RPM_PIN);
      int thurst_value = analogRead(LOAD_CELL);
      // to map into digital 
      int thrust_digital = map(thurst_value, 0, 1023, 0, 255);
      
      //not sure
      Serial.write(rpm_value);
      Serial.write(thrust_digital);
      
      
    }
    // two null bytes for end of data 
    Serial.write(0x00);
    Serial.write(0x00);
}
