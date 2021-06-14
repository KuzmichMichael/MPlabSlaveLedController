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
    ledStateBuffer = RCREG;   
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
    
    if (ledStateBuffer & firstLEDcode){
    PORTAbits.RA2 = 1;
    } else PORTAbits.RA2 = 0;
   
    if (ledStateBuffer & secondLEDcode){
    PORTAbits.RA3 = 1;
    } else PORTAbits.RA3 = 0;
        
    if (ledStateBuffer & thirdLEDcode){
    PORTBbits.RB5 = 1;
    } else PORTBbits.RB5 = 0;
    
    if (ledStateBuffer & fourthLEDcode){
    PORTBbits.RB3 = 1;
    } else PORTBbits.RB3 = 0;

    if (ledStateBuffer & fifthLEDcode){
    PORTBbits.RB2 = 1;
    } else PORTBbits.RB2 = 0;
    
    if (ledStateBuffer & sixthLEDcode){
    PORTBbits.RB0 = 1;
    } else PORTBbits.RB0 = 0;
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