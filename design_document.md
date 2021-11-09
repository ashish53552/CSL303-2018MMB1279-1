---------------------------------------------------------------------------------------------------------
This is a multithreaded dispatcher which mimics the process management by dispatcher
in Operating System
---------------------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------------------
The program is a collection of the following files
---------------------------------------------------------------------------------------------------------
/*Design logic is provided at the bottom it is also suggested to go through the files
for better understanding*/

-request_listener.c
	The following functions are used:
	
	void dispatcher(struct queue *schedule)
			->summons worker threads to execute the dlls

	void * worker_thread(void *p_schedule)
			->The worker threads for dispatcher

	void * server_worker_thread(void * arg)
			-> To implement the multithreaded socket server

	void mem_manager()
			-> sets the resource limit

	int do_stuff(int newsockfd,struct queue *schedule);
			->Deconstructs the data sent to the socket servers to create the struct request and 
			enques into the dispatcher queue;

	void secondary_worker(struct request item)
			->Calls the dll using dll_function_invocation after deque operation from the dispatcher

	int main(int argc, char *argv[])
			-> Contains main socket server code

-dll_loader.c

	double dll_function_invocation(struct request *temp)
		-> calls the dll using the arguments sent by client

	
-client.c

	This is a file that gives the client(user) to send the request data to the user
	The following arguments are required
	port | dll_name | func_name | func_args 
	All the above mentioned arguments must be space separated
	Currently the library supported is (libm.so.6) and functions pow,ceil,sqrt,sin,cos
	sending wrong inputs will display the desired error.

-configs.h 

	->includes all header files for better readability

-dispatcher_queue.h 

	-> contains a queue datastructure for dispatcher

-server_thread_queue.h 

	-> contains a queue datastrcture for server threads

-unit_test.c 

	-> Tests the queue for enque and deque operations by inserting more elements than queue capacity
	-> Tests the dll function library by providing
		->wrong dll names
		->wrong func names
		->wrong func arguments
	->Threading code can be tested by setting the QUEUE_SIZE to 1 
	and using a bash script
	->If the queue capacity is reached it will throw Queue Capacity reached to the client and logs 
	other errors to the server console.
	->If the file descriptor and size limit is reached the server
	will reset the connection and logs will be displayed on the server console.

---------------------------------------------------------------------------------------------------------
Design Logic
---------------------------------------------------------------------------------------------------------

When the client enters the data into the console all the arguments are converted into a single
string of data which to be further received by the sever

As soon as the connection is successful the server's multithread enqueues the client socket 
file descriptor into the sschedule (name of queue) 

The server_worker_thread function looks for sschedule to not be empty
it executes the do_stuff function which uses the client socket fd to read the byte stream
written by the client on the socket, which is further deconstructed to be converted into suitable
struct request and is enqueued into another queue(named as schedule)

This queue(schedule) is used by the dispatcher.

The threads summoned by the dispatcher look for the data in queue(schedule) and after 
acquiring mutex lock they call the secondary_worker function and deque the struct request 
from queue(schedule)																																																																																																																						 unlocking mutex.

Similar thread pooling is done for handling multiple connections to the server.

The secondary_worker function takes the struct request item and calls the function 
dll_function_invocation from the dll_loader.c to execute the dll and returns the 
result to the client. With suitable error handling.
