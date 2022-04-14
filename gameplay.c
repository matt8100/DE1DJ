/*
Gameplay mode helpers
*/

#pragma once

#include "constants.c"
#include "arrows.c"
#include "graphics.c"
#include "interrupts.c"
#include "gameplayBG.c"

// determines origin point of arrow to draw in specific lane and moves arrows
void drawArrowsInLanes() {
  int x, y;

  // lane 1
  x = 99;
  y = state.laneArrow[0];
  if (y != 0) drawArrow(x, y, LeftArrow);

  y = state.laneArrow[1];
  if (y != 0) drawArrow(x, y, LeftArrow);

  // lane 2
  x = 139;
  y = state.laneArrow[2];
  if (y != 0) drawArrow(x, y, DownArrow);

  y = state.laneArrow[3];
  if (y != 0) drawArrow(x, y, DownArrow);

  // lane 3
  x = 179;
  y = state.laneArrow[4];
  if (y != 0) drawArrow(x, y, UpArrow);

  y = state.laneArrow[5];
  if (y != 0) drawArrow(x, y, UpArrow);

  // lane 4
  x = 219;
  y = state.laneArrow[6];
  if (y != 0) drawArrow(x, y, RightArrow);

  y = state.laneArrow[7];
  if (y != 0) drawArrow(x, y, RightArrow);
}

void drawGameplay() {
  clearScreen();
  drawScreen(gameplayBG);
  drawArrowsInLanes();
  waitForVsync();

  int i;
  for (i = 0; i < NUMNOTES; i++) {
    if (state.laneArrow[i]) {
      state.laneArrow[i] += state.noteSpeed;
      if (state.laneArrow[i] >= 240) state.laneArrow[i] = 0;
    }
  }
}

// set both buffers to be same to avoid potential flicker
void onPause() {
  clearScreen();
  drawScreen(gameplayBG);
  drawArrowsInLanes();
  waitForVsync();
  clearScreen();
  drawScreen(gameplayBG);
  drawArrowsInLanes();
}

void drawPaused() {
  drawPauseText(295, 50, PauseText);
  waitForVsync();
  drawPauseText(295, 50, PauseText);

  int i;
  for (i = 0; i < 30; i++) {
    waitForVsync();
  }

  clearPauseText(295, 50);
  waitForVsync();
  clearPauseText(295, 50);

  for (i = 0; i < 30; i++) {
    waitForVsync();
  }
}
