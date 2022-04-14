/*
Contains all program definitions
*/

#pragma once

// State variables
struct State {
  short int mode;

  int audioTime;
  int audioSize;

  int laneArrow[8];
  int noteSpeed;
  int score;
} state;

#define MAINMENU 1
#define GAMEPLAY 2
#define PAUSED 3

#define MAINMENUSONGLENGTH 394675
#define GAMEPLAYSONGLENGTH 1739784

#define LANE1 0
#define LANE2 2
#define LANE3 4
#define LANE4 6
#define NUMNOTES 8

// Helpers
#define abs(x) (x > 0) ? x : -x

// Memory-mapped I/O
#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050

// Audio
#define AUDIO_BASE 0xFF203040
volatile int* audioPtr = (int*) AUDIO_BASE;

// PS/2
#define PS2_BASE 0xFF200100

#define ESC 0xF076
#define BKSP 0xF066
#define PGUP 0xF07D
#define PGDN 0xF07A
#define SPACE 0xF029
#define LEFTARROW 0xF06B
#define RIGHTARROW 0xF074
#define UPARROW 0xF075
#define DOWNARROW 0xF072

volatile int* PS2Ptr = (int*) PS2_BASE;
short int keyboardData[3] = {0, 0};

// Timers
#define MPCORE_PRIV_TIMER 0xFFFEC600

volatile int* privateTimerPtr = (int*) MPCORE_PRIV_TIMER;

// Display
#define PIXEL_BUFFER_BASE 0xFF203020
#define PIXEL_STATUS_BASE 0xFF20302C
#define CHAR_BUFFER_BASE 0xFF203030
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

volatile int* pixelCtrlPtr = (int*) PIXEL_BUFFER_BASE;
volatile int* pixelStatusPtr = (int*) PIXEL_STATUS_BASE;
volatile int pixelBufferStart;


// Interrupts
#define ICCPMR 0xFFFEC104
#define ICCICR 0xFFFEC100
#define ICDDCR 0xFFFED000

#define ICCIAR 0xFFFEC10C
#define ICCEOIR 0xFFFEC110

#define PRIV_TIMER_INTERRUPT 29
#define KEY_INTERRUPT 73
#define AUDIO_INTERRUPT 78
#define PS2_INTERRUPT 79