#include "sim800l.h"
#include "Arduino.h"
#include <pins_arduino.h>

Sim800l::Sim800l(SoftwareSerial *serialToSim800l)
{
    serial = serialToSim800l;
    serial->begin(9600);
    if (getStatus())
    {
      while(!isSimReady())
      {
        delay(140);
      }
      return;
    }
    else
        Serial.println("Something went wrong");

}
Sim800l::~Sim800l()
{
    delete serial;
}

//TODO NEED TO BE VALIDATED
bool Sim800l::sendSms(const String &phoneNumber, const String &message)
{
    sendCommand("AT+CMGF=1");
    String resp = readSerial();
    if(resp.indexOf("OK") != -1)
    {
         String setNumber = "AT+CMGS=\"+48" + phoneNumber + "\"";
         sendCommand(setNumber,nonValidation);
         sendCommand(message,nonValidation);
         endTask();
         resp = readSerial();
         return true;   
    }
    return false;
}


//TODO NEED TO BE WERYFIED
int Sim800l::getCallStatus()
{
    /*
     values of return:
     
     0 Ready (MT allows commands from TA/TE)
     2 Unknown (MT is not guaranteed to respond to tructions)
     3 Ringing (MT is ready for commands from TA/TE, but the ringer is active)
     4 Call in progress
     */
    sendCommand("AT+CPAS");
    String readed = readSerial();
    debug(readed);
    return 0; //readed.substring(readed.indexOf("+CPAS: ")+7,_buffer.indexOf("+CPAS: ")+9).toInt();

}

/***********************
 * TODO
 * "REC UNREAD" Received unread messages
 * "REC READ" Received read messages
 * "STO UNSENT" Stored unsent messages
 * "STO SENT" Stored sent messages
 * "ALL" All messages

 * 0 Normal
 * 1 Not change status of the specified SMS record
 **********************/
void Sim800l::listSMSes()
{
    sendCommand("AT+CMGF=1");
    String resp = readSerial();
    sendCommand("AT+CMGL");
    resp = readSerial();

    if (resp.indexOf("OK") != -1)
    {
        sendCommand("AR+CMGL=ALL,0");
        String resp = readSerial();
        debug(resp);
    }
}


/*Response
     +CSQ: <rssi>,<ber>Parameters
     <rssi>
     0 -115 dBm or less
     1 -111 dBm
     2...30 -110... -54 dBm
     31 -52 dBm or greater
     99 not known or not detectable
     <ber> (in percent):
     0...7 As RXQUAL values in the table in GSM 05.08 [20]
     subclause 7.2.4
     99 Not known or not detectable
     */
//TODO RETURNED VALUE && VALIDATE
void Sim800l::signalQuality()
{
    sendCommand("AT+CSQ");
    String readed = readSerial();
    debug(readed);
    //Serial.println(readSerial());
}


//TODO RETURNED VALUE 
bool Sim800l::callNumber(const String &phoneNumber)
{
    String setNumber = "ATD" + phoneNumber + ";";
    //sendCommand("ATD");

    debug("Calling on " + phoneNumber);
    sendCommand(setNumber);
    String resp = readSerial();
    if (resp == "OK")
    {
        delay(3000);
        return true;
    }
    debug(resp);
    return false;
}

//TODO RETURNED VALUE && VALIDATE
void Sim800l::configureGPRS()
{
    sendCommand("AT+CSTT=\"internet\",\"\",\"\"");
    String resp = readSerial();
    //  if(resp == "OK")
    //  {
    sendCommand("AT+CIICR");
    resp = readSerial();
    //    if(resp == "OK")
    //    {
    sendCommand("AT+CIFSR");
    resp = readSerial();
    debug(resp);
    //    }

    //  }

    //sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    //   readResponse();
    //
    //
    //
    //    sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
    //     readResponse();
    //
    //
    //      sendCommand("AT+SAPBR=3,1,\"USER\",\"internet\"");
    //       readResponse();
    //
    //
    //        sendCommand("AT+SAPBR=3,1,\"PWD\",\"internet\"");
    //          readResponse();
    //
    //
    //          sendCommand("AT+SAPBR=2,1");
    //            readResponse();
    //
    //
    //            sendCommand("AT+FTPCID=1");
    //              readResponse();
    //
    //

}

//VERYFY
//TODO RETURNED VALUE && VALIDATE

void Sim800l::getLocationApplication()
{
    sendCommand("AT+CIPGSMLOC=1,1");
    readResponse();
}
//TODO VERYFY
void Sim800l::productInformation()
{
    sendCommand("ATI");
    readResponse();
}

/*************************************
*SEND STOP SEQUENCE                  *
*NEED TO READ AND VALIDATE RESPONSE  *
*************************************/
void Sim800l::endTask()
{
    String msg="";
    msg.setCharAt(0, (char)26); 
    sendCommand(msg,nonValidation); 
}

//TODO VALIDATE
/**************************************************************
*setPhoneFunctionality                                        *
*Parameters                                                   *
*<fun> 0 Minimum functionality                                *
*  1 Full functionality (Default)                             *
*  4 Disable phone both transmit and receive RF circuits.     *
*<rst> 1 Reset the MT before setting it to <fun> power level  *
***************************************************************/
void Sim800l::setPhoneFunctionality()
{  
    if(sendCommand("AT+CFUN=1"))
    {
      String resp = readSerial();
      if(resp.indexOf("OK") != -1)
      {
        return true;
      }
      debug(resp.c_str());
    }
   return false;
}

bool Sim800l::isSimReady()
{
    if (sendCommand("AT+CPIN?"))
    {
        String resp = readSerial();
        debug(resp);
        return (resp == "OK");
    }
    return false;
}

bool Sim800l::getStatus()
{

    if (sendCommand("at"))
    {
        String resp = readSerial();
        debug(resp.c_str());
        //if(resp.indexOf("OK") != -1)
       // {
            //set echo mode
            sendCommand("ATE1");
            resp = readSerial();
            debug(resp);
            if(resp.indexOf("OK") != -1)
            {
              return true;
            }
       // }
    }
     return false;
}

void Sim800l::readResponse()
{
    while (serial->available())
        Serial.println(serial->read());
}

String Sim800l::readSerial()
{
    int timeout = 0;
    String ret = "";
    char readed;
    while (!serial->available() && timeout < 100)
    {
        delay(10);
        timeout++;
    }
    while (serial->available())
    {
        readed = serial->read();
        if (readed == (char) 26 || readed == (char) 13)
        {
            break;
        }
        if (readed > (char) 31)
        {
            ret += readed;
        }
    }
    if (ret.length() == 0)
    {
        return readSerial();
    }
    return ret;
}


bool Sim800l::sendCommand(const String & command ,echoValidate validation)
{
    String string = "";
    string = command + "\r\n";
    serial->write(string.c_str());
    delay(10);
    string = readSerial();
    debug(string);
     if (string == command || validation)
    {
        debug("sendCommand SUCCESED: " + command);
        return true;
    }
    debug("sendCommand FAILED: " + command);
    return false;
}


void Sim800l::debug(const String & info)
{
    size_t n = info.length() + 8;
    char message[n];
    snprintf(message, n, "DEBUG: %s", info.c_str());
    Serial.println(message);
}
