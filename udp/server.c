#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

#define _SIZE_ 1024

//udp服务器
void print(const char *argv)
{
	printf("Usage:%s [IP][PORT]\n",argv);
}

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		print(argv[0]);
		return -1;
	}
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0)
	{
		perror("socket()");
		return sock;
	}
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(argv[2]));
	local.sin_addr.s_addr=inet_addr(argv[1]);

	socklen_t addrlen=sizeof(local);
	//
	if(bind(sock,(struct sockaddr*)&local,addrlen)<0)
	{
		perror("bind()");
		return 1;
	}
	
	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	char buf[_SIZE_];

	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		//本函数用于从（已连接）套接口上接收数据，并捕获数据发送源的地址。
		ssize_t ret=recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr *)&client,&len);
		if(ret>0)
		{
			printf("client:%s\n",buf);
			fflush(stdout);

			sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&client,len);
		}
		else
		{
			perror("recvfrom()");
			return ret;
		}
	}
	close(sock);
	return 0;
}
