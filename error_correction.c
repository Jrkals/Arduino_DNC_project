// these will be updated as colors are read. They’re basically “running” codes
int code1 = 0;
int code2 = 0;

int halfDelay = 50;

// example: colorCodeToBaseTen[1] = 17 means the color code of 1 gets mapped to 17
int *colorCodeToBaseTen;
int maxColorCode = 13;

void fillColorCodeArray() {
	colorCodeToBaseTen = malloc(sizeof(int) * maxColorCode);
	// encode colors as base ten numbers by filling the colorCodeToBaseTen array
	for(int i = 0; i < maxColorCode; i++) {
		colorCodeToBaseTen[i] = -1;
	}
	// IMPORTANT the codes should be between 0 and 9 so that they don't step on each other's toes in base-ten land
	colorCodeToBaseTen[9] = 17;
	// etc TODO
}

// asciiToCodeword[3] = 20429 means the char of ascii 3 is mapped to codeword 20429
int *asciiToCodeword;
int asciiTableSize = 128;
int numberOfBitsInCodewords = 7;
int maxHammingDistance = 3;

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

void setup() {
	fillColorCodeArray();
	generateCodewords();
}

int hammingDistance(int num1, int num2) {
	return abs(num1 = num2);
}

int indexOfNearestCodeWord(int *code) {
	int indexOfGreater = 0;
	while(asciiToCodeword[indexOfGreater] < *code && indexOfGreater < asciiTableSize) {
		indexOfGreater++;
	}
	if(indexOfGreater == asciiTableSize) {
		return indexOfGreater - 1;
	}
	int indexOfLesser = indexOfGreater - 1;
	if(asciiToCodeword[indexOfGreater] - *code < *code - asciiToCodeword[indexOfLesser]) {
		return indexOfGreater;
	} else {
		return indexOfLesser;
	}
}

void outputCharacter(char character) {
	Serial.println(character);
}

void updateCode(int *code) {
	// update code according to read color
	// this is the only place where colors are converted to binary
	int baseTenColorCode = colorCodeToBaseTen(*code);
	if(baseTenColorCode < 0) {
		Serial.println("color code not in colorCodeToBaseTen: " + *code);
		return;
	}
	(*code) *= 10;
	(*code) += baseTenColorCode;
}

// returns true if it finds a codeword
// and false if it does not
int codeSearch(int *code) {
	updateCode(code);
	int nearestCodewordIndex = indexOfNearestCodeWord(code);
	int nearestCodeword = asciiToCodeword[nearestCodewordIndex];
	if (hammingDistance(*code, nearestCodeword) <= maxHammingDistance) {
		char character = (char) nearestCodewordIndex;
		outputCharacter(character);
		return 1;
	}
	return 0;
}

int alternator = 0;
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
	alternator = !alternator
	delay(halfDelay);
}
