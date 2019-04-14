#include "CORE.h"

int fullDelay = 100;

char *message = "hello";

// asciiToCodeword[3] = 20429 means the char of ascii 3 is mapped to codeword 20429
int *asciiToCodeword;
int asciiTableSize = 128;
int numberOfBitsInCodewords = 7;

int colorIndex = 0;

CORE_COLOR_BEACON beacon;

void generateCodewords() {
  asciiToCodeword = malloc(sizeof(int) * asciiTableSize);
  // based on numberOfBitsInCodewords,
  // fill asciiToCodeword will evenly-spaced codewords.
  // codewords cannot be greater than 2 ^ numberOfBitsInCodewords.
  int maxCodeWord = pow(2, numberOfBitsInCodewords);
  int distanceBetweenCodewords = maxCodeWord / asciiTableSize;
  for(int i = 0; i < asciiTableSize; i++) {
    asciiToCodeword[i] = distanceBetweenCodewords * i;
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
  baseTenToColorCode[2] = beacon.WHITE;
  baseTenToColorCode[3] = beacon.GREEN;
  baseTenToColorCode[4] = beacon.OFF;
}

int *colorsToSend;
int lastColorIndex;

int baseTenToBaseFive(int baseTen) {
  int baseFive = 0;
  while(baseTen > 0){
    int power = 1;
    int exponent = 0;
    while(power <= baseTen) {
      power *= 5;
      exponent += 1;
    }
    power /= 5;
    exponent -= 1;
    baseTen -= power;
    baseFive += exponent;
  }
  return baseFive;
}

void getColorsToSend() {
  colorsToSend = malloc(sizeof(int) * 100);
  int i = 0;
  int colorsToSendIndex = 0;
  while(message[i] != '\0') {
    Serial.println("hi");
    char charToSend = message[i];
    int codeword = baseTenToBaseFive(asciiToCodeword[charToSend]);
    // now need to convert codeword to color codes and add each color code to the array
    while(codeword > 0) {
      int nextDigit = codeword % 10;
      codeword /= 10;
      int nextColorCode = baseTenToColorCode[nextDigit];
      colorsToSend[colorsToSendIndex] = nextColorCode;
      colorsToSendIndex++;
    }
    i++;
  }
  colorsToSend[colorsToSendIndex] = -1;
  lastColorIndex = colorsToSendIndex - 1;
  colorIndex = lastColorIndex;
}

void setup() {
  Serial.begin(9600);
  fillColorCodeArray();
  generateCodewords();
  getColorsToSend();
  Serial.println();
  Serial.println("new line");
}



void loop() {
  //Serial.println("in loop");
  if(colorsToSend[colorIndex] == -1) {
    colorIndex = lastColorIndex;
  }
  beacon.setColor(colorsToSend[colorIndex]);
  colorIndex--;
  delay(fullDelay);
}