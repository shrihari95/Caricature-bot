#include"TIM.h"


void TIM_init()
{

	/* Initialize TIM Ch 7 (TC7) for periodic interrupts every 1.000 ms */

  TSCR1   = 0b10000000; // Enable timer subsystem
  TIOS    = 0b10000000; // Set Timer Ch7 as Output Compare
  TSCR2   = 0b00001100; // Enable Timer Counter reset by successful output compare on Ch7
                          // and Prescale Factor of 16

  TC7     = 1500000;       // For 1ms period
  TIE     = 0b10000000; // Enable Timer Interrupts 

  
}

void RTI_init()
{

	/* Initialize RTI for 2.048 ms interrupt rate */
	CRGINT = CRGINT | 0x80;
	RTICTL = 0x7F;

}



interrupt 15 void TIM_ISR(void)
{
	// clear TIM CH 7 interrupt flag
	TFLG1 = TFLG1 | 0x80;

	scanButtons();
}



interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flag
  	CRGFLG = CRGFLG | 0x80; 
	
	// 9S12C32 healthy heart beat
    PTT_PTT0 ^= 1;
  	
 
}

