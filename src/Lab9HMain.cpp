// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 1/1/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Sprite.h"
#include <math.h>
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern "C" void TIMG8_IRQHandler(void);

Sprite duck;
Sprite fish1(nullptr);
Sprite fish2(nullptr);
Sprite fish3(nullptr);
Sprite bubbles(bubbles4040);
Sprite bubbles2(bubbles4040);
uint32_t Data;
int now;
int last = 0;
int oldx;
int oldy;
int score = 0;

SlidePot Sensor(1500,0); // copy calibration from Lab 7

void PLL_Init(void){ // set phase lock loop (PLL)
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;

const char L1E[] ="Welcome to";
const char L2E[] ="Crossy Road!";
const char L3E[] ="How to Play:";
const char L4E[] ="Avoid the cars,";
const char L5E[] ="and don't die.";
const char L6E[] ="Press the up arrow";
const char L7E[] ="to begin.";
const char L8E[] ="Ouch! Game Over!";
const char L9E[] ="Score: ";

const char L1S[] ="\xAD";
const char L2S[] ="Bienvenidos a";
const char L3S[] ="Crossy Road!";
const char L4S[] ="C\xA2mo Juegas:";
const char L5S[] ="Evita los autos";
const char L6S[] ="y no meures.";
const char L7S[] ="Pressione la";
const char L8S[] ="flecha hacia arriba";
const char L9S[] ="para commencar.";
const char L10S[] ="Ay! Juego ";
const char L11S[] = "Terminado!";
const char L12S[] ="Puntaje: ";

const char *PhrasesE[9]={ L1E, L2E, L3E, L4E, L5E, L6E, L7E, L8E, L9E};
const char *PhrasesS[12]={ L1S, L2S, L3S, L4S, L5S, L6S, L7S, L8S, L9S, L10S, L11S, L12S};


void TIMG12_IRQHandler(void) {
    uint32_t pos, msg;
    if(TIMG12->CPU_INT.IIDX == 1) {
        oldx = duck.getX();
        oldy = duck.getY();

        Data = Sensor.In();
        Data = (120 * Data) / 4095;

        if(Data >= oldx - 15 && Data <= oldx + 15) {
            duck.setX(oldx);
        } else {
            duck.setX(Data);
        }

        if(oldx != duck.getX()) {
            duck.setDraw(1);
        } else {
            duck.setDraw(0);
        }

        int switches = Switch_In();
        now = switches;
        if(switches > last) {
            int up = LaunchPad_InS4()==(1<<27);
            int down = LaunchPad_InS3()==(1<<26);

            if(up != 0) {
                duck.Move_Up();
                score += 100;
                Sound_Quack();
            } else if (down != 0) {
                duck.Move_Down();
                Sound_Quack();
            }
        }

        last = now;
    }


}

bool detectCollision() {
    int fish1sq = sqrt((duck.getX()-fish1.getX())*(duck.getX()-fish1.getX()) + (duck.getY()-fish1.getY())*(duck.getY()-fish1.getY()));
    int fish2sq = sqrt((duck.getX()-fish2.getX())*(duck.getX()-fish2.getX()) + (duck.getY()-fish2.getY())*(duck.getY()-fish2.getY()));
    int fish3sq = sqrt((duck.getX()-fish3.getX())*(duck.getX()-fish3.getX()) + (duck.getY()-fish3.getY())*(duck.getY()-fish3.getY()));
    if(fish1sq < 20 || fish2sq < 20 || fish3sq < 20) {
        //Sound_Crash();
        return true;
    }
    return false;
}


void TIMG8_IRQHandler(void) {
    if((TIMG8->CPU_INT.IIDX) == 1){
       fish1.Move_Left(1);
       fish2.Move_Right(1);
       fish3.Move_Right(1);
    }
}

int main(void) {
    int prev = 0;
  __disable_irq();
  PLL_Init();
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init();
  Switch_Init();
  LED_Init();
  Sound_Init();
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26);
  TimerG12_IntArm(1600000, 2);
  uint16_t tm = (uint16_t) 4444;
  TimerG8_IntArm(tm, 200, 1);

  __enable_irq();
  ST7735_FillScreen(ST7735_HTPBLUE);
  Sound_Hippy();
  ST7735_DrawBitmap(11, 120, hippywhale10580, 105, 80);

  bubbles.setX(80);
  bubbles.setY(40);
  bubbles.DrawBubbles(40,40);

  bubbles2.setX(6);
  bubbles2.setY(155);
  bubbles2.DrawBubbles(40,40);


  for(int i = 0; i < 4; i++) {
      Sound_Hippy();
      bubbles.setY(bubbles.getY() + 3);
      bubbles.DrawBubbles(40,40);
      bubbles2.setY(bubbles2.getY() - 3);
      bubbles2.DrawBubbles(40,40);

      Clock_Delay1ms(400);

      bubbles.setY(bubbles.getY() - 3);
      bubbles.DrawBubbles(40,40);
      bubbles2.setY(bubbles2.getY() + 3);
      bubbles2.DrawBubbles(40,40);

      Clock_Delay1ms(400);
  }

  __disable_irq();

  ST7735_FillScreen(ST7735_HTPBLUE);

  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Choose Language");
  ST7735_SetCursor(1, 4);
  ST7735_OutString((char *)"English");
  ST7735_SetCursor(1, 5);
  ST7735_OutString((char *)"Press the Up");
  ST7735_SetCursor(1, 6);
  ST7735_OutString((char *)"Arrow");

  ST7735_SetCursor(1, 11);
  ST7735_OutString((char *)"Spanish");
  ST7735_SetCursor(1, 12);
  ST7735_OutString((char*)"Pressione la");
  ST7735_SetCursor(1, 13);
  ST7735_OutString((char*)"flecha hacia abajo");


  bubbles.setX(65);
  bubbles.setY(105);
  bubbles.DrawBubbles(40,40);

  int s = 3;
  int count = 1;
  while(Switch_In() == 0) {
      Clock_Delay1ms(300);
      if(count % 2 == 0) {
          s = -3;
      } else {
          s = 3;
      }
      Sound_Hippy();
      bubbles.setY(bubbles.getY() + s);
      bubbles.DrawBubbles(40,40);
      count++;

  }


  int switches2 = Switch_In();
  if(((switches2 >> 27) & 0x01) == 1) {
      myLanguage = English;
  }
  if(((switches2 >> 26) & 0x01) == 1) {
      myLanguage = Spanish;
  }

  if(myLanguage == Spanish) {
      ST7735_FillScreen(ST7735_HTPBLUE);

      ST7735_SetCursor(1, 2);
      ST7735_OutString((char *)PhrasesS[0]);
      ST7735_OutString((char *)PhrasesS[1]);
      ST7735_SetCursor(1, 3);
      ST7735_OutString((char *)PhrasesS[2]);
      ST7735_SetCursor(1, 4);
      ST7735_OutString((char *)PhrasesS[3]);
      ST7735_SetCursor(1, 5);
      ST7735_OutString((char *)PhrasesS[4]);
      ST7735_SetCursor(1, 6);
      ST7735_OutString((char *)PhrasesS[5]);
      ST7735_SetCursor(1, 7);
      ST7735_OutString((char *)PhrasesS[6]);
      ST7735_SetCursor(1, 8);
      ST7735_OutString((char *)PhrasesS[7]);
      ST7735_SetCursor(1, 9);
      ST7735_OutString((char *)PhrasesS[8]);



  } else if(myLanguage == English) {
      ST7735_FillScreen(ST7735_HTPBLUE);

      ST7735_SetCursor(1, 2);
      ST7735_OutString((char *)PhrasesE[0]);
      ST7735_SetCursor(1, 3);
      ST7735_OutString((char *)PhrasesE[1]);
      ST7735_SetCursor(1, 4);
      ST7735_OutString((char *)PhrasesE[2]);
      ST7735_SetCursor(1, 5);
      ST7735_OutString((char *)PhrasesE[3]);
      ST7735_SetCursor(1, 6);
      ST7735_OutString((char *)PhrasesE[4]);
      ST7735_SetCursor(1, 6);
      ST7735_OutString((char *)PhrasesE[5]);
      ST7735_SetCursor(1, 7);
      ST7735_OutString((char *)PhrasesE[6]);


  }

    bubbles.setX(60);
    bubbles.setY(145);
    bubbles.DrawBubbles(40,40);

    bubbles2.setX(10);
    bubbles2.setY(130);
    bubbles2.DrawBubbles(40,40);



  while(LaunchPad_InS4()==(1<<27)){};
  while(LaunchPad_InS4() == 0){};

  ST7735_DrawBitmap(0, 159, roads, 128, 160);

  duck.setX(50);
  duck.setY(196);
  duck.setImage(duck2335);
  duck.Draw(50, 156, 23, 25);

  fish1.setX(90);
  fish1.setY(30);
  fish1.setImage(fishleft4225);
  fish1.setDraw(0);

  fish2.setX(10);
  fish2.setY(110);
  fish2.setImage(fishright4225);
  fish2.setDraw(0);

  fish3.setX(45);
  fish3.setY(70);
  fish3.setImage(fishcool4225);
  fish3.setDraw(0);


  __enable_irq();
  int count1 = 0;
  while(1){
      fish1.DrawFish(42, 25);
      fish2.DrawFish(42, 25);
      fish3.DrawFish(42, 25);
      duck.Draw(oldx, oldy, 23, 35);

      if(detectCollision() == true)
      {

          if(myLanguage == Spanish) {
               Sound_Crash();
               //__disable_irq();
                ST7735_FillScreen(ST7735_HTPBLUE);
                ST7735_SetCursor(1, 2);
                ST7735_OutString((char *)PhrasesS[0]);
                ST7735_OutString((char *)PhrasesS[9]);
                ST7735_SetCursor(1, 3);
                ST7735_OutString((char *)PhrasesS[10]);
                ST7735_SetCursor(1, 4);
                ST7735_OutString((char *)PhrasesS[11]);
                ST7735_OutUDec(score);
                ST7735_DrawBitmap(30, 165, duckfront69105, 69, 105);
                bubbles.setX(80);
                bubbles.setY(68);
                bubbles.setImage(bubbles4040);
                bubbles.DrawBubbles(40,40);

                for(int i = 0; i < 7; i++) {
                    bubbles.setY(bubbles.getY() + 3);
                    bubbles.DrawBubbles(40,40);

                    Clock_Delay1ms(400);

                    bubbles.setY(bubbles.getY() - 3);
                    bubbles.DrawBubbles(40,40);

                    Clock_Delay1ms(400);

                }
                __disable_irq();


            } else if(myLanguage == English) {
                Sound_Crash();
                //__disable_irq();
                ST7735_FillScreen(ST7735_HTPBLUE);
                ST7735_SetCursor(1, 2);
                ST7735_OutString((char *)PhrasesE[7]);
                ST7735_SetCursor(1, 3);
                ST7735_OutString((char *)PhrasesE[8]);
                ST7735_OutUDec(score);
                ST7735_DrawBitmap(30, 165, duckfront69105, 69, 105);

                bubbles.setX(80);
                bubbles.setY(68);
                bubbles.setImage(bubbles4040);
                bubbles.DrawBubbles(40,40);

                for(int i = 0; i < 7; i++) {
                    bubbles.setY(bubbles.getY() + 3);
                    bubbles.DrawBubbles(40,40);

                    Clock_Delay1ms(400);

                    bubbles.setY(bubbles.getY() - 3);
                    bubbles.DrawBubbles(40,40);

                    Clock_Delay1ms(400);

                }
                __disable_irq();
            }
          return 0;
      }
  }
  return 0;
}



