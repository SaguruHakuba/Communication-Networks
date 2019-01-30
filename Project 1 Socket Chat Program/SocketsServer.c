 /*From Textbook
 Modified by Ryan Oberlitner and Scott Cai
 ECE312 Project 01
 12/12/2018
 This code connects over the Internet to another c program called SocketsClient.c with sockets.
 It allows two-way real-time text
 communication between the two programs. The program prompts the
 user for a username and waits for the connection of the other client
 Usage: ./socketServer [port number]
 */

 #include <stdio.h>
  #include <sys/types.h> 
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <signal.h>

  void error(char *msg)
  {
    //print error messages
      perror(msg);
      exit(1);
  }

   int main(int argc, char *argv[])
   {
        int sockfd, newsockfd, portno, clilen;
        char buffer[256];   //variable that stores the message
        char clientAd[50];  //variable that stores client address
        char name[16];  //variable that sotres your username
        char username[256];
        char msg[256];  //the whole message is made up of username and buffer

        struct sockaddr_in serv_addr, cli_addr;
        int n;

        printf("Provide user name: "); //asking for your user name
        fgets(name, 15, stdin);
        strtok(name, "\n");
        sprintf(username, "<%s> ", name); //read in username and add brackets

        if (argc < 2) {
            fprintf(stderr,"ERROR, no port provided\n");
            exit(1);
        }
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
           error("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0) 
                 error("ERROR on binding");
        //finshed socket opening and binding, waiting for connection

        printf("Waiting for connection...\n"); 

        //listening for client connection
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
             error("ERROR on accept");

        inet_ntop(AF_INET, &(cli_addr.sin_addr), clientAd, 50);
        printf("Connection established with %s...\n", clientAd);

        pid_t pid = fork(); //fork a child process from the parent one
        if (pid == 0) {
          //here is the child process
        	while(1) {
	        	bzero(buffer,256); //clear buffer
		        printf("<You> ");
	        	fgets(buffer,255,stdin); //get the text you wrote
	        	if(strcmp(buffer, "exit\n")==0) { //decide if it is exit signal
	        		n = write(newsockfd, "exit\n", 5); //send exit signal if receive exit
	        		close(sockfd);
	        		exit(0);
	        	}
	        	strcpy(msg, username);
	        	strcat(msg, buffer);
		        n = write(newsockfd, msg, strlen(msg)); //write message to client
		        if (n < 0) error("ERROR writing to socket");
	        }
        } else {
          //here is the parent process
	        while(1) {
	        	bzero(buffer,256); //clear buffer
		        n = read(newsockfd,buffer,255); //read messages from the buffer
		        if (n < 0) error("ERROR reading from socket");
		        if(strcmp(buffer, "exit\n")==0) { //decide if receive exit signal
		        	n = write(newsockfd, "exit\n", 5);
		        	printf("\nEXITING!!!\n");
		        	close(sockfd);
		        	kill(pid, SIGKILL); //kill the  child process if exit signal receives
		        	return 0;
		        }
		        printf("\n%s<You> ",buffer);
		        fflush(stdout); //make sure <You> prints correctly
	        }
        }
        return 0; 
   }
