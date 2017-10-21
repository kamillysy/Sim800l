#ifndef SIM800LHELPERS_H
#define SIM800LHELPERS_H
#include <Arduino.h>

#define  nonValidation false
#define validate true

/**************************************************************
 *setPhoneFunctionality                                        *
 *Parameters                                                   *
 *<fun> 0 Minimum functionality                                *
 *  1 Full functionality (Default)                             *
 *  4 Disable phone both transmit and receive RF circuits.     *
 *<rst> 1 Reset the MT before setting it to <fun> power level  *
 ***************************************************************/
enum phoneFunctionality{
    minimum=0,
    full=1,
    disable=4
};
struct Time{
      unsigned int sec;
      unsigned int min;
      unsigned int hour;
      unsigned int day;
      unsigned int mon;
      unsigned int year;
    };
     struct Geographical{
      int val;
      unsigned int hundredth;
      String toString(){
        return val+"."+hundredth;
      }
     };
    
      struct Possiton{
      Geographical length; 
      Geographical altitude;
      String toString(){
        return length.toString()+altitude.toString();
      }
    };
   
String phoneFunctionalToString(phoneFunctionality functionality);


#endif
