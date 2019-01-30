/* From textbook
 * Modified by: Ryan Oberlitner and Scott Cai
 * ECE312 Project 1
 * 12/12/2018
 * This code connects over the internet to another c program called
 * SocketsServer.c, using Sockets. It allows two-way real-time text
 * communication between the two programs. The program prompts the
 * user for a username and also requires the IP address of the server
 * Usage: ./SocketsClient [IP address] [port number]
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];		// variable that stores the message
	char name[16];			// variable that stores the name input
	char username[256];		// stores the username in angle brackets
	char msg[256];			// stores the username and message
	char servAd[50];		// stores the address of the server
	printf("Provide user name: ");	// ask for user name
	fgets(name,15,stdin);		// wait for user input for name
	strtok(name,"\n");		// remove the new line character from name
	sprintf(username,"<%s> ",name);	// add brackets and store in 'username'
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	inet_ntop(AF_INET, &(serv_addr.sin_addr), servAd, 50);	// Get the server address
	printf("Connection established with %s.\n",servAd);		// Say you have a connection

	pid_t pid = fork();		// create a new process
	if (pid == -1) {
		printf("Error forking");
		exit(3);
	}

	if (pid == 0) {
		// The child process transmits messages
		while(1) {
			bzero(buffer,256);					// clear buffer
			printf("<you> ");					// write to the screen
			fgets(buffer,255,stdin);			// accept input from the user
			if (strcmp(buffer,"exit\n")==0) {	// if the exit signal should be sent
				n = write(sockfd,"exit\n",5);	// send the exit signal
				close(sockfd);					// close sockets
				exit(0);						// exit the process
			}
			strcpy(msg,username);				// copy the username into 'msg'
			strcat(msg,buffer);					// add the buffer (from user) to 'msg'
			n = write(sockfd,msg,strlen(msg));	// send 'msg' over the link
			if (n < 0)							// check for an error
				error("ERROR writing to socket");
		}
	}

	else {
		// The parent process recieves messages
		while(1) {
			bzero(buffer,256);					// clear buffer
			n = read(sockfd,buffer,255);		// read incoming message
			if (n < 0)							// check for an error
				error("ERROR reading from socket");
			if (strcmp(buffer,"exit\n")==0) {	// check for an exit signal
				n = write(sockfd,"exit\n",5);	// send an exit signal if necessary
				printf("\nExiting\n");			// say you're exiting
				close(sockfd);					// close the socket connection
				kill(pid, SIGKILL);				// kill the process transmitting
				return;							// return, ending the program
			}
			printf("\n%s<you> ", buffer);		// Print out the message
			fflush(stdout);						// Make sure the '<you>' prints correctly
		}
	}
	return 0;
}

