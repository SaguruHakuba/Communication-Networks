#include "UDP.h"

int udp(char* message, char* buffer, int bufferLen) {
	int clientSocket, nBytes;
    
    struct sockaddr_in clientAddr, serverAddr;

    /*Create UDP socket*/
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket");
        return 0;
    }

    /* Bind to an arbitrary return address.
     * Because this is the client side, we don't care about the address 
     * since no application will initiate communication here - it will 
     * just send responses 
     * INADDR_ANY is the IP address and 0 is the port (allow OS to select port) 
     * htonl converts a long integer (e.g. address) to a network representation 
     * htons converts a short integer (e.g. port) to a network representation */
    memset((char *) &clientAddr, 0, sizeof (clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(0);

    if (bind(clientSocket, (struct sockaddr *) &clientAddr, sizeof (clientAddr)) < 0) {
        perror("bind failed");
        return 0;
    }

    /* Configure settings in server address struct */
    memset((char*) &serverAddr, 0, sizeof (serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /* send a message to the server */
    if (sendto(clientSocket, message, strlen(message), 0,
            (struct sockaddr *) &serverAddr, sizeof (serverAddr)) < 0) {
        perror("sendto failed");
        return 0;
    }

    /* Receive message from server */
    nBytes = recvfrom(clientSocket, buffer, bufferLen, 0, NULL, NULL);

    // printf("Received from server: %s\n", buffer);

    close(clientSocket);
    return 0;
}