#include <Servo.h>

Servo ESC;     // create servo object to control the ESC
#define ESC_PIN 26
#define RPM_PIN 6
#define LOAD_PIN 8
#define RECORD_TIME 2 
#define NUM_TARGET_SPEEDS 10
#define NUM_SAMPLES 3

void setup() {
  pinMode(RPM_PIN, INPUT);
  pinMode(LOAD_PIN, INPUT);
  pinMode(ESC_PIN, INPUT);//pinMode(8, INPUT);
  // put your setup code here, to run once:
  ESC.attach(ESC_PIN);                    
  ESC.writeMicroseconds(1500);
  delay(7000);
}

void loop() {
  int target_speed[] = {1000, 1100, 1200, 1300, 1400, 1600, 1700, 1800, 1900, 2000};
  while(Serial.read() == 0xF0F0){
    for(int i = 0; i < NUM_TARGET_SPEEDS; i++){
      ESC.writeMicroseconds(target_speed[i]);
      delay(5000); 
      // cease all operations and record samples for the defined time 
      int start_time = millis();
      int end_time = start_time; 
      for(int samples = 0; i < NUM_SAMPLES && (end_time - start_time) >= RECORD_TIME *1000 ; i++) {
        Serial.write(0x00);
        Serial.write(0x00);
        
        uint8_t rpm_value = analogRead(RPM_PIN);
        uint8_t load_value = digitalRead(LOAD_PIN);
        Serial.write(rpm_value);
        Serial.write(load_value);
        
        Serial.write(0x00);
        Serial.write(0x00);
        // set motor to zero speed
        end_time = millis();
        
        }
     
        ESC.writeMicroseconds(1500); 
       
    
    }
  
    
  }

}
