#include<stdlib.h>
#define BUFFER_SIZE 256 /*to change the buffer size */
#define DATA_SIZE 2048 
struct request {
int  clientFd;
char dll_name[BUFFER_SIZE]; // Name of a dynamically loaded library
char func_name[BUFFER_SIZE]; // Name of a function to call from the DLL
char func_args[BUFFER_SIZE][BUFFER_SIZE]; // Arguments to pass to the function
};


struct queue{
	int f,r,c,s;
	struct request* arr;
};

struct queue * init(int cap){
	struct queue *q=(struct queue *)malloc(sizeof(struct queue ));
	q->f=0;
	q->r=-1;
	q->c=cap;
	q->s=0;
	q->arr=(struct request *) malloc((q->c )* sizeof(struct request));
	return q;
}
void enque(struct request el,struct queue *q){
	if((q->s>q->c)){
		return;
	}
	q->s++;
	q->r=(q->r+1)%(q->c);
	q->arr[q->r]=el;
}
struct request deque(struct queue *q){
	struct request temp=q->arr[q->f];
	q->f=(q->f+1)%q->c;
	q->s--;
	return temp;
}
