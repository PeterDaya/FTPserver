/*
 * This program sends the contents of a file
 * specified by a user over the network.
 * You can connect to this server using netcat, although
 * I am writing a client that connects to this server as well. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


#define MAX 5

static void err(char *err) {
	perror(err);
	exit(1);
}

int main(int argc, char **argv) {
	/* Before the infinite for loop, we want to initialize the socket
	 * and server info so it can later accept a client. */

	struct sockaddr_in servAddr, clntAddr;
	int servSock, clntSock;
	unsigned short port;
	unsigned int clntLen;

	if (argc != 2) {
		err("Please type: ./ftpserver <port number>");
	}
	
	port = atoi(argv[1]);
	
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		err("socket()");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		err("bind()");

	if (listen(servSock, MAX) < 0)
		err("listen()");

	for (;;) {
		clntLen = sizeof(clntAddr);
		if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0)
			err("accept()");
		
		printf("Received a connection from user: %s\n", inet_ntoa(clntAddr.sin_addr));
		char fname[100], prompt[100], buf[4096];
		sprintf(prompt, "Please enter the name of a file: ");

		if (send(clntSock, prompt, strlen(prompt), 0) != strlen(prompt))
			err("send()");

		if (recv(clntSock, fname, sizeof(fname), 0) < 0)
			err("recv()");
		
		printf("The user requested the file named: %s\n", fname);
		
		//The file name sent by the user might contain \n so it is necessary
		//to process the string before fopen()
		char *c;
		if ((c = strchr(fname, '\n')))	
			*c = '\0';
		FILE *fp = fopen(fname, "r");
		if (fp == NULL)
			err("The file pointer is NULL");

		int bytes;
		while ((bytes = fread(buf, sizeof(char), sizeof(buf), fp)) > 0)
			send(clntSock, buf, bytes, 0);

		fclose(fp);
		close(clntSock);
	}

	return 0;
}
