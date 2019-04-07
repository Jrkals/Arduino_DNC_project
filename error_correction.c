// these will be updated as colors are read. They’re basically “running” codes
int code1 = 0;
int code2 = 0;

// asciiToCodeword[3] = 20429 means the char of ascii 3 is mapped to codeword 20429
int *asciiToCodeword;
int asciiTableSize = 128;
int numberOfBitsInCodewords = 17;
int numberOfColorsPerCodeWord;
int maxHammingDistance = 3;

void generateCodewords() {
	asciiToCodeword = malloc(sizeof(int) * asciiTableSize);
	// based on numberOfBitsInCodewords,
	// fill asciiToCodeword will evenly-spaced codewords.
	// codewords cannot be greater than 2 ^ numberOfBitsInCodewords.
	// can take each ascii character and multiply by 25 to have an array of size 3125. Each 	code word then has room for 25 neighbors, with only 15 possible neighbors using base 5.
}

void setup() {
	numberOfColorsPerCodeWord = ?;
fillColorCodeArray()
	generateCodewords();
}

int hammingDistance(int num1, int num2) {
	
}

int indexOfNearestCodeWord(int *code) {
	// search through asciiToCodeword to find the codeword
	// that is closest to the given code.
	// return the index of the codeword.
	// “nearest” means smallest hamming distance
}

void outputCharacter(char character) {
	// print it if you want
}

int base = 5; // the number of colors we use for communication

// example: colorCodeToBaseTen[1] = 17 means the color code of 1 gets mapped to 17
int *colorCodeToBaseTen;
int maxColorCode = 13;

void fillColorCodeArray() {
	colorCodeToBaseTen = malloc(sizeof(int) * maxColorCode);
	// encode colors as base ten numbers by filling the colorCodeToBaseTen array
	
}

void updateCode(int *code) {
	// update code according to read color
	// this is the only place where colors are converted to binary
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
}
