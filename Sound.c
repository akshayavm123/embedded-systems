// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

//Globals
const uint8_t *curSoundArray; //pointer to the current sound data
uint32_t curSoundLen; //length of the current sound array
uint32_t curIdx; //current index in array
#define BusClock 80000000
#define SampleFreq 11025



void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL=0x00; //disable systick
  SysTick->LOAD=period-1; //set reload value
  SysTick->VAL=0; //clear current value
  SysTick->CTRL=0x07; //enable Systick with IEN on and CLCK on

  SCB->SHP[1] = (SCB->SHP[1] & 0x00FFFFFF) | (priority << 24);
  // write this
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5-bit DAC
void Sound_Init(void){
  DAC5_Init();
}
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    // output one value to DAC if a sound is active
  DAC5_Out(curSoundArray[curIdx]);
  curIdx=curIdx+1;
  if(curIdx==curSoundLen){
    SysTick->LOAD=0;
  }

}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
  curSoundArray= pt;
  curSoundLen= count;
  SysTick_IntArm(BusClock/SampleFreq, 2);
  
}
void Sound_Shoot(void){
// write this
  Sound_Start(shoot, 4080);
}

void Sound_Explosion(void){
// write this
 Sound_Start(explosion, 8731);
}



