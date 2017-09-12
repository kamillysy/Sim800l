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

String phoneFunctionalToString(phoneFunctionality functionality);


#endif
