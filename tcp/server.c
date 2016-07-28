#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/wait.h>

const char *ip="127.0.0.1";
const short port=8080;
#define _SIZE_ 1024

void *running(void* arg)
{
	int fd=(int)arg;
	char buf[_SIZE_];
	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		int s=read(fd,buf,sizeof(buf)-1);
		if(s>0)
		{
			buf[s-1]='\0';
			printf("client#%s\n",buf);
			fflush(stdout);
			write(fd,buf,s);
		}
		else
		{
			perror("read");
			return NULL;
		}
	}
	return (void *)0;
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
	
//	int opt=1;
//	socklen_t optlen=sizeof(opt);
//	//不经历TIME_WAIT时间
//	setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,&opt,optlen);

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
		close(socket);
		return 2;
	}
	
	//监听服务器端
	if(listen(sock,10)<0)
	{
		perror("listen");
		close(socket);
		return 3;
	}

	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len=sizeof(peer);
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
		pthread_create(&id,NULL,running,(void *)fd);
		pthread_detach(id);
	}

	close(sock);
	return 0;
}
