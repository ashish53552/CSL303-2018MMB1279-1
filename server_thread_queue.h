#include <stdlib.h>

struct for_server_thread{
	int newsockfd;
	struct queue *schedule;
};

struct squeue{
	int f,r,c,s;
	struct for_server_thread ** arr;
};

struct squeue * sinit(int cap){
	struct squeue * q=(struct squeue *)malloc(sizeof(struct squeue ));
	q->f=0;
	q->r=-1;
	q->c=cap;
	q->s=0;
	q->arr=(struct for_server_thread **) malloc((q->c )* sizeof(struct for_server_thread *));
	return q;
}
void senque(struct for_server_thread * el,struct squeue * q){
	if((q->s>q->c)){
		return;
	}
	q->s++;
	q->r=(q->r+1)%(q->c);
	q->arr[q->r]=el;
}
struct for_server_thread * sdeque(struct squeue *q){
	struct for_server_thread *temp=q->arr[q->f];
	q->f=(q->f+1)%q->c;
	q->s--;
	return temp;
}
