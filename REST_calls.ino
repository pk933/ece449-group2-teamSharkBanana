/*

Testing for simple REST calls by using the "curl" command from a remote computer

* myArduino IP Address: 10.190.83.87
* myArduino#2 IP Address:10.190.79.172

 */

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
YunServer server;

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  Bridge.begin();

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }
  delay(50); // Poll every 50ms
}

void process(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "digital") {
    digitalCommand(client);
  }

  // is "analog" command?
  if (command == "analog") {
    analogCommand(client);
  }

  // is "mode" command?
  if (command == "mode") {
    modeCommand(client);
  }
  
  // is "loop" command?
  if (command == "loop"){
    loopCommand(client);
  }
  
  // is "alternate" command?
  if (command == "alter") {
    alterCommand(client);
  }
  
  // is "all" command?
  if (command =="all"){
    allCommand(client);
  }
  
}

// either turn pins 11 ~ 13 ALL OFF or ALL ON depending on the command
void allCommand(YunClient client){

       String nextCommand = client.readStringUntil('/');
       if(nextCommand == "off"){
           digitalWrite(13, LOW);
           digitalWrite(12, LOW);
           digitalWrite(11, LOW); 
           client.print(F("All Pins OFF"));
       }
       if(nextCommand == "on"){
           digitalWrite(13, HIGH);
           digitalWrite(12, HIGH);
           digitalWrite(11, HIGH); 
           client.print(F("All Pins ON"));
       }
  
}

// loop through and Blink from the given pin "pin" , "number" number of times where period = the delay time
void loopCommand(YunClient client) {
  int pin, period, number;
  
  //Read pin number
  pin = client.parseInt();
  
  if(client.read() == '/') {
    period = client.parseInt();
    if(client.read() == '/'){
      number = client.parseInt(); 
    }
  }
  
  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" blinking "));
  client.print(number);
  client.print(F(" times with delay of "));
  client.println(period);
  
  blinkLoop(pin, period, number);
  
  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(LOW));

}


// Blink from the given pin "pin" , "number" number of times where period = the delay time
void blinkLoop(int pin, int period, int number){
  
  for(int i = 0; i < number; i++){
       digitalWrite(pin, HIGH);
       delay(period);
       digitalWrite(pin, LOW);
       delay(period);
  }
}

// // Blink jn order of pin1, pin2, and pin3 , "number" number of times where period = the delay time
void alterLoop(int pin1, int pin2, int pin3, int period, int number){
  
  for(int i = 0; i < number; i++){
       digitalWrite(pin1, HIGH);
       delay(period);
       digitalWrite(pin1, LOW);
       delay(period);
       digitalWrite(pin2, HIGH);
       delay(period);
       digitalWrite(pin2, LOW);
       delay(period);
       digitalWrite(pin3, HIGH);
       delay(period);
       digitalWrite(pin3, LOW);
       delay(period);  
  }

}

// Loop through and Blink jn order of pin1, pin2, and pin3 , "number" number of times where period = the delay time
void alterCommand(YunClient client){
  int pin1, pin2, pin3, period, number;
  
  //Read pin numbers
  pin1 = client.parseInt();
  pin2 = client.parseInt();
  pin3 = client.parseInt();
  
  
  if(client.read() == '/') {
    period = client.parseInt();
    if(client.read() == '/'){
      number = client.parseInt(); 
    }
  }
  
  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin1);  
  client.print(F(", Pin D"));
  client.print(pin2);
  client.print(F(", Pin D"));
  client.print(pin3);
  client.print(F(" blinking "));
  client.print(number);
  client.print(F(" times with delay of "));
  client.println(period);
  
  alterLoop(pin1, pin2, pin3, period, number);
  
  String key1 = "D";
  key1 += pin1;
  Bridge.put(key1, String(LOW));
  String key2 = "D";
  key2 += pin2;
  Bridge.put(key2, String(LOW));
  String key3 = "D";
  key3 += pin3;
  Bridge.put(key3, String(LOW));  
}

// with given "pin" number, set it equal to "value"
void digitalCommand(YunClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  }
  else {
    value = digitalRead(pin);
  }

  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

// with given "pin" number, read the current value of the "pin"
void analogCommand(YunClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/analog/5/120"
  if (client.read() == '/') {
    // Read value and execute command
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    // Read analog pin
    value = analogRead(pin);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {
  int pin;

  // Read pin number
  pin = client.parseInt();

  // If the next character is not a '/' we have a malformed URL
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}

