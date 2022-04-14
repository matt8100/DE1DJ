#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constants.c"
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
  memset(state.laneArrow, 0, sizeof(state.laneArrow));
  state.noteSpeed = 5;

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
      case PAUSED:
        drawPaused();
        break;
      default: state.mode = MAINMENU;
    }
  }
}
