#include"dispatcher_queue.h"
#include"server_thread_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#define PATH "/lib/x86_64-linux-gnu/"
#define HEADER_FILE "libm.so.6"


int check_validity(int i,int t){
	if(i<t){
		return -1;
	}if(i>t) return 1;
	if(i==t) return 0;
}
char *dll_throw(int i,int t){
    check_validity(i,t);
    if(check_validity(i,t)<0){
        return "Too many arguments\n";
    };
    if(check_validity(i,t)>0){
        return "Few arguments\n";
    }
    return "1";
}
char * dll_function_invocation(struct request *temp){
	int t=0;
	while(temp->func_args[t][0]!='\0'){
		t++;
	}
    int i=0;
	void *handle;
	void *cosine;
	char *str=(char *) malloc(BUFFER_SIZE*sizeof(char));
	char *error;
	strcpy(str,PATH);
	strcat(str,temp->dll_name);
	if(strcmp(temp->dll_name,HEADER_FILE)!=0){
       return "DLL does not exist \nPlease create the suitable code in dll_loader.h\nOnly libm.so.6 is supported\n";
	}
	double val=__INT64_MAX__;
	handle = dlopen (str, RTLD_LAZY);
	if (!handle) {
		fputs (dlerror(), stderr);
		exit(1);
	}
	if(strcmp(temp->func_name,"pow")==0){
        i=2;
		if(strcmp(dll_throw(i,t),"1")!=0){
            return dll_throw(i,t);
        };

		double (*opfun)(double,double);
		opfun=dlsym(handle,temp->func_name);
		val=opfun(atof(temp->func_args[0]),atof(temp->func_args[1]));
	}else if(strcmp(temp->func_name,"cos")==0){

        i=1;
		if(strcmp(dll_throw(i,t),"1")!=0){
            return dll_throw(i,t);
        };

		double (*opfun)(double);
		opfun=dlsym(handle,temp->func_name);
		val=opfun(atof(temp->func_args[0]));
	}else if(strcmp(temp->func_name,"sin")==0){

        i=1;
		if(strcmp(dll_throw(i,t),"1")!=0){
            return dll_throw(i,t);
        };

		double (*opfun)(double);
		opfun=dlsym(handle,temp->func_name);
		val=opfun(atof(temp->func_args[0]));
	}else if(strcmp(temp->func_name,"sqrt")==0){

		i=1;
		if(strcmp(dll_throw(i,t),"1")!=0){
            return dll_throw(i,t);
        };

		double (*opfun)(double);
		opfun=dlsym(handle,temp->func_name);
		val=opfun(atof(temp->func_args[0]));
	}else if(strcmp(temp->func_name,"ceil")==0){

		i=1;
		if(strcmp(dll_throw(i,t),"1")!=0){
            return dll_throw(i,t);
        };

		double (*opfun)(double);
		opfun=dlsym(handle,temp->func_name);
		val=opfun(atof(temp->func_args[0]));
	}else{
		return "Supported functions are [pow] [sin] [cos] [sqrt] [ceil]\n";
	}
	dlclose(handle);
    sprintf(str, "%f", val);
    strcat(str,"\n");
	return str;
}