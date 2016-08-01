/*************************************************************************
	> File Name: fork_and_vfork.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年07月23日 星期六 09时14分13秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int globvar = 5;
 
int main()
{
    pid_t pid;
    int var = 1;
    int i;
    
    printf("fork is different with vfork \n");

    /*pid = fork();*/
    pid = vfork();

    switch(pid) {

    case 0:
        i = 3;
        while(i-->0) {
            
            printf("Child process is running\n");
            globvar++;
            var++;
            sleep(2);
        }
        printf("Child's globvar = %d , var = %d \n", globvar , var );
        break;
    
    case -1:
        perror("Creation is fail\n");
        exit(0);

    default :
        i = 5;
        while(i-- > 0) {
            
            printf("father process is running\n");
            globvar++;
            var++;
            sleep(2);
        }
        printf("Parent's globvar = %d ,var = %d\n",globvar , var);
        exit(0);

    }
}
