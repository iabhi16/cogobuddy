#define USE_ARDUINO_INTERRUPTS true  
#include <PulseSensorPlayground.h>   
const int PulseWire =analogRead(0); 
int Threshold = 550;
PulseSensorPlayground pulseSensor;                    
String quality ="";
int buttonState = 0;
#include <XBee.h>
#include <SoftwareSerial.h>  
XBee xbee = XBee();
unsigned long start = millis();
uint8_t payload[] = { 0, 0 };
Tx16Request tx = Tx16Request(0x1874, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();
int pin5 = 0;
int statusLed = 11;
int errorLed = 12;
void flashLed(int pin, int times, int wait) {  
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      if (i + 1 < times) {
        delay(wait);
      }
    }
}
void setup()
{
  Serial.begin(9600);
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   
   if (pulseSensor.begin()) {
    Serial.println("Pulse Detected!"); 
   pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  Serial.begin(9600);
  xbee.setSerial(Serial);
    delay(5000);
  }
}


void loop() {
int myBPM = pulseSensor.getBeatsPerMinute(); 
if (pulseSensor.sawStartOfBeat()) {           
 Serial.println("Pulse Rate:"); 
 Serial.print(myBPM); 
 Serial.println("BPM");                   
}

  delay(20);                  
  int mq135,methanelevel,health,gaslevel;
  gaslevel = analogRead(0);
  methanelevel = analogRead(2);
  Serial.print("Air Quality Level:");
  if(gaslevel <380){
    quality = "Good";
  }
  else if (gaslevel >380 && gaslevel<490){
    quality = "Moderate";
  }
  else if (gaslevel >490 && gaslevel<600){
    quality = "Poor";
  }
  else if (gaslevel >600){
    quality = "Very Poor";
  }
Serial.println(quality);
delay(1000);
Serial.print("Methan:");
if (methanelevel<450){
  Serial.print("Methane not Present\n");
}
    else
    {
      Serial.print("Methane Present\n");
    }
   delay(2000);
    buttonState = digitalRead(1); 
    if (buttonState == 1)
    {      
    Serial.println("Emergency"); 
    } 
   Serial.print("\n");
   if (millis() - start > 15000) {
      pin5 = analogRead(5);
      payload[0] = pin5 >> 8 & 0xff;
      payload[1] = pin5 & 0xff;
      xbee.send(tx);
      flashLed(statusLed, 1, 100);
    }
    if (xbee.readPacket(5000)) {             
      if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
         xbee.getResponse().getTxStatusResponse(txStatus);
           if (txStatus.getStatus() == SUCCESS) {
              flashLed(statusLed, 5, 50);
           } else {
              flashLed(errorLed, 3, 500);
           }
        }      
    } else if (xbee.getResponse().isError()) {
    } else {
      flashLed(errorLed, 2, 50);
    }
    
    delay(1000);
}


