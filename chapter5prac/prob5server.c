#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
#define LEN 4
void error_handling(char* message);

int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;

	int i, str_len, recv_cnt;
	int *len= NULL;
	char message[BUF_SIZE];
	char str[BUF_SIZE-4];

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port= htons(atoi(argv[1]));
		
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock,3)==-1)
		error_handling("listen() error");

	clnt_addr_size=sizeof(clnt_addr);

	for(i=0;i<5;++i){
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("connected client %d\n",i+1);
		
		// #1 read
		read(clnt_sock, &str_len, LEN);
		recv_cnt=0;
		while(recv_cnt < str_len){
			recv_cnt += read(clnt_sock, message+recv_cnt, BUF_SIZE-1);
		}
		printf("from client : %s\n", message);

		// #2 send
		printf("send : ");
		fgets(str, sizeof(str), stdin);
		str[strlen(str)-1]='\0';
		str_len = strlen(str);
		len=(int*)message; //this was the problem
		*len = str_len;
		strcpy(message+4,str);

		write(clnt_sock, message, BUF_SIZE-1);

		// #3 read
		read(clnt_sock, &str_len, LEN);
		recv_cnt=0;
		while(recv_cnt<str_len){
			recv_cnt += read(clnt_sock, message+recv_cnt, BUF_SIZE-1);
		}
		printf("from client : %s\n", message);

		// #close
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}


