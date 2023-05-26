#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERROR -1
#define BUFFER_SIZE 512
#define PORT 2222

int main() 
{
    int sSocket = 0;
    if ((sSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) 
    {
        perror("socket failed: "); 
    	exit(EXIT_FAILURE);  
    }

    struct sockaddr_in sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(PORT);
    sAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sSocket, (struct sockaddr *) &sAddr, sizeof(sAddr)) == ERROR) 
    {
        close(sSocket);
        perror("binding error: "); 
    	exit(EXIT_FAILURE);  
    }

    printf("data waiting...\n");

    unsigned int addrLen = 0;
    int readCount = 0;

    struct sockaddr_in clAddr;

    char *buf = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if (buf == NULL) 
    {
        close(sSocket);
        perror("alloc error (server): "); 
    	exit(EXIT_FAILURE);  
    }

    while (1) 
    {
        addrLen = sizeof(clAddr);
        readCount = recvfrom(sSocket, buf, BUFFER_SIZE, 0, (struct sockaddr *) &clAddr, &addrLen);

        if (readCount == ERROR) 
        {
            close(sSocket);
            free(buf);
            perror("recieve failure: "); 
    		exit(EXIT_FAILURE);  
        }
        buf[readCount] = '\0';

        printf("Received: %s\n", buf);

        if (sendto(sSocket, buf, readCount, 0, (struct sockaddr *) &clAddr, addrLen) == ERROR) 
        {
            close(sSocket);
            free(buf);
            perror("send failure: "); 
    		exit(EXIT_FAILURE);  
        }
    }

    close(sSocket);
    free(buf);

    return EXIT_SUCCESS;
}
