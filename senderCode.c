// Sender code
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Forward Declarations
char** convertMsgToBase5(char*);
char* convertBase5ToCodeWord(char*);
int convertBase5ToBase10(char*);
char* convertBase10ToBase5(int);
char* convertBase5ToLight(char*);
char convertBase5CharToLight(char);
char* convertCharToBase5(char);
char convertIntToChar(int);
int convertCharToInt(char);
char** convertBase5ToLightMsg(char** base5Msg);
int getLengthOfMessage(char*);
char** convertToCodeWord(char**);


//global state variables
char* currentCharacter;

int arrayOfCodeWordsBase10[125];
char* arrayOfCodeWordsBase5[125];

char* message;
int lom;

enum color {
  Black, // 0
  Green, // 1
  Red, // 2
  Blue, // 3
  White, // 4
};

int main(){
  char* testString = "Hi There!";
  message = testString;
  lom = getLengthOfMessage(message);
  char** base5Msg = convertMsgToBase5(testString);
  char** codedMsg = convertToCodeWord(base5Msg);
  char** lightMsg = convertBase5ToLightMsg(codedMsg);
  for(int i = 0; i < lom; i++){
    printf("'%c' %s ", testString[i],  base5Msg[i]);
    printf("%s ", codedMsg[i]);
    printf("%s\n", lightMsg[i]);
  }
  return 0;
}

// "Hi There" -> ["100", "423"..."222"]
char** convertMsgToBase5(char* string){
  char** finalBase5String = (char**) malloc(sizeof(char*)*(sizeof(string)/sizeof(char)));
  int i = 0;
  while(string[i] != '\0'){
    finalBase5String[i] = (char*) malloc(sizeof(char)*3);
    //printf("in while convertMsgToBase5\n");
    char* charAsBase5Str = convertCharToBase5(string[i]);
    //printf("i is %d %s\n", i,charAsBase5Str);
    //finalBase5String[i] = charAsBase5Str;
    strcpy(finalBase5String[i], charAsBase5Str);
    //printf("finalbase5String[%d] %s\n",i, finalBase5String[i]);
    i++;
  }
  //printf("finalbase5String[3] %s\n", finalBase5String[3]);
  return finalBase5String;
}

// "100" -> "21100"
// the first digit added (in this case the 1) is the sum of the next three % 5
// the next digit (in this case the 2) is the sum of the next four digits % 5
char* convertBase5ToCodeWord(char* base5Msg){
  char* codeWord = (char*) malloc(sizeof(char)*(getLengthOfMessage(base5Msg)+3));
  int sum = 0;
  for(int i = 0; i < sizeof(base5Msg)/sizeof(char); i++){
    sum += convertCharToInt(base5Msg[i]);
  }
  int d1 = sum % 5;
  char chard1 = convertIntToChar(d1);
  int d2 = (sum + d1)%5;
  char chard2 = convertIntToChar(d2);

  codeWord[0] = chard2;
  codeWord[1] = chard1;
  codeWord[2] = base5Msg[0];
  codeWord[3] = base5Msg[1];
  codeWord[4] = base5Msg[2];
  codeWord[5] = '\0';
  return codeWord;
}

// "444" -> 124
int convertBase5ToBase10(char* base5Msg){
  int i = 0;
  int power = sizeof(base5Msg)/sizeof(char); // length of the string
  int total = 0;
  while(base5Msg[i] != '\0'){
    //printf("in while convertBase5toBase10\n");
    int n = convertCharToInt(base5Msg[i]);
    int nTot = 1;
    for(int j = 1; j <= power; j++){
      nTot *= n;
    }
    total += n;
    power--;
    i++;
  }
  return total;
}

// 26-> "101"
char* convertBase10ToBase5(int base10Num){
  int originalBase10Num = base10Num;
  char* rv = (char*) malloc(sizeof(char)*4);
  int d1, r1, r2, d2;
  d1 = base10Num / 25;
  r1 = base10Num % 25;
  d2 = r1 / 5;
  r2 = r1 % 5;
  rv[0] = convertIntToChar(d1);
  rv[1] = convertIntToChar(d2);
  rv[2] = convertIntToChar(r2);
  rv[3] = '\0';
  return rv;
}

// "4321" -> "wlrg"
char* convertBase5ToLight(char* base5Str){
  char* lightStr = (char*) malloc(sizeof(base5Str));
  int i = 0;
  while(base5Str[i] != '\0'){
    //printf("in while convertBase5toLight\n");    
    lightStr[i] = convertBase5CharToLight(base5Str[i]);
    i++;
  }
  base5Str[i] = '\0';
  return lightStr;
}

// '0' -> 'b'
char convertBase5CharToLight(char c){
  if(c == '0') {return 'b';}
  if(c == '1') {return 'g';}
  if(c == '2') {return 'r';}
  if(c == '3') {return 'l';}
  else         {return 'w';}
}

// 'H' -> "242"
char* convertCharToBase5(char c){
  //printf("incoming char is %c\n", c);
  int n = c;
  char* base5Msg = convertBase10ToBase5(n);
  printf("'%c' becomes %s\n", c, base5Msg);
  return base5Msg;
}

// 4 -> '4'
char convertIntToChar(int n){
  if(n == 0) return '0';
  if(n == 1) return '1';
  if(n == 2) return '2';
  if(n == 3) return '3';
  else return '4';
}

// '1' -> 1
int convertCharToInt(char c){
  if(c == '0') return 0;
  if(c == '1') return 1;
  if(c == '2') return 2;
  if(c == '3') return 3;
  else return 4;
}

// takes 2d array of base 5 messages and turns them 
// into light signals
char** convertBase5ToLightMsg(char** base5Msg){
  char** rv = (char**) malloc(lom*sizeof(char*));
  for(int i = 0; i < lom; i++){
    char* temp = convertBase5ToLight(base5Msg[i]);
    //printf("%s\n", temp);
    rv[i] = (char*) malloc(5*sizeof(char));
    strcpy(rv[i], convertBase5ToLight(base5Msg[i]));
  }
  return rv;
}

// "Hi there" -> 8
int getLengthOfMessage(char* string){
  int i = 0;
  while(string[i] != '\0'){
    i++;
  }
  return i;
}

// takes 2d array of base 5 messages and turns them
// into their code words still base 5
char** convertToCodeWord(char** msg){
  char** rv = (char**) malloc(lom*sizeof(char*));
  for(int i = 0; i < lom; i++){
    char* temp = convertBase5ToCodeWord(msg[i]);
    //printf("%s\n", temp);
    rv[i] = temp;
  }
  return rv;
}
