/*
Graphics setup and helpers
*/

#pragma once

#include <stdint.h>
#include "constants.h"

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
