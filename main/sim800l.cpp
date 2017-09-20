
#include "Arduino.h"
#include <pins_arduino.h>
#include "sim800l.h"
Sim800l::Sim800l(SoftwareSerial *serialToSim800l, const int &rstPin)
{
    RstPin = rstPin;
    serial = serialToSim800l;
    serial->begin(9600);
    String resp="";
    bool moduleReady=false, simStatus=false, simConnected = false;
    delay(300);
    isModuleStarted();
    

    for(unsigned counter=0;counter<10;counter++)
    {
        if(!simStatus)
        {
            simStatus = getStatus();
        }
        else if(!moduleReady)
        {
            moduleReady = isModuleReady();
        }
        if(moduleReady && simStatus)
        {
            simConnected = waitForSimConnected(1000);
            break;
        }
        delay(23);
    }
    if(!moduleReady || !simStatus || !simConnected)
    {
        Serial.println("Something went wrong");
    }

}
Sim800l::~Sim800l()
{
    delete serial;
}
bool Sim800l::waitForSimConnected(const unsigned int timeout)
{
  delay(300);
  unsigned int del;
  for(  del = 0; del<timeout; del+=13)
  {
    if(isSimReady())
    {
      return true;
    }
    else{
      delay(13);   
    }
      
  }
  return false;
   
}
//TODO NEED TO BE VALIDATED
bool Sim800l::sendSms(const String &phoneNumber, const String &message)
{
    if(sendCommand("AT+CMGF=1"))
    {
      if(validateResponse("OK"))
      {
        String setNumber = "AT+CMGS=\"+48" + phoneNumber + "\"";
        sendCommand(setNumber,nonValidation);
        sendCommand(message,nonValidation);
        endTask();
        String resp = readSerial();
        validateResponse("OK");
        return true;
      }
    }
    

   

    return false;
}


