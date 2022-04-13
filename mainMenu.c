/*
Main menu mode heleprs
*/

#pragma once

#include "constants.h"
#include "mainMenuBG.h"

// draws two-frame main menu
void drawMainMenu() {
  clearScreen();
  drawScreen(MainMenuFrame1);
  waitForVsync();
  clearScreen();
  drawScreen(MainMenuFrame1);

  // wait for 1 second
  int i;
  for (i = 0; i < 30; i++) {
    waitForVsync();
  }

  clearScreen();
  drawScreen(MainMenuFrame2);
  waitForVsync();
  clearScreen();
  drawScreen(MainMenuFrame2);

  // wait for 1 second
  for (i = 0; i < 30; i++) {
    waitForVsync();
  }
}
