#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<error.h>
#include<unistd.h>
#define _SIZE_ 1024
int main()
{

	int fd=open("./log",O_CREAT | O_RDWR,"664");
	if(fd<0)
	{
		perror("open()");
		return fd;
	}

	close(1);

	int ret=dup2(fd,1);

	char buf[_SIZE_];

	while(1)
	{
		memset(buf,'\0',sizeof(buf));

		ssize_t s=read(0,buf,sizeof(buf));
		if(s>0)
		{
			buf[s-1]='\0';
			if(strncmp("quit",buf,4)==0)
				break;
			printf("%s\n",buf);
			fflush(stdout);
		}
	}
	close(fd);
	return 0;
}
