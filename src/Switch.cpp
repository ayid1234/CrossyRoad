/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author: Diya Shrawagi & Ayushi Kate
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; //up switch
    IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00040081; //down switch
  
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
  return (GPIOA->DIN31_0 & (3<<26)); //replace this your code
}
