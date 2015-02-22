void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  Serial.print("Light ON --- ");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  
  // get reading from sensor and print
  readSensor();
  
  Serial.print("Light OFF --- ");
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  
  // get reading from sensor and print
  readSensor();
}

void readSensor(void){
  int sensorBucket  = analogRead(A0);
  float sensorVoltage = sensorBucket * (5.0 / 1023.0);
  Serial.print(" Bucket: ");
  Serial.print(sensorBucket);
  Serial.print(" Voltage: ");
  Serial.println(sensorVoltage);
  return;
}

