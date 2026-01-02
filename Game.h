// Game.h
// Whack-A-Mole game header
// Minasha Gunarathna and Akshaya Vinaya Manikandan
// Last Modified: December 1, 2025


#ifndef GAME_H
#define GAME_H


#include <stdint.h>


// Game constants
#define NUM_COLS 3
#define NUM_ROWS 2
#define NUM_HOLES (NUM_COLS * NUM_ROWS)  // 6 holes total


// Enumerations
typedef enum {English, Spanish} Language;
typedef enum {MENU, PLAYING, GAMEOVER} State;


// Mole structure
typedef struct {
  uint8_t active;
  uint8_t timer;
  uint8_t hit;
} Mole;


// Game state structure
typedef struct {
  State state;
  uint32_t score;
  uint8_t lives;
  Language language;
  uint8_t malletRow;
  uint8_t malletCol;
  Mole moles[6];
  uint32_t spawnTimer;
  uint32_t explosionTimer;
  int8_t explosionHole;
} GameState;


// Function prototypes
void Game_Init(void);
void Game_Start(void);
void Game_Update(void);
void Game_SpawnMole(void);
void Game_Whack(void);
void Game_ToggleRow(void);
void Move_Dot(void);
void Move_Mallet(void);


// External variables


extern volatile uint32_t ADCvalue;
extern volatile uint8_t WhackFlag;
extern volatile uint8_t RowToggleFlag;
extern uint32_t Random(uint32_t n);
extern uint32_t Random32(void);
extern uint32_t Data;        // 12-bit ADC
extern uint32_t Position;
extern uint32_t Time;
extern uint32_t Flag;
extern int8_t WAM_Screen;
extern int8_t WAM_Language;
extern int16_t oldY;
extern int8_t mallet_Row;


//images
// extern const unsigned short explode[];
// extern const unsigned short hole[];
// const unsigned short mole1[];
// const unsigned short mallet[];
#endif // GAME_H





