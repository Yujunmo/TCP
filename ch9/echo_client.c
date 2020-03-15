#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;
	int recv_len;
	int recv_cnt;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("sock() error");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");
	else
		puts("connected ........");

	
	while(1){
		fputs("input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "q\n")|| !strcmp(message,"Q\n")){break;}
		str_len= write(sock, message, strlen(message));
		recv_len =0;
		
		while(recv_len<str_len){
			recv_cnt=read(sock, &message[recv_len], BUF_SIZE-1);
			if(recv_cnt==-1)
				error_handling("read() error!");
			recv_len += recv_cnt;
		}
		message[recv_len]=0;
		printf("message from server :  %s", message);
	}
	close(sock);
	return 0;
}

void error_handling(char * message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
