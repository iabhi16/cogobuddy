#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "coalmine-e9ae6.firebaseio.com"
#define FIREBASE_AUTH "BKJFBti484rARdtXwbX4YpHNd3aCi6FDLTU25STf"
#define WIFI_SSID "sdiot@abhi"
#define WIFI_PASSWORD "sdiot123"
#include <XBee.h>
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();
int statusLed = 11;
int errorLed = 12;
int dataLed = 10;
uint8_t option = 0;
uint8_t data = 0;
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
void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
int n = 0;

 pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(dataLed,  OUTPUT);
  Serial.begin(9600);
  xbee.setSerial(Serial);
  flashLed(statusLed, 3, 50);

void loop() 
int value,rx;
value=Serial.receive(rx); 
  Firebase.setFloat("number", value);
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
 
  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("value");
  delay(1000);

  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
  xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
        
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
                xbee.getResponse().getRx16Response(rx16);
          option = rx16.getOption();
          data = rx16.getData(0);
        } else {
                xbee.getResponse().getRx64Response(rx64);
          option = rx64.getOption();
          data = rx64.getData(0);
        }  
        flashLed(statusLed, 1, 10);
        analogWrite(dataLed, data);
      } else {
        flashLed(errorLed, 1, 25);    
      }
    } else if (xbee.getResponse().isError()) {
      nss.print("Error reading packet.  Error code: ");  
      nss.println(xbee.getResponse().getErrorCode());
    } 
}
