/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){

//Whack button
 IOMUX-> SECCFG.PINCM[PA12INDEX]= 0x00040081;
//Up/Down Button
 IOMUX-> SECCFG.PINCM[PA17INDEX]= 0x00040081;
//Start/Language button
 IOMUX-> SECCFG.PINCM[PA25INDEX]= 0x00040081;


// handles PA0–PA15
  GPIOA->POLARITY15_0 |= (1 << (12*2));   // PA12 rising

// handles PA16–PA31
  GPIOA->POLARITY31_16 |= (1 << ((17-16)*2));  // PA17 rising
  GPIOA->POLARITY31_16 |= (1 << ((25-16)*2));  // PA25 rising
  
//Clear prior interrupt flags
  GPIOA->CPU_INT.ICLR = (1<<12) | (1<<17) | (1<<25);

//Arm Interrupts
  GPIOA->CPU_INT.IMASK |= (1<<12) | (1<<17) | (1<<25);

//Interrupt of GPIOA is Group 1(IRQ1)
 NVIC->IP[0] = (NVIC->IP[0] & ~(0x0000FF00)) | (2 << 14);

//Enable NVIC line for Group1
 NVIC->ISER[0] = 1 << 1; 
}
// return current state of switches
uint32_t Switch_In(void){
  uint32_t result=0;
  if(GPIOA->DIN31_0 & (1<<12)){
    result |= 0x01;
  }
  if(GPIOA->DIN31_0 & (1<<17)){
    result |= 0x02;
  }
  if(GPIOA->DIN31_0 & (1<<25)){
    result |= 0x04;
  }
    // write this
  return result; // if all set bits 0-2 would be 1's
}
void GROUP1_IRQHandler(void){
    if(GPIOA->CPU_INT.RIS & (1<<12)){    // whack
        GPIOA->CPU_INT.ICLR = 1<<12;
        //WhackHandler();
    }
    if(GPIOA->CPU_INT.RIS & (1<<17)){    // move columns
        GPIOA->CPU_INT.ICLR = 1<<17;
        //SwitchColumn();
    }
    if(GPIOA->CPU_INT.RIS & (1<<25)){    // language/menu toggle
        GPIOA->CPU_INT.ICLR = 1<<25;
        //LanguageMenuSwitch();
    }
}