#include "RHP.h"

#define RHPport 105

void encrypt_rhp(char* msg, uint8_t type, uint16_t srcPort, uint8_t* frame) {
	int index;
	uint16_t checksum = 0;

	frame[0] = type;

	if (type == 1) {
		frame[1] = (strlen(msg)+1) % 256;
		frame[2] = (strlen(msg)+1) / 256;
	} else {
		frame[1] = RHPport % 256;
		frame[2] = RHPport / 256;
	}

	frame[3] = srcPort % 256;
	frame[4] = srcPort / 256;

	for (int i = 0; i < strlen(msg); i++) {
		frame[i+5] = msg[i];
	}

	index = strlen(msg) + 5;

	if (strlen(msg) % 2 == 1) {
		frame[index] = 0;
		index++;
	}

	for (int i = 0; i < index-1; i+=2) {
		checksum += frame[i+1] * 256 + frame[i];
	}

	uint16_t complement = (-1) * checksum - 1;

	frame[index] = 0;
	frame[index+1] = complement % 256;
	frame[index+2] = complement / 256;
}

// void decrypt_rhp