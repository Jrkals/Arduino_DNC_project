#include "CORE.h"

int fullDelay = 10000;

char *message = "hello";

// asciiToCodeword[3] = 20429 means the char of ascii 3 is mapped to codeword 20429
long *asciiToCodeword;
long asciiTableSize = 128;
// if this is 7, there will be distance 1 between all the codewords since 128 = 2^7
int numColorsPerChar = 6;
int colorIndex = 0;

CORE_COLOR_BEACON beacon;

long myPow(int num, int exponent) {
  long answer = 1;
  while(exponent > 0) {
    answer *= num;
    exponent -= 1;
  }
  return answer;
}

void generateCodewords() {
  asciiToCodeword = malloc(sizeof(int) * asciiTableSize);
  // fill asciiToCodeword will evenly-spaced codewords
  long minCodeWord = myPow(5, numColorsPerChar);
  long maxCodeWord = myPow(5, numColorsPerChar + 1);
  long distanceBetweenCodewords = (maxCodeWord - minCodeWord) / asciiTableSize;
//  Serial.println(maxCodeWord);
  for(int i = 0; i < asciiTableSize; i++) {
//    Serial.println("codeword:");
//    Serial.println(distanceBetweenCodewords * i);
    asciiToCodeword[i] = minCodeWord + (distanceBetweenCodewords * i);
  }
}

int *baseTenToColorCode;
int maxBaseTenCode = 5;

void fillColorCodeArray() {
  baseTenToColorCode = malloc(sizeof(int) * maxBaseTenCode);
  for(int i = 0; i < maxBaseTenCode; i++) {
    baseTenToColorCode[i] = -1;
  }
  // IMPORTANT the codes should be between 0 and 9 so that they don't step on each other's toes in base-ten land
  baseTenToColorCode[0] = beacon.BLUE;
  baseTenToColorCode[1] = beacon.RED;
  baseTenToColorCode[2] = beacon.OFF;
  baseTenToColorCode[3] = beacon.GREEN;
  baseTenToColorCode[4] = beacon.YELLOW;
}

int *colorsToSend;

long baseTenToBaseFive(long baseTen) {
  long baseFive = 0;
  int magnitude = 0;
  while(baseTen > 0){
    long remainder = baseTen % 5;
    baseTen /= 5;
    baseFive += myPow(10, magnitude) * remainder;
    magnitude++;
  }
  return baseFive;
}

void getColorsToSend() {
  colorsToSend = malloc(sizeof(int) * 500);
  int i = 0;
  int colorsToSendIndex = 0;
  while(message[i] != '\0') {
    char charToSend = message[i];
    long baseTenCodeword = asciiToCodeword[charToSend];
    Serial.println(baseTenCodeword);
    long codeword = baseTenToBaseFive(baseTenCodeword);
    Serial.println(codeword);
    // now need to convert codeword to color codes and add each color code to the array
    int charCodeIndex = colorsToSendIndex + numColorsPerChar;
    while(codeword > 0) {
      int nextDigit = codeword % 10;
      codeword /= 10;
      int nextColorCode = baseTenToColorCode[nextDigit];
      colorsToSend[charCodeIndex] = nextColorCode;
      charCodeIndex--;
    }
    colorsToSendIndex += numColorsPerChar;
    i++;
  }
  colorsToSend[colorsToSendIndex] = -1;
}

void setup() {
  Serial.begin(9600);
  fillColorCodeArray();
  generateCodewords();
  getColorsToSend();
  Serial.println("setup complete");
}

void loop() {
  //Serial.println("in loop");
  if(colorsToSend[colorIndex] == -1) {
    colorIndex = 0;
  }
  beacon.setColor(colorsToSend[colorIndex]);
  colorIndex++;
  delay(fullDelay);
}