/*
 * 02/22/2015
 * Test full uncalibrated system.
 */

#define BLUE_LED_PIN  13
#define GREEN_LED_PIN 12
#define RED_LED_PIN   11

#define VALVE_PIN          3
#define FLUSH_PUMP_PIN     4
#define INDICATOR_PUMP_PIN 5
#define SAMPLE_PUMP_PIN    6

#define CLEAN_TIME 10000
#define PRIME_SAMPLE_TIME 5000
#define INJECTION_TIME 2000

#define DROP_TIME 19
#define DELAY_BETWEEN_DROPS 200

void setup() {
  // set up LED
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // set up sensor
  Serial.begin(9600);
  
  // set up pumps + vavle
  pinMode(VALVE_PIN, OUTPUT);
  pinMode(FLUSH_PUMP_PIN, OUTPUT);
  pinMode(INDICATOR_PUMP_PIN, OUTPUT);
  pinMode(SAMPLE_PUMP_PIN, OUTPUT);
}

void loop() {
  //measurePh();
  injectIndicatorDrops(5);
  
  while(true){};  
}

void injectIndicatorDrops(int numDrops){
  for(int i = 0; i < numDrops; i++){
     runFlush(DROP_TIME);
     delay(DELAY_BETWEEN_DROPS);
  }
  return;
}

void runFlush(int milliseconds){
  runPump(milliseconds, FLUSH_PUMP_PIN);
  return;
}

void runIndicator(int milliseconds){
  runPump(milliseconds, INDICATOR_PUMP_PIN);
  return;
}

void runSample(int milliseconds){
  runPump(milliseconds, SAMPLE_PUMP_PIN);
  return;
}

void runPump(int milliseconds, int pumpPin){
    digitalWrite(pumpPin, HIGH);
    delay(milliseconds);
    digitalWrite(pumpPin, LOW);
  return;
}

void measurePh(void){
  injectSampleAndReagent();
  
  readSensorColor(BLUE_LED_PIN);
  readSensorColor(GREEN_LED_PIN);
  readSensorColor(RED_LED_PIN);
  
  cleanChamber();
  
  while(1==1){} //end here with inf loop
  return;
}

void readSensorColor(int ledPin){
  digitalWrite(ledPin, HIGH);
  delay(100);
  readSensor("Testing color... ");
  delay(100);
  digitalWrite(ledPin, LOW);
  return;
}

void injectSampleAndReagent(void){
  digitalWrite(VALVE_PIN, LOW); // ensure valve is closed
  digitalWrite(SAMPLE_PUMP_PIN, HIGH);
  delay(PRIME_SAMPLE_TIME);
  
  digitalWrite(INDICATOR_PUMP_PIN, HIGH);
  delay(INJECTION_TIME);
  
  digitalWrite(INDICATOR_PUMP_PIN, LOW);
  digitalWrite(SAMPLE_PUMP_PIN, LOW);
  return;
}
  

void cleanChamber(void){
  digitalWrite(VALVE_PIN, HIGH);
  digitalWrite(FLUSH_PUMP_PIN, HIGH);
  delay(CLEAN_TIME);
  
  digitalWrite(VALVE_PIN, LOW);
  digitalWrite(FLUSH_PUMP_PIN, LOW);
  return;
}

void readSensor(char* startStr){
  int sensorBucket  = analogRead(A0);
  float sensorVoltage = sensorBucket * (5.0 / 1023.0);
  Serial.print(startStr);
  Serial.print("Bucket: ");
  Serial.print(sensorBucket);
  Serial.print(" Voltage: ");
  Serial.println(sensorVoltage);
  return;
}

void setupThroughAll(void){
  readSensor("Nothing -- ");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  readSensor("BLUE -- ");
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  readSensor("GREEN -- ");
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second

  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  readSensor("RED -- ");
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  readSensor("Nothing -- ");
  
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  return;
}
