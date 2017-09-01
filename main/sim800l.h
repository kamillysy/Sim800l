#ifndef SIM800L_H
#define SIM800L_H
#include <Arduino.h>
#include <SoftwareSerial.h>

class Sim800l
{
public:
    Sim800l(SoftwareSerial *serialToSim800l);
    Sim800l()
    {
    }
    ;
    virtual ~Sim800l();

    bool sendSms(const String &phoneNumber, const String &message);
    int readAt();
    void signalQuality();
    void readResponse();
    
    void setPhoneFunctionality();
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
    bool sendCommandWithEchoValidation(const String& command);
    void sendCommand(const String& command);
    bool getStatus();

    SoftwareSerial* serial;
};

#endif
