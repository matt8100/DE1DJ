#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "graphics.c"
#include "interrupts.c"
#include "hex.c"
#include "mainMenu.c"
#include "gameplay.c"

int main() {
  srand(time(NULL));

  // initialize state
  state.mode = MAINMENU;
  state.audioTime = 0;
  state.audioSize = MAINMENUSONGLENGTH;
  state.score = 0;
  state.laneArrow[0] = 0;
  state.laneArrow[1] = 0;
  state.laneArrow[2] = 0;
  state.laneArrow[3] = 0;

  // setup
  setupInterrupts();
  setupGraphics();
  displayScore();

  // program loop
  while (1) {
    switch(state.mode) {
      case MAINMENU:
        drawMainMenu();
        break;
      case GAMEPLAY:
        drawGameplay();
        break;
      default: state.mode = MAINMENU;
    }
  }
}
