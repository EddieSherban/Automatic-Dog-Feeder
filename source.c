/*
 * File:   source.c
 * Author: Myles, Eddie
 *
 * Created on July 5, 2022, 2:10 PM
 */


#include <xc.h>
#include "sourceheader.h"

#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer (Watchdog Timer is disabled)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
//this is stuff im trying to add to make the RTC work
#pragma config RTCOSC = SOSC            // RTCC Reference Oscillator  Select (RTCC uses Secondary Oscillator (SOSC))



#pragma config FNOSC = FRCDIV          // Initial Oscillator Select (Fast RC Oscillator with Postscaler and PLL module (FRCPLL))
#pragma config POSCMOD = NONE   
#pragma config OSCIOFNC = ON            // OSCO Pin Configuration (OSCO pin functions as port I/O (RA3))
#pragma config PLL96MHZ = ON            // 96MHz PLL Startup Select (96 MHz PLL Startup is enabled automatically on start-up)
#pragma config PLLDIV = NODIV           // USB 96 MHz PLL Prescaler Select (Oscillator input divided by 12 (48 MHz input))
#pragma config IESO = OFF                // Internal External Switchover (IESO mode (Two-Speed Start-up) enabled)
#pragma config SOSCSEL = IO

void main(void) {
//    CLKDIVbits.RCDIV0=1;
//    CLKDIVbits.RCDIV1=1; 
//    CLKDIVbits.RCDIV2=1;  
//    CLKDIV = CLKDIV | (0x7 << 8); 
//    REFOCONbits.ROEN=1;
//    REFOCONbits.ROSEL=0;
    
    /* OSCILLATOR STUFF */
    CLKDIV |= DIV_32KHZ;  //postscale (divide) RC oscillator by 256 to achieve 31.25KHz/2 system clock
    
    
    REFOCON = REFOCON | (0x1 << 15); //this has to do with the oscillator, shift 1 bit 15 times to the left
    REFOCON = REFOCON & ~(0x1 << 12);    
    AD1PCFG = 0xFFFF;
    
    /* INPUTS AND OUTPUTS */
    TRISA &= ~BIT3; //set RA3 as OUTPUT
    TRISB &= ~(BIT2|BIT4); //set RB2 & RB4 as OUTPUTS
    TRISB = TRISB | BIT8; //set RB8 as INPUT
    
    LATA = LATA & ~BIT3;
    
    /* INTERRUPTS */ 
    timer_init();
    __builtin_enable_interrupts(); // enable global interrupts
    LATB = LATB ^ (BIT2|BIT4);
    
    /* initialize state in running mode */
    blinky_LED = 0;
    stable_LED = 0;
    pauseCount = 0;
    feeder_state = running;
    switch_state = not_pressed;
    
    while(1){
        /* check state to determine running LED function */
        state = check_switch();
        if(state == running){
            pauseCount = 0;
            running_LED(blinky_LED);          // (feeder is running LED1 indicator)
        }   else blinky_LED = 0;
        
        /* check state to determine stable LED function */
        state = check_switch();
        if(state == paused){
            if(pauseCount == 0){
                stable_LED = 1;
                paused_LED(stable_LED);           // (turn on paused LED2 indicator)
                paused_upCount = upCount;
            }   else {
                stable_LED = 1;
                paused_LED(stable_LED);           // (turn on paused LED2 indicator)
                upCount = paused_upCount;
                pauseCount = 1;
            }
            pauseCount++;
        }   else LATB = LATB & ~LED2;         //turn off stable LED2 if not paused
        
        /* count up to 12 hours */
        if (upCount == 10){                   // after 12 hours, output a high voltage on RB4
            LATA = LATA ^ BIT3;               // toggle test LED
            upCount = 0;                      // reset seconds counter
        }
    }
    
    return;
}
int check_switch(){
    prev_switch_state = switch_state;
    if (SWITCH_ONE_PRESSED) {
        switch_state = pressed;
    }   else switch_state = not_pressed;  // this is to avoid problems with button, since function runs many times on the press of a button
    if (switch_state == pressed && prev_switch_state != switch_state){ //only change the feeder state when switch state changes
        feeder_state ^= 1;
    }
    return feeder_state;
}

void running_LED (int blinky_LED) { // LED1 indicates running state by turning on and off every second
    if (blinky_LED == 1){
        LATB = LATB | LED1;  //turn on LED
    } 
    else {
        LATB = LATB & ~LED1; //turn off LED 
    }
}

void paused_LED (int stable_LED) { // LED2 indicates paused state
    
    /* SECOND LED */        
    if (stable_LED == paused){
        LATB = LATB | LED2;  //turn on LED
    } 
    else {
        LATB = LATB & ~LED2; //turn off LED 
    }
}


void __attribute__((interrupt(auto_psv))) _T1Interrupt(void){
    blinky_LED ^= 0x1; // change LED state
    upCount++;         // count up once per second
    IFS0bits.T1IF = 0;
}

void timer_init() {
    /* Configuring timer from datasheet */
    T1CONbits.TON = 0;
    T1CONbits.TSIDL = 0;
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0x0;
    T1CONbits.TCS = 0;
    TMR1 = 0;
    PR1 = 15625;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
        
    T1CONbits.TON = 1; //turn timer on after initializing
}