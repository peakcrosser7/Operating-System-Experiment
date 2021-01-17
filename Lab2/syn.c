//Linux父子进程同步
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>

int main(){
	pid_t pid,p;
	int status,i;
	pid=fork();
	if(pid==0){	//子进程
		printf("This is Child Process\n");
		sleep(5);	//休眠5s结束
		printf("Child Process will stop\n");
		exit(6);	//结束进程
	}
	else if(pid>0){
		p=wait(&status);	//等待子进程结束，返回子进程进程号
		i=WEXITSTATUS(status);	//分析子进程返回信息
		printf("This is Parent Process\nChild's pid=%d exit status=%d\n",pid,i);
	}
	return 0;
}
