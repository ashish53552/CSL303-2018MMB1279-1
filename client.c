#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


#define BUFFER_SIZE 256
#define DATA_SIZE 2048
#define MAX_CONNECTIONS 1
#define QUEUE_SIZE 100
#define THREAD_POOL_SIZE 20

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

struct request {
	char dll_name[256]; // Name of a dynamically loaded library
	char func_name[256]; // Name of a function to call from the DLL
	char func_args[256][256]; // Arguments to pass to the function
};


int main(int argc,char *argv[])
{   
	/* args port | dll_name | func_name | func_args 
	*/
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	struct request data;
	char buffer[BUFFER_SIZE];
	if(argc<4){
		fprintf(stderr,"Usage <portNo.> <dll_name> <func_name> <func_args>\n");
		exit(1);
	}
	portno=atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server=gethostbyname("localhost");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		 (char *)&serv_addr.sin_addr.s_addr,
		 server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	
	char s[256];
	sprintf(s,"%d",argc-2);
	int st=0;
	for(int i=0;i<argc-2;i++){
	   if(st==BUFFER_SIZE){
		   printf("Bandwidth capacity reached please increase");
		   exit(1);
	   }
	   int j=0;
	   while(argv[i+2][j]!='\0'){
		   buffer[st+j]=argv[i+2][j];
		   j++;
	   } 
	   st+=j;
	   buffer[st]='^';
	   st++;
	}
	buffer[st]='\0';	
	n = write(sockfd,buffer,BUFFER_SIZE);
	bzero(buffer,BUFFER_SIZE);
	if (n < 0) {
		error("ERROR writing to socket");
		exit(1);
	}
	bzero(buffer,BUFFER_SIZE);
	while((n = read(sockfd,buffer,BUFFER_SIZE-1))>0){
		printf("%s\n",buffer);
		if(buffer[n-1]=='\0') break;
		bzero(buffer,BUFFER_SIZE);
	}
	bzero(buffer,BUFFER_SIZE);
	while((n = read(sockfd,buffer,BUFFER_SIZE-1))>0){
		printf("%s",buffer);
		if(buffer[n-1]=='\n' || buffer[n-1]=='\0'|| buffer[n-1]=='\r') break;
		bzero(buffer,BUFFER_SIZE);
	}
	close(sockfd);
	return 0;
}
