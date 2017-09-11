#ifndef SIM800L_H
#define SIM800L_H
#include <Arduino.h>
#include <SoftwareSerial.h>
#include"sim800lHelpers.h"

class Sim800l
{
public:
    Sim800l(SoftwareSerial *serialToSim800l);
    Sim800l(){}
    
    virtual ~Sim800l();

    bool sendSms(const String &phoneNumber, const String &message);
    int readAt();
    int signalQuality();
    void readResponse();
    
    bool setPhoneFunctionality(phoneFunctionality funcionality=full);
    void getLocationApplication();
    bool callNumber(const String &phoneNumber);
    void endTask();
   
    void configureGPRS();
    void productInformation();
    bool isSimReady();
    int getCallStatus();
    void listSMSes();
private:
    String readSerial();
    void debug(const String& info);
    bool sendCommand(const String& command,echoValidate validation=validate);
    bool getStatus();

    SoftwareSerial* serial;
};

#endif
