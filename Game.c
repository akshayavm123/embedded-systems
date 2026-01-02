// Game.c
// Whack-A-Mole game engine
// Last Modified: December 1, 2025
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "../inc/Arabic.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "Game.h"
#include "images/images.h"


volatile uint32_t ADCvalue = 0;
int8_t WAM_Screen = 0; //0 - menu, 1- game, 2- end
int8_t WAM_Language = 0; //0 - english, 1 - spanish
int8_t mallet_Row = 1;
int8_t score = 0;
uint32_t n=1;
Uint32_t z=0;
uint32_t Random32(void){
  n = 1664525*n+1013904223;
  return n;
}
uint32_t Random6(void){
	Z= (Random32()>>24)%6;
  return z;


}
const uint8_t HolePositions[6][2] = {
  {20, 60},   {20, 100},  {20, 140},  
  {100, 60},  {100, 100},  {100, 140} // Bottom row
};




//init that when you call it does start screen
//language check and change to spanish if pa25 is pressed
void Game_Start(void){
  ST7735_FillScreen(ST7735_BLACK);
  WAM_Screen = 0;
  ST7735_SetCursor(3, 5);
  ST7735_OutString("Whack A Mole");
  ST7735_SetCursor(3, 8);
  ST7735_OutString("English");
  Clock_Delay1ms(500);
  ST7735_SetCursor(3, 10);
  ST7735_OutString("Espanol");
  uint16_t redColor = ST7735_Color565(255, 0, 0);  // pure red
  ST7735_DrawSmallCircle(8, 80, redColor);
  ST7735_DrawBitmap(60, 130, mallet, 16,16);
}
//creates game map
void Game_Init(void){
  __disable_irq();
  ST7735_FillScreen(ST7735_WHITE);
  ST7735_PlotClear(0,2000);
  ST7735_FillScreen(ST7735_WHITE);
  __enable_irq();
  uint16_t whiteColor = ST7735_Color565(255, 255, 255);
  ST7735_DrawSmallCircle(8, oldY, whiteColor);
  if (WAM_Language == 0){
    ST7735_SetCursor(4, 1);
    ST7735_OutString("Whack A Mole");
  } else {
    ST7735_SetCursor(4, 1);
    ST7735_OutString("Golpea Un Topo");
  }
  ST7735_DrawBitmap(20, 60, hole, 16,10);
  ST7735_DrawBitmap(20, 100, hole, 16,10);
  ST7735_DrawBitmap(20, 140, hole, 16,10);
  ST7735_DrawBitmap(100, 60, hole, 16,10);
  ST7735_DrawBitmap(100, 100, hole, 16,10);
  ST7735_DrawBitmap(100, 140, hole, 16,10);
}


void Game_End(void){
  __disable_irq();
  ST7735_PlotClear(0,2000);
  ST7735_FillScreen(ST7735_BLACK);
  uint16_t blackColor = ST7735_Color565(0, 0, 0);
  //ST7735_DrawSmallCircle(8, 80, blackColor);
  ST7735_DrawSmallCircle(8, 100, blackColor);
  __enable_irq();
  if (WAM_Language == 0){
    ST7735_SetCursor(4, 1);
    ST7735_OutString("Whack A Mole");
    ST7735_SetCursor(4, 3);
    ST7735_OutString("Game Over");
    ST7735_SetCursor(4, 5);
    static char buf[20];
    sprintf(buf, "Your Score: %d", score);
    ST7735_OutString(buf);
  } else {
    ST7735_SetCursor(4, 1);
    ST7735_OutString("Golpea Un Topo");
    ST7735_SetCursor(4, 3);
    ST7735_OutString("Fin Del Juego");
    ST7735_SetCursor(4, 5);
    static char buf2[20];
    sprintf(buf2, "Your Score: %d", score);
    ST7735_OutString(buf2);
  }
}
// ST7735_DrawBitmap(20, 59, mole1, 16,16);
// ST7735_DrawBitmap(20, 99, explode, 16,16);


//mole spawn


uint32_t Random6(void) {


  return ((Random32()>>24)%6)+1;  // returns 1, 2, 3, 4, or 5 or 6


}
uint8_t attempts = 0;
uint8_t holeIndex = 0;
uint8_t x;
uint8_t y;
void Game_SpawnMole(void){
  // spawns mole
 
  while (attempts < 20) {
    holeIndex = Random6();
    x = HolePositions[holeIndex][0];
    y = HolePositions[holeIndex][1];
    ST7735_DrawBitmap(x, (y-5), mole1, 16,16);
    attempts++;
    Clock_Delay1ms(1500);
    ST7735_DrawBitmap(x, (y-5), mole1_white, 16,16);
    ST7735_DrawBitmap(20, 60, hole, 16,10);
    ST7735_DrawBitmap(20, 100, hole, 16,10);
    ST7735_DrawBitmap(20, 140, hole, 16,10);
    ST7735_DrawBitmap(100, 60, hole, 16,10);
    ST7735_DrawBitmap(100, 100, hole, 16,10);
    ST7735_DrawBitmap(100, 140, hole, 16,10);
  }
  WAM_Screen = 2;
  Game_End();
}
//mallet moving isr
uint32_t Data;        // 12-bit ADC
uint32_t Position;
uint32_t Time;
uint32_t Flag = 0;
//160x128
int16_t oldmY = 130;
int16_t oldmX = 60;
uint16_t newmY;
void Move_Mallet(){
  Position = Convert(Data);
  if(Position<630){
    newmY = 130;
  } else if (Position<1260){
    newmY = 90;
  } else {
    newmY = 50;
  }


  if(mallet_Row == 1){
    ST7735_DrawBitmap(20, oldmY, mallet_white, 16,16);
    ST7735_DrawBitmap(20, newmY, mallet, 16,16);
    oldmY = newmY;
    // oldmX = 20;
  } else {
    ST7735_DrawBitmap(100, oldmY, mallet_white, 16,16);
    ST7735_DrawBitmap(100, newmY, mallet, 16,16);
    oldmY = newmY;
    // oldmX = 100;
  }
  ST7735_DrawBitmap(20, 60, hole, 16,10);
  ST7735_DrawBitmap(20, 100, hole, 16,10);
  ST7735_DrawBitmap(20, 140, hole, 16,10);
  ST7735_DrawBitmap(100, 60, hole, 16,10);
  ST7735_DrawBitmap(100, 100, hole, 16,10);
  ST7735_DrawBitmap(100, 140, hole, 16,10);
}


int16_t oldY = 80;
int16_t newY;
void Move_Dot(){
  Position = Convert(Data);
  if(Position < 1000){
    newY = 100;
    WAM_Language = 1;
  } else {
    newY = 80;
    WAM_Language = 0;
  }
  uint16_t blackColor = ST7735_Color565(0, 0, 0);
  ST7735_DrawSmallCircle(8, oldY, blackColor);
  uint16_t redColor = ST7735_Color565(255, 0, 0);
  ST7735_DrawSmallCircle(8, newY, redColor);


  oldY = newY;
    }


//isr for button press that changes the column


//whack and replace mole with explosion if in threshold and inc points , otherwise don't inc points


//end screen trigged by the big timer countdown


//in main call init




