#include <Wire.h>

const int bufferSize = 16;
byte byteBuffer[bufferSize] = {0}; // initalize buffer to hold bufferSize bytes
int numBytesStored = 0;

// SDA is analog 4, SCK is analog 5 on Uno

void setup() {
	Serial.begin(115200);	// start serial comms
	Wire.begin();			// start I2C
	Serial.println("Ready to go!");
}

void loop() {

	bool validAddress = false;

	Serial.print("Enter address: ");
	String rawAddress = getLine();
	int address = stringToInt(rawAddress);
	Serial.print(address);
	Serial.print(" (");
	Serial.print(address, HEX);
	Serial.println(")");

	if (address < 0 or address > 127) {
		Serial.println("Invalid I2C address.");
	} else {
		validAddress = true;
	}

	if (validAddress) {

		Serial.print("Enter bytes to send: ");
		getBytes(); // stores bytes in the byte buffer
		Serial.println("Done.");

		int i = 0;
		Wire.beginTransmission(address);

		Serial.print("Sending: ");
		for (int i = 0; i < numBytesStored; i++) {
			Wire.write(byteBuffer[i]);
			Serial.print(byteBuffer[i], HEX);
			Serial.print(" ");
		}
		Serial.println();

		Wire.endTransmission();
		Serial.println("Done!");

		while (Serial.available()) { Serial.read(); }
	
	}
}

void wireXmitByte(int addr, byte data) {
	Wire.beginTransmission(addr);
	Wire.write(data);
	Wire.endTransmission();
}

String getLine() {
	
	// used to find if the line has ended yet
	bool done = false;
	
	// wait for serial to get data. this procedure is blocking.
	while (!Serial.available()) { /* nop */ }
	
	// clear buffer
	String inLine = "";
	
	// read until newline
	while (!done) {
		if (Serial.available()) {
			char inChar = Serial.read();
			if (inChar == '\n') {\
				done = true;
			} else {
				inLine += inChar;
			}
		}
	}

	// clear incoming serial buffer after newline
	while (Serial.available()) { Serial.read(); }

	// return result as String
	return inLine;
}

// takes in ASCII and pops bytes into the buffer until newline char
void getBytes() {

	// used to find if the line has ended yet
	bool done = false;

	// wait for serial to get data. this procedure is blocking.
	while (!Serial.available()) { /* nop */ }
	
	// clear buffer and reset byte count
	for (int i = 0; i < bufferSize; i++) {
		byteBuffer[i] = 0x00;
	}
	numBytesStored = 0;
	
	// read until newline
	int bytePos = 0;
	while (!done and bytePos < bufferSize) {
		if (Serial.available()) {
			byte inByte = Serial.read();
			if (inByte == '\n') {\
				done = true;
			} else {
				byteBuffer[bytePos] = inByte;
				numBytesStored++;
				bytePos++;
			}
		}
	}

	// clear incoming serial buffer after newline
	while (Serial.available()) { Serial.read(); }

}

/* stringToInt
 * params: any string
 * returns: an int from -99 to 999
 * this function trims any string to 3 chars and returns it as an int
 * non-numerical values return 0
 */
int stringToInt(String numStr) {
	char tmpChars[4] = {0}; // holds three digits and a null terminator; supports up to the number 999
	int strLen;
	strLen = numStr.length();
	if (strLen > 2) { strLen = 2; }
	for (int i = 0; i <= strLen; i++) {
		tmpChars[i] = numStr[i];
	}
	return atoi(tmpChars);
}