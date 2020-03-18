#include<stdio.h>
#include<unistd.h>
#include<signal.h>


/*
EXPLANATION
1. main function registers signals. 
2. when the signal is occured, corresponding signal handler will be
automatically called by OS.
3. signal handler can't be called while the process is blocked.
4. if the process is blocked, it is waken up (by OS?) upon SIGNAL is detected.
	so that signal handler can be successfully called. 

summary : process and os interaction. os can call function for the process
and vice versa. 
*/

// signal handler #1
void timeout(int sig){
	if(sig==SIGALRM)	puts("time out");
	alarm(2);
}

// signal handler #2
void keycontrol(int sig){
	if(sig==SIGINT)	puts("ctrl + c pressed");
}

int main(int argc, char* argv[]){
	int i;
	
	//signal register function  : registering singal 
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);

	alarm(2);
	
	for(i =0 ; i <3 ; ++i){
		puts("wait ...");
		sleep(100);
	}
	
	return 0;
}


