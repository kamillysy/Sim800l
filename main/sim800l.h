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

    int sendSms(const String &phoneNumber, const String &message);
    int readAt();
    void signalQuality();
    void readResponse();
    bool getStatus();
    void setPhoneFunctionality();
    void getLocationApplication();
    void callNumber(const String &phoneNumber);
    void endTask();
    bool sendCommand(const String& command);
    void configureGPRS();
    void productInformation();
    bool isSimReady();
    int getCallStatus();
    void listSMSes();
private:
    String readSerial();
    void debug(const String& info);

    SoftwareSerial* serial;
};

#endif
