#include "sim800l.h"
#include <SoftwareSerial.h>
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 3
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 2
#include "sim800l.h"
Sim800l* sim=NULL;
SoftwareSerial serialToSim(SIM800_TX_PIN,SIM800_RX_PIN);

void setup() {
  Serial.begin(9600);
  sim=new Sim800l(&serialToSim);
  delay(300);
  sim->setPhoneFunctionality(full);
  sim->configureGPRS();
 // sim->getLocationApplication();
  //sim->productInformation();
  //String call(sim->getCallStatus());
  //String msg="call status = " + call;
  //Serial.println(msg.c_str());
  //sim->listSMSes();
//  sim->signalQuality();
}
 
void loop() {
  //Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  if(serialToSim.available()){
    Serial.write(serialToSim.read());
    
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){  
    serialToSim.write(Serial.read());
    
  }
}
