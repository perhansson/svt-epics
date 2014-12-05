#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <libxml/parser.h>
#include "client_util.h"



void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    const unsigned int BUFFER_SIZE = 256;
    //const unsigned int XML_BUFFERS = 200;
    //char buffer[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    //char* xml_buffer[XML_BUFFERS];
    int j;
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
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    int ans = 0;
    char* xml_buf;// = NULL;
    //while(strcmp(ans,"-1")) {
    while(ans!=-1) {
      printf("What to do?\n");
      bzero(buffer,BUFFER_SIZE);
      fgets(buffer,BUFFER_SIZE-1,stdin);
      ans = atoi(buffer);
      printf("Option: %d\n",ans);
      if (ans == -1) {
	printf("quit\n");
	return 0;
      }
      else if (ans == 1) { 
	//xml_buf = NULL;
	n = 0;
	xml_buf = read_xml(&sockfd,&n);
	if(xml_buf!=NULL) {
	  findTemp(xml_buf,n);
	  free(xml_buf);
	}
	continue;
      }
      else if(ans == 2) {
	printf("Write something to tcp/ip port:\n");
	bzero(buffer,BUFFER_SIZE);
	fgets(buffer,BUFFER_SIZE-1,stdin);
	printf("Got \"%s\"\n",buffer);
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) 
	  error("ERROR writing to socket");
	n = read(sockfd,buffer,BUFFER_SIZE-1);
	if (n < 0) 
	  error("ERROR reading from socket");
	printf("reply: \"%s\"\n",buffer);
	continue;
      }
      else {
	printf("try again\n");
	continue;
      }
    }
    close(sockfd);
    return 0;
    
    printf("Please enter the message: ");
    bzero(buffer,BUFFER_SIZE);
    fgets(buffer,BUFFER_SIZE-1,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,BUFFER_SIZE);
    n = read(sockfd,buffer,BUFFER_SIZE-1);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
