#include<stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include<error.h>
#include<string.h>
#include<unistd.h>

#define _SIZE_ 1024
 //实现进程双向通信    使用可以双向通信的管道
int main()
{
	int fd[2];
	//socketpair   第一个参数  使用do_main域间套接字  第二个是使用面向字节流的服务（面向数据块SOCK_DGRAM）
	int ret = socketpair(PF_LOCAL,SOCK_STREAM,0,fd);
 
	//返回0表示成功
	if(ret!=0)
	{
		perror("socketpair()");
		return ret;
	}
	
	pid_t id=fork();
	if(id<0)
	{
		perror("fork()");
		return id;
	}
	else if(id==0)
	{
		//child process
		close(fd[0]);
		char buf[_SIZE_];
		int done=0;
		while(!done)
		{
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"ali94s");
			//子进程先写   父进程就会先打印出子进程写的内容
			write(fd[1],buf,sizeof(buf));
			read(fd[1],buf,sizeof(buf));
			sleep(1);
			printf("%s i am child\n",buf);
			fflush(stdout);
		}
	}
	else
	{
		//father process
		close(fd[1]);
		char buf[_SIZE_];
		while(1)
		{
			memset(buf,'\0',sizeof(buf));
			//父进程先读  读到子进程写的内容   完成进程间通信
			read(fd[0],buf,sizeof(buf));
			printf("%s i am father\n",buf);
			fflush(stdout);
			strcpy(buf,"yes,ali is yougy boy");
			write(fd[0],buf,sizeof(buf));
		}
	}
	return 0;
}
