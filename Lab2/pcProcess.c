//Linux下创建父子进程
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int ct,pt;	//设置父子进程休眠时间
int main(int argc, char** argv){
	pid_t pid;
	switch(argv[1][0]){
	case '0':
		printf("\n普通情况：\n");
		ct=pt=5;
		break;
	case '1':
		printf("\n父进程先结束：\n");
		pt=2,ct=8;
		break;
	case '2':
		printf("\n子进程先结束：\n");
		pt=8,ct=2;
		break;

	}
	pid=fork();
	if(pid==0) {
		sleep(ct);
		printf("\nChild Process: pid:%d ppid:%d\n",
			getpid(),getppid());	//输出进程id和父进程id
	}
	else if(pid>0) {
		sleep(pt);
		printf("\nParent Process: pid:%d ppid:%d\n",
			getpid(),getppid());
	}
	return 0;	
}
