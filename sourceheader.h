/* 
 * File:   
 * Author: Eddie Sherban
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

void int_init();
void timer_init();
int check_switch();
void running_LED(int blinky_LED);
void paused_LED(int stable_LED);


#define DIV_32KHZ 0x0700  //0000 0111 0000 0000 NOTE: this is divide by 256 so 31.25kHz
#define DIV_125KHZ 0x0600 //0000 0110 0000 0000
#define DIV_250KHZ 0x0600 //0000 0110 0000 0000
#define BIT2 0x4 //0100
#define BIT3 0x8 //1000 or #define BIT3 (0x1 << 3)
#define BIT4 0x10 //0001 0000
#define BIT8 0x100 //0001 0000 0000
#define BIT9 0x200 //0010 0000 0000
#define BIT10 0x400 //0100 0000 0000
#define TWELVE_HOURS 43200
#define running 0
#define paused 1
#define not_pressed 0
#define pressed 1

#define LED1 0x4 //0100 (bit 2)
#define LED2 0x10 // 0001 0000 (bit 4)
#define TEST_OP 0x10 //0001 0000 (bit 4)

#define SWITCH_ONE_PRESSED (PORTB & BIT8) == 0x0

unsigned char blinky_LED;
unsigned char stable_LED;
unsigned int upCount = 0;
unsigned int paused_upCount;
int pauseCount;
int countTrigger;
int feeder_state;
int state;
int switch_state;
int prev_switch_state;

