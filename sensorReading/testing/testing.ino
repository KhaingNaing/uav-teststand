
// Helper macro to merge 2 bytes
#define TO_INT16(a,b) (((b)<<8)|(a))
#define TEST_PIN A0
#include <stdlib.h>

uint16_t combined;
unsigned long time;
int trigger = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TEST_PIN, INPUT);
}
uint16_t read_int()
{
  uint16_t result[2];
  for (int i = 0; i < 2; i++) {
    // Wait for data at the port
    // while(!Serial.available())
    result[i] = Serial.read();
    Serial.print(result[i], HEX);
  }
  return TO_INT16(result[0], result[1]);
}

void loop() {
  if (Serial.read()) {
    Serial.print("READY!!!!!!!!");
    Serial.print(Serial.read());
    // combined = read_int();
    // Serial.print(combined);
  }
  if(!Serial.available()){
    Serial.print("not available\n");
    Serial.println(Serial.available());

    delay(1000);
  }
  // if (trigger == false) {
  //   // combined = read_int();
  //   // delay(1000);
  //   Serial.print(combined);

  //   Serial.print("Time: ");
  //   time = millis();
  //   //prints time since program started
  //   Serial.println(time);
  //   // wait a second so as not to send massive amounts of data
  //   delay(1000);
  // }
  
}

