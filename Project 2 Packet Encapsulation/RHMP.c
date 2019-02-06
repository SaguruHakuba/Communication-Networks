#include "RHMP.h"

#define commID 312

void encrypt_rhmp(char* msg, int messageLen, uint8_t type, uint8_t* frame, uint32_t id_response) {
	int index;

	frame[0] = type % 64 + (commID % 4) * 64;

	frame[1] = commID / 4;

	if (type == 4) {
		frame[2] = 4;
		frame[4] = id_response % 256;
		frame[5] = id_response / 256 % 256;
		frame[6] = id_response / 256 / 256 % 256;
		frame[7] = id_response / 256 / 256 / 256 % 256;
	} else if (type == 16) {
		frame[2] = messageLen;
		for (int i = 0; i < messageLen; i++) {
			frame[i+3] = msg[i];
		}
	} else {
		frame[2] = 0;
	}



}