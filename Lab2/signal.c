//Linux下利用信号机制进程通信
#include<signal.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

void stop(int signum){
	printf("Bye, world!\n");
	exit(0);
}

int main(){
	pid_t pid;
	pid =fork();
	if(pid==0){
		signal(SIGUSR1,stop);
		while(1){
			printf("I'm Child Process, alive!\n");
			sleep(2);	
		}	
	}
	else if(pid>0){
		while(1){
			char op;
			printf("To terminate Child Process. Yes or No?\n");
			scanf("%c",&op);
			if(op=='Y'||op=='y'){
				kill(pid,SIGUSR1);
				wait();
				break;
			}
			sleep(2);
		}
		printf("Parent Process will stop.\n");
	}
	return 0;
}
