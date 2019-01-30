/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 9876
#define BUFSIZE 1024

int main() {
    int udpSocket, nBytes;
    char buffer[BUFSIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    int i;

    /* Create UDP socket */
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket\n");
        return 0;
    }

    /* Bind the socket to any valid IP address and a specific port */
    /* Configure settings in address struct */
    memset((char *)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /* Bind socket with address struct */
    if (bind(udpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        return 0;
    }

    printf("waiting on port %d\n", PORT);
    addr_size = sizeof (clientAddr);
    /* Try to receive any incoming UDP datagram. Address and port of
      requesting client will be stored on clientAddr variable */
    nBytes = recvfrom(udpSocket, buffer, BUFSIZE, 0, (struct sockaddr *) &clientAddr, &addr_size);
    printf("received %d bytes\n", nBytes);

    /* Convert message received to uppercase */
    for (i = 0; i < nBytes; i++)
        buffer[i] = toupper(buffer[i]);

    /* Send uppercase message back to client, using clientAddr as the address */
    if (sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *) &clientAddr, addr_size) < 0) {
        perror("sendto failed");
        return 0;
    }

    close(udpSocket);
    return 0;
}