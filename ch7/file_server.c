#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
#define NUM 1

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}


int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;

       	char message[BUF_SIZE];
        int str_len, i;
	int read_cnt;
	
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

	if(listen(serv_sock,NUM)==-1)
		error_handling("listen() error");

	clnt_addr_size=sizeof(clnt_addr);

	FILE* fp;
	char buf[BUF_SIZE];
	fp = fopen("example.c","rb");
	if(fp==NULL){
		printf("file open failure");
		return 0;
	}
		

	for(i=0;i<NUM;i++){
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client %d\n", i+1);

		//fill in 
		while(1){
			read_cnt= fread((void*)buf,1,BUF_SIZE,fp);
			if(read_cnt < BUF_SIZE){
				write(clnt_sock,buf,read_cnt);				
				break;
			}
			write(clnt_sock,buf,read_cnt);				
		}
		shutdown(clnt_sock, SHUT_WR);
		read(clnt_sock, buf,BUF_SIZE);
		printf("message from client : %s\n", buf);
		fclose(fp);
		close(clnt_sock);
        }
	close(serv_sock);
	return 0;
}
