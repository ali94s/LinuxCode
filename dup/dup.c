#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<error.h>

#define _SIZE_ 1024
//  文件描述符的重定向   dup
int main()
{
	//  若打开的文件不存在则创建 一可读写的方式打开
	int fd = open("./log",O_CREAT | O_RDWR,"664");
	if(fd<0)
	{
		perror("open()");
		return fd;
	}

	close(1);
	int new_fd=dup(fd);
	if(new_fd==-1)
	{
		perror("dup()");
		return -1;
	}
	close(fd);
	char buf[_SIZE_];
	while(1)
	{
		memset(buf,'\0',sizeof(buf));

		ssize_t s=read(0,buf,sizeof(buf));
		//fgets(buf,sizeof(buf),stdin);
		if(s>0)
		{
			if(strncmp("quit",buf,4)==0)
				break;
			printf("%s",buf);
			fflush(stdout);
		}
	}
	close(new_fd);
	return 0;
}
