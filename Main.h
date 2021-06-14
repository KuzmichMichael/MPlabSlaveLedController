/* 
 * File:   Main.h
 * Author: xatini
 *
 * Created on 4 мая 2021 г., 23:55
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
    
#define oneSecondDelay 25
#define resetButt RA0
    
enum ModeCodes{
    FIRST_MODE  = 0x01,
    SECOND_MODE = 0x02,
    THIRD_MODE  = 0x03,
    FOURTH_MODE = 0x04,
    FIFTH_MODE  = 0x05,
    SIXTH_MODE  = 0x06
}; 

/*enum ModeCodes{
    FIRST_MODE  = 0x80,
    SECOND_MODE = 0xC0,
    THIRD_MODE  = 0x81,
    FOURTH_MODE = 0xE0,
    FIFTH_MODE  = 0x82,
    SIXTH_MODE  = 0xC1
};*/ 

enum curModes{
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    FIFTH,
    SIXTH
};    
enum curModes LEDcurrentMode = FIRST;

unsigned char delayBuffer = 0;
unsigned char globalCounter = 0;   

unsigned long resetButtDelay = 0;
unsigned char framingErrorData = 0;

void initHW();  
void processModes();
void myShortDelay();
void myLongDelay();
#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