//TODO NEED TO BE WERYFIED
int Sim800l::getCallStatus()
{
    /*
     values of return:

     0 Ready (MT allows commands from TA/TE)
     2 Unknown (MT is not guaranteed to respond to functions)
     3 Ringing (MT is ready for commands from TA/TE, but the ringer is active)
     4 Call in progress
     */
    sendCommand("AT+CPAS");
    String readed = readSerial();
    // debug(readed.substring(readed.indexOf("+CPAS: ")+7,readed.indexOf("+CPAS: ")+9));
    return readed.substring(readed.indexOf("+CPAS: ")+7,readed.indexOf("+CPAS: ")+9).toInt();

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
//TODO veryfy what's exactly that mean?
void Sim800l::listSMSes()
{
    sendCommand("AT+CMGF=1");
    String resp = readSerial();
    sendCommand("AT+CMGL");
    resp = readSerial();

    if (validateResponse("OK"))
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

//TODO RETURNED VALUE is an level , subclause write class/enum wich decode it
//&& VALIDATE
float Sim800l::signalQuality()
{
    sendCommand("AT+CSQ");
    String readed = readSerial();
    debug(readed);
    int pos = readed.indexOf(',');
    readed[pos]='.';
    float ret;
    readed.substring(readed.indexOf("+CSQ: ")+6,readed.indexOf("+CPAS: ")+11);//.toFloat();
    //readed(ret);
    debug(readed);
    return ret;
    //Serial.println(readSerial());
}


//TODO RETURNED VALUE 
bool Sim800l::callNumber(const String &phoneNumber)
{
    String setNumber = "ATD" + phoneNumber + ";";
    //sendCommand("ATD");

    debug("Calling on " + phoneNumber);
    sendCommand(setNumber);
    if (validateResponse("OK"))
    {
        delay(3000);
        return true;
    }
    return false;
}
void Sim800l::ping(const String &host)
{
  configureInternet();
  String command = "AT+CIPPING=\""+ host + "\",\"4\",\"32\",\"100\",\"64\",\"12\",\"\",\"\"";
  sendCommand(command);
  delay(300);
  
  
  debug(readSerial());
  debug(readSerial());
  debug(readSerial());
  debug(readSerial());
  validateResponse("OK");
  cloaseBearer();
}

//TODO RETURNED VALUE && VALIDATE
void Sim800l::configureInternet()
{
    sendCommand("AT+CSTT=\"internet\",\"\",\"\"");
    validateResponse("OK");
    sendCommand("AT+SAPBR=3,1,\"PHONENUM\"\",\"*99#\"");
    debug(readSerial());
    sendCommand("AT+CIICR");//Start wireless connection with the GPRS. 
    validateResponse("OK");
    sendCommand("AT+CIFSR");//Gets the IP address assigned to the module
      debug(readSerial());
       
        
    
    

   

    //
    //
    //
        

    //
    //


          
    //
    //
            
    //
    //
//              sendCommand("AT+SAPBR=2,1");
//                readResponse();
    //
    //
//                sendCommand("AT+FTPCID=1");
//                  readResponse();
//    //
    //

}


//VERYFY
//TODO RETURNED VALUE && VALIDATE

/*****
 AT+SAPBR=3,1,"Con
 OK
type","GPRS"
AT+SAPBR=3,1,"AP
 OK
N","CMWAP"
AT+SAPBR=1,1
 OK
AT+SAPBR=2,1
 +SAPBR: 1,1,"10.89.193.1"
OK
*////////
void Sim800l::getLocationApplication()
{
    sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    if(validateResponse("OK"))
    {
       sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
       if(validateResponse("OK"))
       {
           sendCommand("AT+SAPBR =1,1");
           validateResponse("OK");
         
           
           sendCommand("AT+CIPGSMLOC=1,1");
            debug(readSerial());
            validateResponse("OK");
            delay(250);
           cloaseBearer();
        }
        else
        {
        debug(readSerial());
        }
    }
    else
    {
       debug(readSerial());
    }
    //readResponse();
}
void Sim800l::cloaseBearer(){
  sendCommand("AT+SAPBR=0,1");
  validateResponse("OK");
}

void Sim800l::productInformation()
{
    sendCommand("ATI");
    String resp = readSerial();
    debug("product information: " + resp);
}

/*************************************
 *SEND STOP SEQUENCE                  *
 *NEED TO READ AND VALIDATE RESPONSE  *
 *************************************/
void Sim800l::endTask()
{
  String msg="";
  serial->print((char)26);
  readSerial();
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
bool Sim800l::setPhoneFunctionality(phoneFunctionality funcionality)
{  
    String command="AT+CFUN="+phoneFunctionalToString(funcionality);

    if(sendCommand(command))
    {
        return validateResponse("OK");
    }
    return false;
}

bool Sim800l::isModuleReady()
{
    if (sendCommand("AT+CPIN?"))
    {
      if(validateResponse("+CPIN: READY"))
      {
        return validateResponse("OK");
      }
        
    }
    return false;
}

bool Sim800l::isSimReady()
{
  bool stat= false;
  sendCommand("AT+CPAS", nonValidation);
  delay(80);
  stat= validateResponse("+CPAS: 0");
  delay(30);
  validateResponse("OK");
  return stat;
  
}
void Sim800l::restartMoudule()
{
  debug("restartMoudule");
   digitalWrite(RstPin,HIGH);
   delay(1000);
   digitalWrite(RstPin,LOW);
   delay(1000);
}

bool Sim800l::getStatus()
{

    if (sendCommand("at"))
    {
        if(validateResponse("OK"))
        {
            //set echo mode
            sendCommand("ATE1");
            if(validateResponse("OK"))
            {
                return true;
            }
        }

    }
    return false;
}

void Sim800l::readResponse()
{
    while (serial->available())
        Serial.println(serial->read());
}



bool Sim800l::sendCommand(const String & command ,bool validation)
{
    String string = "";
    string = command + "\r\n";
    serial->write(string.c_str());
    delay(120);
    string = readSerial();
    debug(string);
    if (string == command || validation==false)
    {
        debug("sendCommand SUCCESED: " + command);
        return true;
    }
    debug("sendCommand FAILED: " + command + "received: " + string);
    return false;
}


void Sim800l::debug(const String & info)
{
    size_t n = info.length() + 8;
    char message[n];
    snprintf(message, n, "DEBUG: %s", info.c_str());
    Serial.println(message);
}
bool Sim800l::isModuleStarted()
{
  if(sendCommand("at"))
  {
     validateResponse("OK");
     String resp = readSerial();
     if(resp =="+CPIN: READY" || resp =="Call Ready" ||resp =="SMS Ready")
     {
      debug(resp + "OK");
      resp = readSerial();
      if(resp =="+CPIN: READY" || resp =="Call Ready" ||resp =="SMS Ready")
      {
       debug(resp + "OK");
       resp = readSerial();
       if(resp =="+CPIN: READY" || resp =="Call Ready" ||resp =="SMS Ready")
       { 
        debug(resp + "OK");
       }
     }
    }
  }
 
}

bool Sim800l::validateResponse(const String &expectedResp)
{
      
     String resp = readSerial();
     if(resp == expectedResp)
     {
        return true;
     }
     debug("Expected:"+expectedResp + "Received: " + resp);
     return false;
}

String Sim800l::readSerial()
{
    unsigned int timeout = 0x2FF;
    String ret = "";
    char readed;
    while (!serial->available() && timeout)
    {
        delay(10);
        timeout--;
    }
    for (;serial->available();delay(10))
    {
        readed = serial->read();
        if (readed == (char) 26  || readed == (char) 13 || readed == (char) 18 || readed == (char) 19 || readed == (char) 10)
        {
            break;
        }
        if ((int)readed >  31)
        {
            ret += readed;
        }
    }
    if (ret.length() == 0 && timeout)
    {
        return readSerial();
    }
    return ret;
}


String phoneFunctionalToString(phoneFunctionality functionality)
{
    String ret="";
    switch(functionality)
    {
    case minimum:
        ret="0";
        break;
    case full:
        ret="1";
        break;
    case disable:
        ret="4";
        break;
    }
    return ret;
}
