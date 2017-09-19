#ifndef SIM800L_H
#define SIM800L_H
#include <Arduino.h>
#include <SoftwareSerial.h>
#include"sim800lHelpers.h"

class Sim800l
{
public:
    Sim800l(SoftwareSerial *serialToSim800l, const int &rstPin );
    Sim800l(){}

    virtual ~Sim800l();

    bool sendSms(const String &phoneNumber, const String &message);
    int readAt();
    float signalQuality();
    void readResponse();

    bool setPhoneFunctionality(phoneFunctionality funcionality=full);
    void getLocationApplication();
    bool callNumber(const String &phoneNumber);
    void endTask();

    void configureInternet();
    void productInformation();
    bool isModuleReady();
    int getCallStatus();
    void listSMSes();
    void ping(const String &host);
    bool waitForSimConnected(const unsigned int timeout);
    void restartMoudule();
private:
    void cloaseBearer();
    String readSerial();
    void debug(const String& info);
    bool sendCommand(const String& command,bool validation = validate);
    bool getStatus();
    bool validateResponse(const String &expectedResp);
    bool isSimReady();
    bool isModuleStarted();
    
    
    int RstPin;

    SoftwareSerial* serial;
};

#endif
