#include "RHP.h"

void encrypt_rhp(char* msg, uint8_t type, uint16_t srcPort, uint8_t* frame) {
	int index;
	uint16_t checksum = 0;

	frame[0] = type;
	frame[1] = PORT % 256;
	frame[2] = PORT / 256;
	frame[3] = srcPort % 256;
	frame[4] = srcPort / 256;

	for (int i = 0; i < strlen(msg); i++) {
		frame[i+5] = msg[i];
	}

	index = strlen(msg) + 5;

	if (strlen(msg) % 2 == 0) {
		frame[index] = 0;
		index++;
	}

	for (int i = 0; i < index-1; i+=2) {
		checksum += frame[i] * 256 + frame[i+1];
	}

	uint16_t complement = checksum ^ 1;

	frame[index] = complement % 256;
	frame[index+1] = complement / 256;
}

// void decrypt_rhp