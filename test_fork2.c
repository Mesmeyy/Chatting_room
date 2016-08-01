/*************************************************************************
	> File Name: test_fork2.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年07月23日 星期六 08时58分37秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    pid_t pid;
    
    pid = fork();

    switch( pid ){

    case 0  :
        while( 1 ){
            
            printf( "A background process , PID : %d \n , ParentID : %d \n" , getpid() , getppid() ) ;
            sleep(3);
        }

    case -1 :
        perror( "Process creation failed \n " ) ;
        exit(1);

    default :
        printf( "I am parent process , my pid is : %d \n" ,getpid()) ;
        exit(0);
    } 

    exit(0);
}
