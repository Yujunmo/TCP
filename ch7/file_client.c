#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char * message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}


int main(int argc, char* argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
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

	// fill in 
	FILE *fp = fopen("fromSever.c","wb");
	if(fp==NULL){
		printf("file open failure");
		return 0;
	}
	sleep(5);
	while((recv_cnt=read(sock, message, BUF_SIZE)) != 0){
		fwrite((void*)message, 1,recv_cnt,fp);
	}
	write(sock,"thanks",7);
	fclose(fp);
	close(sock);
	close(sock);
	return 0;
}
