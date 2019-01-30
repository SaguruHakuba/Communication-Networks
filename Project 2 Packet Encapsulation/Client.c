#include "UDP.c"
#include "RHP.c"


#define MESSAGE "hello"
#define BUFSIZE 1024

int main() {
	uint16_t total;


	char buffer[BUFSIZE];
	uint8_t frame[strlen(MESSAGE) + 8];
	encrypt_rhp(MESSAGE, 0, 2256, frame);


	for(int loop = 0; loop < strlen(MESSAGE)+8; loop++)
	      printf("%d ", frame[loop]);

	udp(frame, buffer, BUFSIZE);
	printf("Received from server: %s\n", buffer);
	// for(int i = 0; i < 30; i++)
	//       printf("%d ", buffer[i]);

	total = buffer[0] + buffer[1] * 256;
	int j = 0;
	while (j==30) {
		j+=2;
		total += buffer[j] + buffer[j+1] * 256;
	}

	printf("total is %d\n", total);
	printf("frame0 is %hhu\n", buffer[0]);
	printf("frame1 is %hhu\n", buffer[1]);
	printf("frame2 is %hhu\n", buffer[2]);
	printf("frame3 is %hhu\n", buffer[3]);
	printf("frame4 is %hhu\n", buffer[4]);
	printf("frame5 is %hhu\n", buffer[5]);

	for(int i = 0; i < 30; i++)
	      printf("%d ", buffer[i]);
}