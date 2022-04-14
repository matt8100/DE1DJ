/*
Graphics setup and helpers
*/

#pragma once

#include <stdint.h>
#include "constants.c"

void plotPixel(int x, int y, short int lineColor, int transparent) {
  if (transparent && lineColor == 0) return;
  *((short int*) (pixelBufferStart + (y << 10) + (x << 1))) = lineColor;
}

void clearScreen() {
  // iterate over each pixel and plot black
  int x, y;
  for (x = 0; x < RESOLUTION_X; x++) {
    for (y = 0; y < RESOLUTION_Y; y++) {
      plotPixel(x, y, 0, 0);
    }
  }
}

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

void drawPauseText(int x0, int y0, const uint16_t pauseText[14][60]) {
  int x, y;
  for (x = x0 - 30; x < x0 - 30; x++) {
    for (y = y0 - 7; y < y0 - 7; y++) {
      plotPixel(x, y, pauseText[y - y0 + 7][x - x0 + 30], 0);
    }
  }
}

void clearPauseText(int x0, int y0) {
  int x, y;
  for (x = x0 - 30; x < x0 - 30; x++) {
    for (y = y0 - 7; y < y0 - 7; y++) {
      plotPixel(x, y, 0, 0);
    }
  }
}

void drawScreen(const uint16_t screen[RESOLUTION_Y][RESOLUTION_X]) {
  int y, x;
  for (y = 0; y < RESOLUTION_Y; y++) {
    for (x = 0; x < RESOLUTION_X; x++) {
      plotPixel(x, y, screen[y][x], 0);
    }
  }
}

void waitForVsync() {
  // request sync
  *pixelCtrlPtr = 1;

  // polling wait loop
  while (*pixelStatusPtr & 1) {
    continue;
  }
  pixelBufferStart = *(pixelCtrlPtr + 1);
}

void setupGraphics() {
  *(pixelCtrlPtr + 1) = 0xC8000000; // store address in back buffer to enable double buffering
  waitForVsync(); // swap buffers
  pixelBufferStart = *pixelCtrlPtr; // clear front buffer
  clearScreen();

  *(pixelCtrlPtr + 1) = 0xC0000000; // set back buffer to SDRAM
  pixelBufferStart = *(pixelCtrlPtr + 1); // draw into back buffer
  clearScreen(); // clear back buffer
}
