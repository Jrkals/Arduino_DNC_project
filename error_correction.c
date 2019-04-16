#include "CORE.h"

CORE_COLOR_SENSOR color;

int halfDelay = 500;
int colorBase = 5;
int *colorCodeToColorBase;
int maxColorCode = 32;
long *asciiToCodeword;
long asciiTableSize = 128;
int numColorsPerChar = 6;
long maxHammingDistance = 1;

long myPow(int num, int exponent) {
  long answer = 1;
  while(exponent > 0) {
    answer *= num;
    exponent -= 1;
  }
  return answer;
}

void fillColorCodeArray() {
  colorCodeToColorBase = malloc(sizeof(int) * maxColorCode);
  // encode colors as base ten numbers by filling the colorCodeToColorBase array
  // first, put -1 everywhere so we can tell if we lookup an invalid color code
  for(int i = 0; i < maxColorCode; i++) {
    colorCodeToColorBase[i] = -1;
  }
  colorCodeToColorBase[3] = 0; // blue
  colorCodeToColorBase[4] = 0; // blue
  colorCodeToColorBase[10] = 1; // red
  colorCodeToColorBase[1] = 2; // white
  colorCodeToColorBase[0] = 3; // off (unreliable, should be removed)
  colorCodeToColorBase[9] = 4; // yellow
}

long baseTenToColorBase(long baseTen) {
  long numInColorBase = 0;
  int magnitude = 0;
  long remainder;
  while(baseTen > 0){
    remainder = baseTen % colorBase;
    baseTen /= colorBase;
    numInColorBase += myPow(10, magnitude) * remainder;
    magnitude++;
  }
  return numInColorBase;
}

void generateCodewords() {
  asciiToCodeword = malloc(sizeof(long) * asciiTableSize);
  // fill asciiToCodeword will evenly-spaced codewords
  long minBaseTenCodeWord = myPow(5, numColorsPerChar - 1);
  long maxBaseTenCodeWord = myPow(5, numColorsPerChar) - 1;
  Serial.println("min base 10 codeword:");
  Serial.println(minBaseTenCodeWord);
  Serial.println("max base 10 codeword:");
  Serial.println(maxBaseTenCodeWord);
  long distanceBetweenCodewords = (maxBaseTenCodeWord - minBaseTenCodeWord) / asciiTableSize;
  long baseTenCodeword;
  long colorBaseCodeword;
  for(int i = 0; i < asciiTableSize; i++) {
    baseTenCodeword = minBaseTenCodeWord + (distanceBetweenCodewords * i);
//    Serial.println("base 10 codeword:");
//    Serial.println(baseTenCodeword);
    colorBaseCodeword = baseTenToColorBase(baseTenCodeword);
//    Serial.println("color base codeword:");
//    Serial.println(colorBaseCodeword);
    asciiToCodeword[i] = colorBaseCodeword;
  }
}
// 4111002

void updateCode(long *code) {
  int colorCode = color.getColorNumber();
  int colorBaseColorCode = colorCodeToColorBase[colorCode];
  if(colorBaseColorCode < 0) {
    Serial.println("color code not in colorCodeToBaseTen:");
    Serial.println(colorCode);
    colorBaseColorCode = 1;
  }
  (*code) *= 10;
  (*code) += colorBaseColorCode;
  // after adding another digit, the code might be too long
  // we'll shorten it be "erasing" the digit that is too large
  long eraser = myPow(10, numColorsPerChar);
  for(int i = 0; i < colorBase; i++) {
    (*code) = (*code) % eraser;
  }
}

int hammingDistance(long num1, long num2) {
  int distance = 0;
  while(num1 > 0 && num2 > 0) {
    if((num1 % 10) != (num2 % 10)) {
      distance++;
    }
    num1 /= 10;
    num2 /= 10;
  }
  while(num1 > 0) {
    num1 /= 10;
    distance++;
  }
  while(num2 > 0) {
    num2 /= 10;
    distance++;
  }
  return distance;
}

void outputCharacter(char theChar) {
  Serial.println("found character:");
  Serial.println(theChar);
}

boolean codeSearch(long *code) {
  updateCode(code);
  Serial.println("searching with code:");
  Serial.println(*code);
  for(int i = 0; i < asciiTableSize; i++) {
    if(hammingDistance(*code, asciiToCodeword[i]) <= maxHammingDistance) {
      outputCharacter((char) i);
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(4800);
  fillColorCodeArray();
  generateCodewords();
  color.colorSetup(color.PASSIVE, color.SIXTY);
  Serial.println("setup complete");
}

boolean alternator = false;
// these will be updated as colors are read. They’re basically “running” codes
long code1 = 0;
long code2 = 0;

void loop() {
  if (alternator) {
    if (codeSearch(&code1)) {
      code1 = 0;
      code2 = 0;
    }
  } else {
    if (codeSearch(&code2)) {
      code1 = 0;
      code2 = 0;
    }
  }
  delay(halfDelay);
}