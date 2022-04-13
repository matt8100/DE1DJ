/*
Gameplay mode helpers
*/

#pragma once

#include "constants.h"
#include "arrows.h"
#include "graphics.c"
#include "gameplayBG.h"

// draws an arrow around an origin point
void drawArrow(int x0, int y0, const uint16_t arrow[32][32]) {
  int x, y;
  for (x = x0 - 16; x < x0 + 16; x++) {
    for (y = y0 - 16; y < y0 + 16; y++) {
      if (x < 0 || x > RESOLUTION_X || y < 0 || y > RESOLUTION_Y) continue;
      plotPixel(x, y, arrow[y - y0 + 16][x - x0 + 16], 1);
    }
  }
}

// determines origin point of arrow to draw in specific lane and moves arrows
void drawArrowInLane(int lane) {
  int x, y;

  switch (lane) {
    case LANE1:
      x = 99;
      y = state.laneArrow[LANE1];
      if (y == 0) break;

      drawArrow(x, y, LeftArrow);
      break;
    case LANE2:
      x = 139;
      y = state.laneArrow[LANE2];
      if (y == 0) break;

      drawArrow(x, y, DownArrow);
      break;
    case LANE3:
      x = 179;
      y = state.laneArrow[LANE3];
      if (y == 0) break;

      drawArrow(x, y, UpArrow);
      break;
    case LANE4:
      x = 219;
      y = state.laneArrow[LANE4];
      if (y == 0) break;

      drawArrow(x, y, RightArrow);
      break;
    default: break;
  }
}

void drawGameplay() {
  clearScreen();
  drawScreen(gameplayBG);
  drawArrowInLane(LANE1);
  drawArrowInLane(LANE2);
  drawArrowInLane(LANE3);
  drawArrowInLane(LANE4);
  waitForVsync();

  for (i = 0; i < 4; i++) {
    if (state.laneArrow[i]) {
      state.laneArrow[i] += 1;
      if (state.laneArrow[i] == 240) state.laneArrow[i] = 0;
    }
  }
}
