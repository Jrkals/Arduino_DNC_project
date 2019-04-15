#include "CORE.h"

CORE_COLOR_SENSOR color;

//// these will be updated as colors are read. They’re basically “running” codes
//long code1 = 0;
//long code2 = 0;
long *codes;

int halfDelay = 1000;

// example: colorCodeToBaseTen[1] = 17 means the color code of 1 gets mapped to 17
int *colorCodeToBaseTen;
int maxColorCode = 32;

void fillColorCodeArray() {
  colorCodeToBaseTen = malloc(sizeof(int) * maxColorCode);
  // encode colors as base ten numbers by filling the colorCodeToBaseTen array
  // first, put -1 everywhere so we can tell if we lookup an invalid color code
  for(int i = 0; i < maxColorCode; i++) {
    colorCodeToBaseTen[i] = -1;
  }
  // IMPORTANT the codes should be between 0 and 9 so that they don't step on each other's toes in base-ten land
  colorCodeToBaseTen[3] = 0; // blue
  colorCodeToBaseTen[4] = 0; // blue
  colorCodeToBaseTen[10] = 1; // red
  colorCodeToBaseTen[1] = 2; // white
  colorCodeToBaseTen[0] = 3; // off
  colorCodeToBaseTen[9] = 4; // yellow
}

// asciiToCodeword[3] = 20429 means the char of ascii 3 is mapped to codeword 20429
long *asciiToCodeword;
long asciiTableSize = 128;
int numColorsPerChar = 6;
int colorIndex = 0;
long maxHammingDistance = 100;
long maxBaseFiveCodeword;

long myPow(int num, int exponent) {
  long answer = 1;
  while(exponent > 0) {
    answer *= num;
    exponent -= 1;
  }
  return answer;
}

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

void generateCodewords() {
  asciiToCodeword = malloc(sizeof(int) * asciiTableSize);
  // fill asciiToCodeword will evenly-spaced codewords
  long minCodeWord = myPow(5, numColorsPerChar);
  long maxCodeWord = myPow(5, numColorsPerChar + 1);
  maxBaseFiveCodeword = baseTenToBaseFive(maxCodeWord);
  long distanceBetweenCodewords = (maxCodeWord - minCodeWord) / asciiTableSize;
//  Serial.println(maxCodeWord);
  for(int i = 0; i < asciiTableSize; i++) {
//    Serial.println("codeword:");
//    Serial.println(distanceBetweenCodewords * i);
    asciiToCodeword[i] = minCodeWord + (distanceBetweenCodewords * i);
  }
}

void setup() {
  Serial.begin(9600);
  codes = malloc(sizeof(long) * numColorsPerChar);
  for(int i = 0; i < numColorsPerChar; i++) {
      codes[i] = 0;
  }
  fillColorCodeArray();
  generateCodewords();
  color.colorSetup(color.PASSIVE, color.SIXTY);
  Serial.println("setup complete");
}

long hammingDistance(long num1, long num2) {
  return abs(num1 - num2);
}

int indexOfNearestCodeWord(long *code) {
  int indexOfGreater = 0;
  while(asciiToCodeword[indexOfGreater] < *code && indexOfGreater < asciiTableSize) {
    indexOfGreater++;
  }
  if(indexOfGreater == asciiTableSize) {
    return indexOfGreater - 1;
  }
  int indexOfLesser;
  if(indexOfGreater == 0) {
    indexOfLesser = 0;
  }else{
    indexOfLesser = indexOfGreater - 1;
  }
  if(abs(asciiToCodeword[indexOfGreater] - *code) < abs(*code - asciiToCodeword[indexOfLesser])) {
    return indexOfGreater;
  } else {
    return indexOfLesser;
  }
}

void outputCharacter(char character) {
  Serial.println(character);
}

void updateCode(long *code) {
  // update code according to read color
  // this is the only place where colors are converted to binary
  if ((int)(log(*code) / log(5)) >= numColorsPerChar){
    *code = 0;
  }
  int colorCode = color.getColorNumber();
  int baseTenColorCode = colorCodeToBaseTen[colorCode];
  if(baseTenColorCode < 0) {
    Serial.println("color code not in colorCodeToBaseTen:");
    Serial.println(colorCode);
    baseTenColorCode = 1;
  }
  (*code) *= 10;
  (*code) += baseTenColorCode;
  //(*code) = (*code) % maxBaseFiveCodeword;
}

long baseFiveToBaseTen(long baseFive) {
  long baseTen = 0;
  int magnitude = 0;
  while(baseFive > 0){
    long remainder = baseFive % 10;
    baseFive /= 10;
    baseTen += myPow(5, magnitude) * remainder;
    magnitude++;
  }
  return baseTen;
}

// returns true if it finds a codeword
// and false if it does not
int codeSearch(long *code) {
  Serial.println(*code);
  updateCode(code);
  long baseTenCode = baseFiveToBaseTen(*code);
  int nearestCodewordIndex = indexOfNearestCodeWord(baseTenCode);
  long nearestCodeword = asciiToCodeword[nearestCodewordIndex];
//  Serial.println(nearestCodeword);
  //Serial.println(hammingDistance(baseTenCode, nearestCodeword));
  if (hammingDistance(baseTenCode, nearestCodeword) <= maxHammingDistance) {
    char character = (char) nearestCodewordIndex;
    Serial.println("character:");
    Serial.println(nearestCodewordIndex);
    //outputCharacter(character);
    return 1;
  }
  return 0;
}

int codeIndex = 0;
int numCodesToUpdate = 1;
int readsSinceLastFind = 0;
void loop() {
//  if (alternator) {
//    if (codeSearch(&code1)) {
//      code1 = 0;
//      code2 = 0;
//    }
//  } else {
//    if (codeSearch(&code2)) {
//      code1 = 0;
//      code2 = 0;
//    }
//  }
  for(int i = 0; i < numCodesToUpdate; i++) {
    if(codeSearch(&(codes[i]))){
      readsSinceLastFind = -1;
      numCodesToUpdate = 0;
      for(int j = 0; j < numColorsPerChar; j++) {
        codes[j] = 0;
      }
      break;
    }
  }
//  for(int i = 0; i < numCodesToUpdate; i++) {
//    if(readsSinceLastFind > 0 && (readsSinceLastFind + i) % numColorsPerChar == 1) {
//      codes[i] = 0;
//    }
//  }
//  readsSinceLastFind++;
  if (numCodesToUpdate < numColorsPerChar) {
    numCodesToUpdate++;
  }
  delay(halfDelay);
}
