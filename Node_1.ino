
#include <XBee.h>
#include <SoftwareSerial.h>

uint8_t ssRX = 8;

uint8_t ssTX = 9;

SoftwareSerial nss(ssRX, ssTX);

XBee xbee = XBee();

ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();

XBeeAddress64 test = XBeeAddress64();

void setup() { 
  Serial.begin(9600);
  xbee.setSerial(Serial);
  // start soft serial
  nss.begin(9600);
}

void loop() {
  
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    

    if (xbee.getResponse().getApiId() == ZB_IO_SAMPLE_RESPONSE) {
      xbee.getResponse().getZBRxIoSampleResponse(ioSample);

      nss.print("Received I/O Sample from: ");
      
      nss.print(ioSample.getRemoteAddress64().getMsb(), HEX);  
      nss.print(ioSample.getRemoteAddress64().getLsb(), HEX);  
      nss.println("");
      
      if (ioSample.containsAnalog()) {
        nss.println("Sample contains analog data");
      }

      if (ioSample.containsDigital()) {
        nss.println("Sample contains digtal data");
      }      

      // read analog inputs
      for (int i = 0; i <= 4; i++) {
        if (ioSample.isAnalogEnabled(i)) {
          nss.print("Analog (AI");
          nss.print(i, DEC);
          nss.print(") is ");
          nss.println(ioSample.getAnalog(i), DEC);
        }
      }

      // check digital inputs
      for (int i = 0; i <= 12; i++) {
        if (ioSample.isDigitalEnabled(i)) {
          nss.print("Digital (DI");
          nss.print(i, DEC);
          nss.print(") is ");
          nss.println(ioSample.isDigitalOn(i), DEC);
        }
      }
      

    } 
    else {
      nss.print("Expected I/O Sample, but got ");
      nss.print(xbee.getResponse().getApiId(), HEX);
    }    
  } else if (xbee.getResponse().isError()) {
    nss.print("Error reading packet.  Error code: ");  
    nss.println(xbee.getResponse().getErrorCode());
  }
}
