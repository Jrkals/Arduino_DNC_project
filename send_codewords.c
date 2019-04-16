#include "CORE.h"

CORE_COLOR_BEACON beacon;

int fullDelay = 1000;
char *message = "h";
int colorBase = 5;
int *colorBaseToColorCode;
int maxColorCode = 32;
long *asciiToCodeword;
long asciiTableSize = 128;
int numColorsPerChar = 6;

long myPow(int num, int exponent) {
  long answer = 1;
  while(exponent > 0) {
    answer *= num;
    exponent -= 1;
  }
  return answer;
}

int digitAtIndex(int index, long num) {
  while(index > 0) {
    num = num / 10;
    index--;
  }
  return (int) (num % 10);
}

void fillColorCodeArray() {
  if(colorBase > 9) {
    Serial.println("Error: colorBase cannot exceed 9");
    return;
  }
  colorBaseToColorCode = malloc(sizeof(int) * colorBase);
  colorBaseToColorCode[0] = beacon.BLUE;
  colorBaseToColorCode[1] = beacon.RED;
  colorBaseToColorCode[2] = beacon.WHITE;
  colorBaseToColorCode[3] = beacon.GREEN;
  colorBaseToColorCode[4] = beacon.YELLOW;
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

void setup() {
  Serial.begin(9600);
  fillColorCodeArray();
  generateCodewords();
  Serial.println("setup complete");
}

int charIndex = 0;
int colorIndex = 0;

long charBeingSent;
long codewordBeingSent;
int colorBaseNumToSend;

void loop() {
  if(colorIndex >= numColorsPerChar) {
    colorIndex = 0;
    charIndex++;
  }
  if(message[charIndex] == '\0') {
    charIndex = 0;
  }
  charBeingSent = message[charIndex];
  codewordBeingSent = asciiToCodeword[charBeingSent];
  Serial.println(codewordBeingSent);
  colorBaseNumToSend = digitAtIndex(colorIndex, codewordBeingSent);
  beacon.setColor(colorBaseToColorCode[colorBaseNumToSend]);
  colorIndex++;
  delay(fullDelay);
}