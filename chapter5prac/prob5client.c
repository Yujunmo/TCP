#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
#define LEN 4
void error_handling(char* message);

int main(int argc, char* argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	char str[BUF_SIZE-4];
	int str_len;
	int recv_cnt;
	int *len= NULL;
	
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

	// #1 send
	printf("send : ");
	fgets(str, sizeof(str), stdin);
	str[strlen(str)-1]='\0';
	str_len = strlen(str);
	printf("length : %d\n", str_len);
	len = (int*)message;
	*len = str_len;
	strcpy(message+4,str);
	printf("message : %s\n",message+4);
	write(sock,message,BUF_SIZE-1);
	
	// #2 read
	read(sock, &str_len, LEN);
	recv_cnt=0;
	
	while(recv_cnt<str_len){
		recv_cnt += read(sock,message+recv_cnt,BUF_SIZE-1);
	}
	printf("from server : %s\n",message);
	
	// #3 send
	printf("send : ");
	fgets(str, sizeof(str), stdin);
	str[strlen(str)-1]='\0';
	str_len = strlen(str);

	*len = str_len;
	strcpy(message+4, str);
	
	write(sock, message,BUF_SIZE-1);

	// #close
	close(sock);
	return 0;
}


void error_handling(char * message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
