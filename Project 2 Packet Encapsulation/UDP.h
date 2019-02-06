#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER "137.112.41.52"
#define PORT 1874

int udp(char*, int, char*, int);