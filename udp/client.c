#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define _SIZE_ 1024

void print(const char *argv)
{
	printf("Usage: %s [IP][PORT]\n",argv);
}

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		print(argv[0]);
	}

	int sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0)
	{
		perror("socket()");
		return sock;
	}

	struct sockaddr_in remote;
	remote.sin_family=AF_INET;
	remote.sin_addr.s_addr=inet_addr(argv[1]);
	remote.sin_port=htons(atoi(argv[2]));

	socklen_t len = sizeof(remote);

	char buf[_SIZE_];

	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		printf("Please Enter:");
		fflush(stdout);
		int size=read(0,buf,sizeof(buf)-1);
		if(size > 0)
		{
			buf[size-1]='\0';
			sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&remote,len);

			if(strncmp("quit",buf,4)==0)
				break;

			memset(buf,'\0',sizeof(buf));
			ssize_t ret=recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr *)&remote,&len);

			if(ret<0)
			{
				perror("recvfrom()");
				return ret;
			}
			printf("server:%s\n",buf);
			fflush(stdout);
		}
		else
		{
			perror("read()");
			return size;
		}
	}
	close(sock);
	return 0;
}
