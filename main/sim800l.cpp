#include "sim800l.h"
#include "Arduino.h"

Sim800l::Sim800l(SoftwareSerial *serialToSim800l)
{
  serial = serialToSim800l;
  serial->begin(9600);
  if (getStatus())
    Serial.println("Sim is ready");
  else
    Serial.println("Something went wrong");

}
Sim800l::~Sim800l()
{
  delete serial;
}
int Sim800l::sendSms(const String &phoneNumber, const String &message)
{
  sendCommand("AT+CMGF=1");
  String setNumber = "AT+CMGS=\"+48" + phoneNumber + "\"";
  sendCommand(setNumber);
  sendCommand(message);
  endTask();
  String resp = "SMS resp: ";
  resp += readSerial();
  debug(resp.c_str());

  return 0;
}
bool Sim800l::getStatus()
{

  if (sendCommand("at"))
  {
    String resp = readSerial();
    //debug(resp.c_str());
    if ( (resp.indexOf("OK") ) != -1)
    {
      sendCommand("ATE1");
      resp = readSerial();
      debug(resp);
      return true;
    }
    return false;

  }
}
void Sim800l::signalQuality() {
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
  sendCommand("AT+CSQ");
  //serial->print (F("AT+CSQ\r\n"));
  String readed = readSerial();
  debug(readed);
  //Serial.println(readSerial());
}

void Sim800l::callNumber(const String &phoneNumber)
{
  String setNumber = "ATD" + phoneNumber+";";
  //sendCommand("ATD");

  //snprintf(setNumber, 35, "+48%s\r\n", number);
  sendCommand(setNumber);
  String resp = readSerial();
  debug(resp.c_str());
  if (resp == "OK") {
    delay(3000);
  }

}
void Sim800l::configureGPRS()
{
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
   readResponse();



    sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
     readResponse();


      sendCommand("AT+SAPBR=3,1,\"USER\",\"internet\"");
       readResponse();


        sendCommand("AT+SAPBR=3,1,\"PWD\",\"internet\"");
          readResponse();


          sendCommand("AT+SAPBR=2,1");
            readResponse();


            sendCommand("AT+FTPCID=1");
              readResponse();
            
  


}
void Sim800l::getLocationApplication()
{
  sendCommand("AT+CIPGSMLOC=1,1");
  readResponse();
  //String resp = readSerial();
  //debug(resp.c_str());

}
void Sim800l::productInformation()
{
  sendCommand("ATI");
  readResponse();
}
void Sim800l::readResponse()
{
  while (serial->available())
    Serial.println(serial->read());
}

String Sim800l::readSerial()
{
  //debug("readSerial");
  int timeout = 0;
  String ret = "";
  char readed;
  while  (!serial->available() && timeout < 100  )
  {
    delay(10);
    timeout++;
  }
  while (serial->available()) {
    readed = serial->read();
    if (readed == (char)26 || readed == (char)13)
      break;
    if (readed > (char)31)
    {
      ret += readed;
    }
  }
  if (ret.length() == 0)
    return readSerial();
  return ret;


}
bool Sim800l::sendCommand(const String & command)
{
  String string = "";
  string = command + "\r\n";
  //debug("sendCommand " + command);
  serial->write(string.c_str());
  delay(10);
  string = readSerial();
  //debug(string);
  if (string == command)
  {
    debug("sendCommand SUCCESED: " + command);
    return true;
  }
  debug("sendCommand FAILED: " + command);
  return false;


}
void Sim800l::endTask()
{
  serial->write((char)26);
  String resp = readSerial();
  debug(resp.c_str());
  Serial.println("done");
}
void Sim800l::debug(const String & info)
{
  size_t n = info.length() + 8;
  char message[n];
  snprintf(message, n, "DEBUG: %s", info.c_str());
  Serial.println(message);
}
void Sim800l::setPhoneFunctionality() {
  /*AT+CFUN=<fun>[,<rst>]
    Parameters
    <fun> 0 Minimum functionality
    1 Full functionality (Default)
    4 Disable phone both transmit and receive RF circuits.
    <rst> 1 Reset the MT before setting it to <fun> power level.
  */
  sendCommand("AT+CFUN=1");
  String resp = readSerial();
  debug(resp.c_str());
  //SIM.print (F("AT+CFUN=1\r\n"));
}
//int Sim800l::readAt()
//{
//  if(serial->available())
//  {
//    while(serial->available())
//        out=serial->read();
//  }
//}

