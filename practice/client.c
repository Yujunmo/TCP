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
	char message1[BUF_SIZE];
	char message2[BUF_SIZE];
	char message3[BUF_SIZE];
	int str_len, i;
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
	
	printf("1st : ");
	scanf("%s",message1);
	int len= strlen(message1);
	printf("%d",len);
	printf("2nd : ");
	scanf("%s",message2);
	printf("3rd : ");
	scanf("%s",message3);
	
	write(sock,message1,strlen(message1));
	//write(sock,message2,strlen(message2));
	//write(sock,message3,strlen(message3));

	//end of content

	close(sock);
	return 0;
}
