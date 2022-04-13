/* 
HEX display helpers
*/

#pragma once

void displayScore() {
  int score = state.score;
  int patterns[6] = {0b00111111,0b00111111,0b00111111,0b00111111,0b00111111,0b00111111};

  int digitCount = 0;
  while (score > 0) {
    int digit = score % 10;
    int bitPattern;

    switch (digit) {
      case 1:
        bitPattern = 0b00000110;
        break;
      case 2:
        bitPattern = 0b01011011;
        break;
      case 3:
        bitPattern = 0b01001111;
        break;
      case 4:
        bitPattern = 0b01100110;
        break;
      case 5:
        bitPattern = 0b01101101;
        break;
      case 6:
        bitPattern = 0b01111101;
        break;
      case 7:
        bitPattern = 0b00000111;
        break;
      case 8:
        bitPattern = 0b01111111;
        break;
      case 9:
        bitPattern = 0b01100111;
        break;
      default: bitPattern = 0b00111111;
    }

    patterns[digitCount] = bitPattern;
    digitCount++;
    if (digitCount == 6) break;

    score /= 10;
  }

  int hex5To4Pattern = (patterns[5] << 8) | patterns[4];
  int hex3To0Pattern = (patterns[3] << 24) | (patterns[2] << 16) | (patterns[1] << 8) | patterns[0];

  *((int*) HEX5_HEX4_BASE) = hex5To4Pattern;
  *((int*) HEX3_HEX0_BASE) = hex3To0Pattern;
}
