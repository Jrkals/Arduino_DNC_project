// these will be updated as colors are read. They’re basically “running” codes
int code1 = 0;
int code2 = 0;

int fullDelay = 100;

char *message = "hello";

// asciiToCodeword[3] = 20429 means the char of ascii 3 is mapped to codeword 20429
int *asciiToCodeword;
int asciiTableSize = 128;
int numberOfBitsInCodewords = 7;

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

// example: baseTenToColorCode[1] = 17 means the color code of 17 gets mapped to 1
int *baseTenToColorCode;
int maxBaseTenCode = 5;

void fillColorCodeArray() {
	baseTenToColorCode = malloc(sizeof(int) * maxBaseTenCode);
	for(int i = 0; i < maxBaseTenCode; i++) {
		baseTenToColorCode[i] = -1;
	}
	// IMPORTANT the codes should be between 0 and 9 so that they don't step on each other's toes in base-ten land
	baseTenToColorCode[9] = 17;
	// etc TODO
}

int *colorsToSend;
int lastColorIndex;

void getColorsToSend() {
	colorsToSend = malloc(sizeof(int) * 1000);
	int i = 0;
	int colorsToSendIndex = 0;
	while(message[messageIndex] != '\0') {
		char charToSend = message[messageIndex];
		int codeword = asciiToCodeword[charToSend];
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
	fillColorCodeArray();
	generateCodewords();
	getColorsToSend();
}

int colorIndex = 0;

void loop() {
	if(colorsToSend[colorIndex] == -1) {
		colorIndex = lastColorIndex;
	}
	beacon.setColor(colorsToSend[colorIndex]);
	colorIndex--;
	delay(fullDelay);
}
