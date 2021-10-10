#include"configs.h"

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_t server_thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;





int main(){
	struct queue *tt=init(QUEUE_SIZE);
	printf("Inserting element till QUEUE SIZE....\n");
	for(int i=0;i<QUEUE_SIZE;i++){
		printf("Inserting element %d\n",i+1);
		struct request *temp=(struct request *)malloc(sizeof(struct request));
		enque(*temp,tt);
		free(temp);
		printf("QueueSize:%d QueueCap:%d\n",tt->s,tt->c);
	}
	struct request *temp=(struct request *)malloc(sizeof(struct request));
	printf("Inserting additional element as the Queue is FULL\n");
	if(tt->s<tt->c) {
		enque(*temp,tt);
	}
	printf("QueueSize:%d QueueCap:%d\n",tt->s,tt->c);

	((tt->s>tt->c)?printf("Error in Enque\n"):printf("Enque Check Passed\n"));
	printf("Deque test.....\n");
	printf("Deque till QUEUE_SIZE +1.....\n");
	printf("Output....\n");
	for(int i=0;i<QUEUE_SIZE;i++){
		//struct request *temp=(struct request *)malloc(sizeof(struct request));
		deque(tt);
		printf("QueueSize:%d QueueCap:%d\n",tt->s,tt->c);
	}
	if(tt->s>0) {
		deque(tt);
	}
	printf("QueueSize:%d QueueCap:%d\n",tt->s,tt->c);
	((tt->s<0)?printf("Error in Deque\n"):printf("Deque Check Passed\n"));
	printf("Testing for multiple inputs:\n");
	printf("DLL function test .......\n");
	printf("Input : wrong dll_name args: lib root 2\n");
	strcpy(temp->dll_name,"lib");
	strcpy(temp->func_name,"root");
	strcpy(temp->func_args[0],"2");
	printf("Output:\n");
	printf("%s",dll_function_invocation(temp));
	printf("Input : wrong func_name args: libm.so.6 root 2\n");
	strcpy(temp->dll_name,"libm.so.6");
	strcpy(temp->func_name,"root");
	strcpy(temp->func_args[0],"2");
	printf("Output:\n");
	printf("%s",dll_function_invocation(temp));
	printf("Input : less number of func args: libm.so.6 pow 2\n");
	strcpy(temp->dll_name,"libm.so.6");
	strcpy(temp->func_name,"pow");
	strcpy(temp->func_args[0],"2");
	printf("Output:\n");
	printf("%s",dll_function_invocation(temp));
	printf("Input : more number of func args: libm.so.6 sqrt 2 3\n");
	strcpy(temp->dll_name,"libm.so.6");
	strcpy(temp->func_name,"sqrt");
	strcpy(temp->func_args[0],"2");
	strcpy(temp->func_args[1],"3");
	printf("Output:\n");
	printf("%s",dll_function_invocation(temp));
	temp->func_args[1][0]='\0';
	printf("Input : correct parameters : libm.so.6 sqrt 2 3\n");
	strcpy(temp->dll_name,"libm.so.6");
	strcpy(temp->func_name,"sqrt");
	strcpy(temp->func_args[0],"2");
	printf("Output:\n");
	printf("%s",dll_function_invocation(temp));



	return 0;
}