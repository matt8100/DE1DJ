/*
Interrupt setup and subroutines
*/

#pragma once

#include <string.h>
#include "constants.c"
#include "gameplay.c"
#include "hex.c"
#include "gameplayBGM.c"
#include "mainMenuBGM.c"

/* Setup */
void setupIRQStack() {
  int stack = 0xFFFFFFFF - 7; // stack pointer for IRQ mode
  int mode = 0b11010010; // IRQ mode
  asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
  asm("mov sp, %[ps]" : : [ps] "r" (stack));

  mode = 0b11010011; // SVC mode
  asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

void configInterrupt(int n, int cpuTarget) {
  // configure Set-Enable Registers (ICDISERn)
  int regOffset = (n >> 3) & 0xFFFFFFFC;
  int address = 0xFFFED100 + regOffset;
  int index = n & 0x1F;
  int value = 0x1 << index;
  
  *((int*) address) |= value;

  // configure Interrupt Processor Target Register (ICDIPTRn)
  regOffset = (n & 0xFFFFFFFC);
  index = n & 0x3;
  address = 0xFFFED800 + regOffset + index;
  *((char*) address) = (char) cpuTarget;
}

void configGIC() {
  configInterrupt(PRIV_TIMER_INTERRUPT, 1);
  configInterrupt(AUDIO_INTERRUPT, 1);
  configInterrupt(PS2_INTERRUPT, 1);

  *((int*) ICCPMR) = 0xFFFF; // Interrupt Priority Mask Register, enables interrupts of all priorities
  *((int*) ICCICR) = 1; // CPU Interface control Register, enables signaling of interrupts
  *((int*) ICDDCR) = 1; // Distributor Control Register, send pending interrupts to CPU
}

void configPS2() {
  *(PS2Ptr + 1) = 1; // eanble interrupts for PS/2 port
}

void configPrivTimer() {
  *privateTimerPtr = 200 * 1000000 * 0.2; // load; every 0.2 seconds
  *(privateTimerPtr + 2) = 0b010; // enable automatic 
}

void configAudio() {
  *audioPtr |= 0b10; // enable write interrupt
}

void enableInterrupts() {
  int status = 0b01010011; // SVC mode, interrupts enabled
  asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

void disableInterrupts() {
  int status = 0b11010011;
  asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

void setupInterrupts() {
  setupIRQStack();
  configGIC();
  configPS2();
  configPrivTimer();
  configAudio();
  enableInterrupts();
}

void checkHit(int lane) {
  int position = state.laneArrow[lane];
  if (position > 160 && position < 200) {
    state.laneArrow[lane] = 0;
    state.score += 1;
    displayScore();
  }

  position = state.laneArrow[lane + 1];
  if (position > 160 && position < 200) {
    state.laneArrow[lane + 1] = 0;
    state.score += 1;
    displayScore();
  }
}

/* Interrupt Subroutines */
void PS2ISR() {
  int data, RVALID, RAVAIL;

  do {
    data = *PS2Ptr;
    RVALID = data & 0x8000;
    RAVAIL = data & 0xFFFF0000;

    if (RVALID) {
      keyboardData[1] = keyboardData[0];
      keyboardData[0] = data & 0xFF;
    }
  } while (RAVAIL > 1);

  uint16_t key = (keyboardData[1] << 8) | keyboardData[0];

  switch(state.mode) {
    case MAINMENU:
      if (key == SPACE) {
        state.mode = GAMEPLAY;
        state.audioTime = 0;
        state.audioSize = GAMEPLAYSONGLENGTH;
        state.score = 0;

        clearScreen();
        waitForVsync();
        clearScreen();

        *(privateTimerPtr + 2) = 0b111;
      }
      break;

    case GAMEPLAY:
      switch (key) {
        case LEFTARROW:
          checkHit(LANE1);
          break;
        case DOWNARROW:
          checkHit(LANE2);
          break;

        case UPARROW:
          checkHit(LANE3);
          break;

        case RIGHTARROW:
          checkHit(LANE4);
          break;

        case PGUP:
          if (state.noteSpeed < 10) {
            state.noteSpeed++;
          }
          break;
        
        case PGDN:
          if (state.noteSpeed > 1) {
            state.noteSpeed--;
          }
          break;

        case ESC:
          state.mode = PAUSED;
          onPause();
          *(privateTimerPtr + 2) = 0b010; // disable timer
          *audioPtr = 0; // enable write interrupt
          break;
        default: break;
      }
      break;
    case PAUSED:
      switch (key) {
        case ESC:
          state.mode = GAMEPLAY;
          *(privateTimerPtr + 2) = 0b111; // disable timer
          *audioPtr = 0b10; // enable write interrupt
          break;
        case BKSP:
          state.mode = MAINMENU;
          state.audioTime = 0;
          state.audioSize = MAINMENUSONGLENGTH;
          *audioPtr = 0b10; // enable write interrupt
          memset(state.laneArrow, 0, sizeof(state.laneArrow));
          break;
      }
    default: break;
  }
}

// randomizes when a note will begin descending
void privateTimerISR() {
  int i;
  for (i = 0; i < NUMNOTES; i++) {
    if (!state.laneArrow[i] && rand() % 201 == 0) {
      state.laneArrow[i] += 1;
    }
  }

  *(privateTimerPtr + 3) = 1; // reset interrupt status
}

void audioISR() {
  unsigned int fifoSpace;

  while (1) {
    fifoSpace = *(audioPtr + 1); // read FIFO register

    // if available left/right write space, write
    if ((fifoSpace & 0x00FF0000) > 0 &&	(fifoSpace & 0xFF000000) > 0) {
      int sample = 0;
      
      switch (state.mode) {
        case MAINMENU:
          sample = MainMenuBGM[state.audioTime] * 100000;
          break;
        case GAMEPLAY:
          sample = GameplayBGM[state.audioTime] * 100000;
          break;
        default: sample = 0;
      }

			*(audioPtr + 2) = sample; // left FIFO
			*(audioPtr + 3) = sample; // right FIFO
      state.audioTime += 1;

      // loop audio
      if (state.audioTime == state.audioSize) {
        state.audioTime = 0;

        // if end of gameplay song, return to main menu
        if (state.mode == GAMEPLAY) {
          state.mode = MAINMENU;
          state.audioTime = MAINMENUSONGLENGTH;
          memset(state.laneArrow, 0, sizeof(state.laneArrow));
        }
      }
		}
    // else exit
    else {
      break;
    }
  }
}

/* Exception Handlers */
void __attribute__ ((interrupt)) __cs3_isr_irq(void) {
  int interruptID = *((int*) ICCIAR);

  switch(interruptID) {
    // private timer interrupt
    case PRIV_TIMER_INTERRUPT:
      privateTimerISR();
      break;
    // PS/2 interrupt
    case AUDIO_INTERRUPT:
      audioISR();
      break;
    case PS2_INTERRUPT:
      PS2ISR();
      break;
    default: break;
  }

  *((int*) ICCEOIR) = interruptID; 
}

void __attribute__ ((interrupt)) __cs3_reset(void) {
  while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi(void) {
  while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort(void) {
  while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort(void) {
  while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq(void) {
  while(1);
}
