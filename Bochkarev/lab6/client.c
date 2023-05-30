#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 512
#define ERROR -1
#define PORT 2222

int main() 
{
	int clSocket = 0;
	if ((clSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR)
	{
		perror("can't create socket for client: ");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in sAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(PORT);
	sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	printf("Enter your message: (enter 'exit' to end transmission)\n");

	char *buf = (char *) malloc(BUFFER_SIZE * sizeof(char));
	if (buf == NULL )
	{
		close(clSocket);
		perror("mem alloc faulure: ");
		exit(EXIT_FAILURE);
	}

	char *orig = (char *) malloc(BUFFER_SIZE * sizeof(char));
	if (orig == NULL )
	{
		close(clSocket);
		perror("mem alloc faulure: ");
		exit(EXIT_FAILURE);
	}

	int readCount = 0;

	while (1) 
	{
		fgets(buf, BUFFER_SIZE, stdin);
		strcpy(orig, buf);
		if(strcmp(buf, "exit\n") == 0)
		{
			break;
		}
		if (sendto(clSocket, buf, strlen(buf), 0, (struct sockaddr *) &sAddr, sizeof(sAddr)) == ERROR)
		{
			close(clSocket);
			free(buf);
			perror("send faulure: ");
			exit(EXIT_FAILURE);
		}
		readCount = recvfrom(clSocket, buf, BUFFER_SIZE, 0, NULL, NULL);
		if (readCount == ERROR)
		{
			close(clSocket);
			free(buf);
			perror("recieve faulure: ");
			exit(EXIT_FAILURE);
		}
		buf[readCount] = '\0';
		printf("Received: %s\n", buf);
		if(strcmp(buf, orig) != 0)
		{
			printf("Sorry, your data was not sent properly\n");
		}
	}
	close(clSocket);
	free(buf);
	return EXIT_SUCCESS;
}
