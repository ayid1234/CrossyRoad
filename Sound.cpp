// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

volatile uint32_t Counts = 0;

void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL = 0;
  SysTick->LOAD = period - 1;
  SysTick->VAL = 0;
  SCB->SHP[1] = (SCB->SHP[1] & (~0xC0000000)) | (priority << 30);
  GPIOB->DOUT31_0 = (GPIOB->DOUT31_0) | 0x20;
  SysTick->CTRL = 0x07;
}
// initialize a 11kHz SysTick, however no sound should be started

// initialize any global variables

int size;
// Initialize the 5 bit DAC
void Sound_Init(void){
    SysTick_IntArm(7272, 0);
    DAC5_Init();
    size = 0;
    SysTick->LOAD = 0;

}
const uint8_t* sound;
extern "C" void SysTick_Handler(void);
void SysTick_Handler(void){ // called at 11 kHz
  if(size)
  {
      DAC5_Out(*sound);
      sound++;
      size--;
  }
  else
  {
      SysTick->LOAD = 0;
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
    sound = pt;
    size = count;
    SysTick->LOAD = 7272;
    SysTick->VAL = 1;
  
}
void Sound_Quack(void){
   Sound_Start(QUACK, 5324);
}

void Sound_Crash(void){
   Sound_Start(DeathAh, 6384);
}

void Sound_Hippy(void){
   Sound_Start(hippywhale, 12021);
}
