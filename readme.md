
--------------------------------------------------------------------------------------
File structure
--------------------------------------------------------------------------------------
- client.c  /*client file used to send the request*/
- 
- request_listener.c /*server file*/
- 
- configs.h  /*a header file containing all directives*/
- 
- dispatcher_queue.h /*queue used for thread pooling for dispatcher*/
- 
- server_thread_queue.h /*queue used for thread pooling in server*/
- 
- dll_loader.h /*contains all the functions required for the dll to execute*/
- 
- unit_test.c /*testing*/
--------------------------------------------------------------------------------------
Compile (Please refer to examples):
1) Open 2 separate terminals in linux in the same folder containing the above mentioned files
2) In one terminal (for server):

Example 1:
gcc -o listener request_listener.c -lpthread -ldl
./listener 5546

compile(general):
gcc -o listener request_listener.c -lpthread -ldl
./listener <portNo.>
/*./listener for usage*/
 
3) In the other terminal(client):

Example 2:
gcc -o client client.c
./client 5546 libm.so.6 pow 2 3

Compile(general):
gcc -o client client.c
./client <portNo.> <dll_name> <func_name> <func_args>
/*run ./client for usage*/
Running the above Example 1 and Example 2 in succession 
will return the following output at the console:
------------------------------------------------------------------------
Successfully Enqueued
8.000000
------------------------------------------------------------------------

Explanation:

Successfully Enqueued /*The request has been Enqueued by the dispatcher*/
8.000000 /*Return value of request pow 2 3 (2 raised to power 3)*/


*Please refer to design_document.txt for understanding the code*
*In some cases if the output doesnt match please check whether (math.h)"libm.so.6" exists at "/lib/x86_64-linux-gnu/"
*To change the path edit PATH in dll_loader.h and HEADER_FILE in dll_loader.h 
------------------------------------------------------------------------
Functionalities:
-------------------------------------------------------------------------

- THREAD POOL LIMIT
	The thread pool limit (Number of threads) can be changed in configs.h
	Look for THREAD_POOL_SIZE in configs.h

- QUEUE SIZE LIMIT
	Limits the size of queue both for the socket server thread pool as well as dispatcher queue
	Look for QUEUE_SIZE in configs.h

- MAX CONNECTIONS
	Limits the max connections to be held by the socker server
	Look for MAX_CONNECTIONS in configs.h

- FILE SIZE LIMIT IN MB
	Limits the total File size for all the threads 
	Look for MEMORY_LIMIT_IN_MB in configs.h

- ADD/REMOVE DISPATCHER
	A boolean which can be used to turn on and off the dispatcher(If off it will only enque request will be done)
	Look for SET_DISPATCHER in configs.h

- BUFFER LIMIT
	Limits the BUFFER size for the data packet
	Look for BUFFER_SIZE in dispatcher_queue.h

- TOTAL FILE DESCRIPTOR LIMIT
	Limits the file descriptors in total which can open.
	Look for OPEN_FILE_FD in configs.h

- ALLOW FILE DESCRIPTOR LIMIT
	A boolean which can be used to turn on and off the file descriptor limit
	Look for SET_TOTAL_FD_LIMIT in configs.h

------------------------------------------------------
Testing
------------------------------------------------------
compile:
gcc -o unit_test unit_test.c -lpthread -ldl
./unit_test
/*for output details refer to design document*/

----------------------------------------------------------------------
The compilation assumes the following header files are already present
----------------------------------------------------------------------
	<stdio.h>
	<stdlib.h>
	<string.h>
	<unistd.h>
	<sys/types.h> 
	<sys/socket.h>
	<netinet/in.h>
	<limits.h>
	<pthread.h>
	<sys/resource.h>
	<errno.h>
	<ulimit.h>

-------------------------------------------------------------------------------------------------------



