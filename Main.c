/* 
 * File:   Main.c
 * Author: xatini
 *
 * Created on 4 мая 2021 г., 23:55
 */

#include "Main.h"

/*
 * 
 */
void interrupt isr(void)
{
    if (TMR1IF){
        //process and increment counter
        if (globalCounter >= oneSecondDelay){           
          globalCounter = 0;
        }
        globalCounter++;
        
        //processing reset butt
        if (resetButt){
            resetButtDelay--;
            if (resetButtDelay == 0){
            asm("reset");
            }
        }
        else{
            resetButtDelay = oneSecondDelay;
        }
        
        TMR1IF = 0;
    }
    
  if(RCIF){
    if(FERR) //framing error
    {
      framingErrorData = RCREG;
      return;
    }
    else if (OERR) //Overrun eroor
    {
      CREN = 0;      
      CREN = 1;
      return;
    }
    switch(RCREG){ //process recived bite
        case FIRST_MODE:{
            LEDcurrentMode = FIRST;
            break;
        } 
        case SECOND_MODE:{
            LEDcurrentMode = SECOND;
            break;
        } 
        case THIRD_MODE:{
            LEDcurrentMode = THIRD;
            break;
        } 
        case FOURTH_MODE:{
            LEDcurrentMode = FOURTH;
            break;
        } 
        case FIFTH_MODE:{
            LEDcurrentMode = FIFTH;
            break;
        } 
        case SIXTH_MODE:{
            LEDcurrentMode = SIXTH;
            break;
        }    
    }
    WriteUSART(RCREG);
    RCIF = 0;
  }  
}

int main(int argc, char** argv) {

    initHW();
    
    while (1){
    processModes();
    }
    
    return (EXIT_SUCCESS);
}

// My initHW() looks like this:
void initHW () {
    // configure the digital IO pins and int osc
    TRISA = 0b11110011;             // set Port A: all pins as outputs
    //A1 test output
    ADCON1 = 0xFF;
    PORTA = 0x00;             // set Port A: outputs to zero
    TRISB = 0b11010000;             // set Port B: all pins as outputs
    PORTB = 0x00;             // set Port B: outputs to zero
    OSCCON = 0x70;            // configure the internal oscillator (8MHz)
 
    // enable reading from data EEPROM
    EECON1 = 0x00;
 
    // configure timer 1
    T1CON = 0x85;             // enable tmr1 as a 16bit counter and use
                            // system clock as a source
    PIR1bits.TMR1IF = 0;      // clear the interrupt flag
    IPR1bits.TMR1IP = 0;      // set the propriety to low
    PIE1bits.TMR1IE = 1;      // enable the tmr1 interrupt
 
    // enable the EUSART
    RCSTAbits.SPEN = 1;       // Enable RX on the serial port
    ADCON1bits.PCFG5 = 1;     // configure pin RB1 as digital IO
    ADCON1bits.PCFG6 = 1;     // configure pin RB4 as digital IO
    TRISBbits.TRISB1 = 1;     // configure pin RB1 as input
    TRISBbits.TRISB4 = 1;     // configure pin RB4 as input
    IPR1bits.RCIP = 1;        // Make receive interrupt hi priority
 
    // configure the EUSART as 9600 baud, 8N1 in interrupt mode
    OpenUSART (USART_TX_INT_OFF &
               USART_RX_INT_ON &
               USART_ASYNCH_MODE &
               USART_EIGHT_BIT &
               USART_CONT_RX &
               USART_BRGH_HIGH, 51);
 
    // configure the baud rate generator
    baudUSART (BAUD_8_BIT_RATE &
               BAUD_AUTO_OFF &
               BAUD_WAKEUP_OFF);
    
    // enable interrupts
    INTCONbits.GIEH = 1;      // global enable high priority interrupts
    INTCONbits.GIEL = 1;      // global enable low priority interrupts
    RCONbits.IPEN = 1;        // enable interrupt priorities
    
    //init variables
    LEDcurrentMode = FIRST;
}

