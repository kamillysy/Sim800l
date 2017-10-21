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
    String getLocationApplication();
    bool callNumber(const String &phoneNumber);
    void endTask();
    bool sendEmail(const String& passWord,const String& Subject,const String& Body);
    

    void configureInternet();
    void productInformation();
    bool isModuleReady();
    int getCallStatus();
    void listSMSes();
    void ping(const String &host);
    bool waitForSimConnected(const unsigned int timeout);
    void restartMoudule();
    String getNetworkTimeGMT();
    void setTime(const String &time);
    void setNetworkTime();
    
//   / void sendEmail();
private:
    bool openBearer();
    bool cloaseBearer();
    String readSerial(unsigned int timeout = 0x2FF);
    void debug(const String& info);
    bool sendCommand(const String& command,bool validation = validate);
    bool getStatus();
    bool validateResponse(const String &expectedResp,unsigned int timeout = 0x2FF);
    bool isSimReady();
    bool isModuleStarted();
    
    
    int RstPin;

    SoftwareSerial* serial;
    
    
};

#endif
