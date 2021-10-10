#include"configs.h"


pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_t server_thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
/*
struct sockaddr_in
{
short   sin_family;  
u_short sin_port;
struct  in_addr sin_addr;
char    sin_zero[8]; 
};
*/


void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int do_stuff(int newsockfd,struct queue *schedule){
	/* Function which deconstructs the provided string */
	char data[DATA_SIZE];
	char buffer[BUFFER_SIZE];
	int n;
	bzero(data,DATA_SIZE);
	//a blocking call which waits till all the BUFFER_SIZE-1 bytes are read if they are not it blocks
	while((n = read(newsockfd,buffer,BUFFER_SIZE-1))>0){
		strcat(data,buffer);
		if(buffer[n-1]=='\0') break;
		bzero(buffer,BUFFER_SIZE);
	}
//	printf("%s\n",data);
	int st=0;
	int end=0;
	int j=1;
	struct request item;
	item.clientFd=newsockfd;
	while(data[end]!='\0' && j!=3){
		if(data[end]=='^'){
		char temp[256];
		if(j==1){
			int k=0;
			for(int i=st;i<end;i++){
				temp[k++]=data[i];
			}
			temp[k]='\0';
			st=end+1;
			strcpy(item.dll_name,temp);
		}
		if(j==2){
			int k=0;
			char temp[256];
			for(int i=st;i<end;i++){
				temp[k++]=data[i];
			}
			temp[k]='\0';
			st=end+1;
			strcpy(item.func_name,temp);
		}
		j++;
		}
		end++;

	}
	int r=0;
	while(data[end]!='\0' && j==3){
		if(data[end]=='^'){
			char temp[256];
			int k=0;
			for(int i=st;i<end;i++){
				temp[k++]=data[i];
			}
			temp[k]='\0';
			st=end+1;
			strcpy(item.func_args[r++],temp);
		}
		end++;
	}
	if(schedule->s<schedule->c){
		n=write(newsockfd,"Successfully Enqueued",22);
		enque(item,schedule);
		return 1;
	}
	n=write(newsockfd,"Queue Limit Reached",20);
	return 0;
}

void secondary_worker(struct request item){ 
	char *str=dll_function_invocation(&item);
	int n=write(item.clientFd,str,strlen(str));
	printf("%s",str);
	close(item.clientFd);
	return ;
};

void * worker_thread(void *p_schedule){
	struct queue* schedule=(struct queue*)p_schedule;
	while(1){
		pthread_mutex_lock(&mutex);
		if(schedule->s>0){
			secondary_worker(deque(schedule));
		}
		pthread_mutex_unlock(&mutex);
	}
}
void * server_worker_thread(void * arg){
	struct squeue *sschedule=(struct squeue*)arg;
	while(1){
			pthread_mutex_lock(&mutex);
			if(sschedule->s>0){
				struct for_server_thread *item=sdeque(sschedule);
				do_stuff(item->newsockfd,item->schedule);
			}
			pthread_mutex_unlock(&mutex);
	}
}
void mem_manager(){
  	struct rlimit limit,fd_limit,l2;
	limit.rlim_cur = 1024*1024*MEMORY_LIMIT_IN_MB;
	limit.rlim_max = 1024*1024*MEMORY_LIMIT_IN_MB;
	fd_limit.rlim_cur=OPEN_FILE_FD;
	fd_limit.rlim_max=OPEN_FILE_FD;
	if (setrlimit(RLIMIT_FSIZE, &limit) != 0) {
		printf("setrlimit() failed with errno=%d\n", errno);
		exit(1);
	}
	if(SET_TOTAL_FD_LIMIT){
			if (setrlimit(RLIMIT_NOFILE, &fd_limit) != 0) {
			printf("setrlimit() failed with errno=%d\n", errno);
			exit(1);
		}
	}
	

}
void dispatcher(struct queue *schedule){
	for(int i=0;i<THREAD_POOL_SIZE;i++){
		pthread_create(&thread_pool[i],NULL,worker_thread,(void *)schedule);
	}
}
int main(int argc, char *argv[])
{
	mem_manager();
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct queue *schedule=init(QUEUE_SIZE);
	struct squeue *sschedule=sinit(QUEUE_SIZE);
	// printf("%d\n",sschedule->s);
	struct sockaddr_in serv_addr, cli_addr;
	if (argc < 2) {
		fprintf(stderr,"Usage [port_name] \n");
		exit(1);
	}
		
	for(int i=0;i<THREAD_POOL_SIZE;i++){
		pthread_create(&server_thread_pool[i],NULL,server_worker_thread,(void *)sschedule);
	}
	if(SET_DISPATCHER){
		dispatcher(schedule);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
	 	error("ERROR on binding");
	}



	while(1){ 
		printf("Waiting for connections on port %d ....\n",portno);
		listen(sockfd,MAX_CONNECTIONS);

		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0){
			error("ERROR on accept");
		}	
		printf("Connected to socket fd %d\n",newsockfd);
		pthread_mutex_lock(&mutex); 
		struct for_server_thread* temp= (struct for_server_thread*) malloc(sizeof(struct for_server_thread));
		temp->newsockfd=newsockfd;
		temp->schedule=schedule;
		if(sschedule->s<sschedule->c){
			senque(temp,sschedule);
		}else{
			printf("Queue Cap Reached\n");
			int n=write(newsockfd,"Queue Cap reached aborting...\nIncrease cap and try again...\n",61);
			if(n<0){
				error("ERROR on writing to socket");
			}
		}
		pthread_mutex_unlock(&mutex);
		
	}

	close(sockfd);
	return 0; 
}
