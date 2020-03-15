#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

//this headerfile is neccessary
#include<netinet/tcp.h>


void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	int nagle,state;
	int len;
	int opt_val=1;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	len = sizeof(nagle);
	state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&nagle, &len);
	if(state==-1) error_handling("getsockopt() error!");

	if(nagle==0) printf("nagle is on\n");
	if(nagle==1) printf("nagle is off\n");
	
	//after nagle is set "off"
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));
	state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&nagle, &len);
	if(nagle==0) printf("nagle is on\n");
	if(nagle==1) printf("nagle is off\n");

	return 0;
}

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
