#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
//#include<sys/>

const char *ip="127.0.0.1";
const short port=8080;
#define _SIZE_ 1024

void *running(void *arg)
{
	
}


int main()
{
	//printf("server...\n");
	//创建套接字  打开一个网络通讯端口
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		return 1;
	}

	//初始化本地套接字  并绑定ip和端口
	struct sockaddr_in server_socket;
	struct sockaddr_in client_socket;
	server_socket.sin_family=AF_INET;
	server_socket.sin_addr.s_addr=inet_addr(ip);
	server_socket.sin_port=htons(port);
	//绑定服务器ip和端口  
	if(bind(sock,(struct sockaddr*)&server_socket,sizeof(struct sockaddr_in))<0)
	{
		perror("bind");
		return 2;
	}
	
	//监听服务器端
	if(listen(sock,10)<0)
	{
		perror("listen");
		return 3;
	}

	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len=sizeof(peer);
		while(1)
		{
			int fd=accept(sock,(struct sockaddr*)&peer,&len);
			if(fd<0)
			{
				perror("accept");
				close(sock);
				return 4;
			}
			printf("new user:%s,%d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));

			//创建线程来各司其职
			pthread_t id;
			pthread_create(&id,NULL,running,NULL);


		//	pid_t id=fork();
		//	if(id==0)
		//	{
		//		while(1)
		//		{
		//			char buf[_SIZE_];
		//			memset(buf,'\0',sizeof(buf));
		//			size_t s=read(fd,buf,sizeof(buf)-1);
		//			if(s>0)
		//			{
		//				printf("client:#%s",buf);
		//				fflush(stdout);
		//				memset(buf,'\0',sizeof(buf));	
		//				//buf[strlen(buf)-1]='\0';
		//				//fgets(buf,sizeof(buf),stdin);
		//				//printf("server:#%s",buf);
		//				write(fd,buf,strlen(buf));
		//			}
		//			else
		//			{
		//				perror("read");
		//				return 5;
		//			}
		//		}
		//		exit(0);
		//	}
		//	else
		//	{
		//		pid_t ret=waitpid(-1,NULL,WNOHANG);
		//			
		//	}
		}
	}

	close(sock);
	return 0;
}
