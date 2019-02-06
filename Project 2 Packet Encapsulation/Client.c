#include "UDP.c"
#include "RHP.c"
#include "RHMP.c"
// #include "Client.h"

#define MESSAGE "hello"
#define BUFSIZE 1024

_Bool checkChecksum(char*);
void interpretReceivedMsg(char*);

int main() {
	char buffer[BUFSIZE];
	int RHPLength = strlen(MESSAGE) + 7 + (strlen(MESSAGE) % 2 == 0);
	uint8_t frame[RHPLength];
	uint8_t rhmpFrame[8];

	// step1 : RHP "Hello"
	// printf("\nStep 1:\n");
	// encrypt_rhp(MESSAGE, strlen(MESSAGE), 1, 2256, frame);
	// for(int loop = 0; loop < RHPLength; loop++)
	//       printf("%d ", frame[loop]);
	// udp(frame, RHPLength, buffer, BUFSIZE);
	// interpretReceivedMsg(buffer);
	
	// step2: 
	printf("\nStep 2:\n");
	encrypt_rhmp(0, 0, 8, rhmpFrame, 0);
	encrypt_rhp(rhmpFrame, 3, 0, 2256, frame);
	for(int loop = 0; loop < 10; loop++)
	      printf("%d ", frame[loop]);
	udp(frame, 10, buffer, BUFSIZE);
	interpretReceivedMsg(buffer);
}

void interpretReceivedMsg(char* buffer) {
	uint16_t srcPort = 0, dstPort = 0;

	dstPort += buffer[1]+(buffer[1]<0)*256;
	dstPort += 256*(buffer[2]+(buffer[2]<0)*256);
	srcPort += buffer[3]+(buffer[3]<0)*256;
	srcPort += 256*(buffer[4]+(buffer[4]<0)*256);

	printf("\nReceived from server: \n");
	for(int i = 0; i < 80; i++) {
	    printf("%d ", buffer[i]);
	}

	printf("\nRHP type: %u\n", buffer[0]);
	if (buffer[0] == 1) {
		printf("length: %u\n", dstPort);
		printf("srcPort: %u\n", srcPort);
		printf("message: %s\n", buffer+5);
	} else {
		//rhmp
		printf("dstPort: %u\n", dstPort);
		printf("srcPort: %u\n", srcPort);
		printf("message: %s\n", buffer+9);
	}

	if (checkChecksum(buffer))
		printf("Checksum is valid.");
	else 
		printf("Checksum bad.");
}

_Bool checkChecksum(char* buffer) {
	int bufferLen = -1;
	if (buffer[0] == 1) {
		bufferLen = buffer[1] + buffer[2] * 256;
		bufferLen = bufferLen + 7 + (bufferLen % 2 == 0);
	} else {
		bufferLen = buffer[7];
		printf("bufferLen before is %u\n", bufferLen);
		bufferLen = bufferLen + 10 + (bufferLen % 2 == 1);
	}

	uint32_t total = 0;
	for (int i = 0; i < bufferLen; i+=2) {
		if (buffer[i] < 0) {
			total += 256; 
		}
		if (buffer[i+1] < 0) {
			total += 256*256;
		}
		total += buffer[i] + buffer[i+1] * 256;
	}

	total = total + total / 65536;
	total = total % 65536;

	printf("Checksum is %u\n Buffer length is %u\n", total, bufferLen);
	return total == 0x0000ffff;
}