void processModes(){
    switch(LEDcurrentMode){
    case FIRST:{
        PORTAbits.RA2 = 1;
        myShortDelay();
        PORTAbits.RA2 = 0;
        
        PORTAbits.RA3 = 1;
        myShortDelay();
        PORTAbits.RA3 = 0;
        
        PORTBbits.RB5 = 1;
        myShortDelay();
        PORTBbits.RB5 = 0;
        
        PORTBbits.RB3 = 1;
        myShortDelay();
        PORTBbits.RB3 = 0;
        
        PORTBbits.RB2 = 1;
        myShortDelay();
        PORTBbits.RB2 = 0;
        
        PORTBbits.RB0 = 1;
        myShortDelay();
        PORTBbits.RB0 = 0;
        break;
    }
    case SECOND:{
        PORTAbits.RA2 = 1;      
        PORTAbits.RA3 = 1;    
        PORTBbits.RB5 = 1;
        PORTBbits.RB3 = 1;
        PORTBbits.RB2 = 1;
        PORTBbits.RB0 = 1;
        
        myShortDelay();
        
        PORTAbits.RA2 = 0;      
        PORTAbits.RA3 = 0;    
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 0;
        PORTBbits.RB2 = 0;
        PORTBbits.RB0 = 0; 
        
        myShortDelay();
        break;
    }
    case THIRD:{
        PORTAbits.RA2 = 1;      
        PORTAbits.RA3 = 0;    
        PORTBbits.RB5 = 1;
        PORTBbits.RB3 = 0;
        PORTBbits.RB2 = 1;
        PORTBbits.RB0 = 0;
        
        myShortDelay();
        
        PORTAbits.RA2 = 0;      
        PORTAbits.RA3 = 1;    
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 1;
        PORTBbits.RB2 = 0;
        PORTBbits.RB0 = 1; 
        
        myShortDelay();
        break;
    }
    case FOURTH:{
        PORTAbits.RA2 = 1;      
        PORTAbits.RA3 = 1;    
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 0;
        PORTBbits.RB2 = 1;
        PORTBbits.RB0 = 1;
        
        myLongDelay();
        
        PORTAbits.RA2 = 0;      
        PORTAbits.RA3 = 0;    
        PORTBbits.RB5 = 1;
        PORTBbits.RB3 = 1;
        PORTBbits.RB2 = 0;
        PORTBbits.RB0 = 0; 
        myLongDelay();
        break;
    }
    case FIFTH:{
        PORTAbits.RA2 = 1;
        PORTAbits.RA3 = 1;
        myLongDelay();
        PORTAbits.RA2 = 0;
        PORTAbits.RA3 = 0;
        
        PORTBbits.RB5 = 1;       
        PORTBbits.RB3 = 1;
        myLongDelay();
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 0;
        
        PORTBbits.RB2 = 1;
        PORTBbits.RB0 = 1;
        myLongDelay();
        PORTBbits.RB2 = 0;
        PORTBbits.RB0 = 0;
        break;
    }
    case SIXTH:{
        PORTBbits.RB5 = 1;       
        PORTBbits.RB3 = 1;
        myShortDelay();
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 0;
        
        PORTBbits.RB2 = 1;
        PORTAbits.RA3 = 1;
        myShortDelay();
        PORTBbits.RB2 = 0;
        PORTAbits.RA3 = 0;
        
        PORTAbits.RA2 = 1;
        PORTBbits.RB0 = 1;
        myShortDelay();
        PORTAbits.RA2 = 0;
        PORTBbits.RB0 = 0;
        break;
    }
    default:{
        PORTAbits.RA2 = 1;      
        PORTAbits.RA3 = 0;    
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 0;
        PORTBbits.RB2 = 0;
        PORTBbits.RB0 = 1;
        
        myShortDelay();
        
        PORTAbits.RA2 = 0;      
        PORTAbits.RA3 = 0;    
        PORTBbits.RB5 = 0;
        PORTBbits.RB3 = 0;
        PORTBbits.RB2 = 0;
        PORTBbits.RB0 = 0; 
        
        myShortDelay();
    }

    }
}

void myShortDelay(){
    delayBuffer = globalCounter + 3;
        if (delayBuffer >= 25){
          delayBuffer -= 24;
        }
        while (delayBuffer != globalCounter );
}

void myLongDelay(){
    delayBuffer = globalCounter + 10;
        if (delayBuffer >= 25){
          delayBuffer -= 24;
        }
        while (delayBuffer != globalCounter );
}