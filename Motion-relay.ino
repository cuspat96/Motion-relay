//Turns relay on after detecting movement, or button press.

#define ledPin 13
#define pirPin 2
#define lightSensorPin A0
#define relayPin 4
#define buttonPin 6
bool manuallySwitched = false;
bool relayState = false;
bool ledState = false;
bool previousPirState = false;
bool currentPirState = false;
bool setRelayTo = false;
int unsigned long lastDetection = 0;
int unsigned long startTime = millis();
int lightLevel;
//You can change this variables
int unsigned long delayInSeconds = 60;
int unsigned long pirSetupTime   = 4;
int lightThreshold = 15;
///////////////////////////////
//functions
///////////////////////////////
void printSerial(String a){
  Serial.print(millis()/1000);
  Serial.print(" s. ");
  Serial.println(a);
}
////////SETUP////////////
void pirSetup(){
  int setupTime = millis();
  printSerial("Initiating motion sensor");
  while(millis() < setupTime+pirSetupTime*1000){
    Serial.print(".");
    ledOn();
    delay(500);
    ledOff();
    delay(500);
  }
  Serial.println();
  ledBlink();
  printSerial("Motion sensor ready!");
  
}
void pinsSetup(){
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(pirPin, INPUT);
}
////////TURN ON/OFF////////////
void ledOn(){
  digitalWrite(ledPin, HIGH);
}
void ledOff(){
  digitalWrite(ledPin, LOW);
}
void ledBlink() {
  for(int i=0; i<5; i++){
    ledOn();
    delay(100);
    ledOff();
    delay(100);
  }
}
void relayOn(){
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);
  relayState = true;
  printSerial("Relay ON.");
}
void relayOff(){
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  relayState = false;
  printSerial("Relay OFF.");
}
////////MOTION DETECTION////////
void getMotion(){
  if(digitalRead(pirPin) == HIGH){
    if(millis()>lastDetection+delayInSeconds*1000){
      String str = "Motion detected! Turning relay on for ";
      str += delayInSeconds;
      str += "s.";
      printSerial(str);
      lastDetection = millis();
      setRelayTo = true;
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//SETUP & LOOP
///////////////////////////////
void setup() {
  Serial.begin(9600);
  printSerial("Arduino turned ON");
  ledBlink();
  pinsSetup();
  pirSetup();
}

void loop() {
    lightLevel = analogRead(lightSensorPin);
    if(lastDetection == 0 && manuallySwitched == false){
       setRelayTo = true;
       lastDetection = millis();
    }else{
      if(lightLevel < lightThreshold){
        if(millis()>lastDetection+delayInSeconds*1000 && manuallySwitched == false)
          getMotion();
      }
    }
 
  if(setRelayTo != relayState){
    if(setRelayTo == true){
      relayOn();
    }else{
      relayOff();
    }
  }

  if(relayState == true){
    if(millis()>lastDetection+delayInSeconds*1000 && manuallySwitched == false)
      setRelayTo = false;
  }

  if(digitalRead(buttonPin) == HIGH){
    printSerial("button");
    if(relayState == false){
      setRelayTo = true;
      manuallySwitched = true;
    }else{
      setRelayTo = false;
      manuallySwitched = false;
    }
    while(digitalRead(buttonPin) == HIGH){}
    delay(10);
  }
}

