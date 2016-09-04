/*************************************************************************
	> File Name: server.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月03日 星期三 15时48分48秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include"server_to_client_do.h"
#include<signal.h>
#include"my_struct.h"
#include"mysql.h"
#define LISTENQ 12





void pthread_son_do(int *conn_fd)
{
    int every_choice = 1;
    do_the_son_do(every_choice,*conn_fd);
}
void handler(int signal)
{
    printf("屏蔽失败!\n");
    return ;
}
int main()
{
    int sock_fd;
    int conn_fd;
    int optval;
    MYSQL *connect;
    

    struct sockaddr_in  cli_addr,ser_addr;

    connect = mysql_init(NULL);
    if(connect == NULL) {
        printf("初始化数据库失败!\n");
        return EXIT_FAILURE;
    }
    connect = mysql_real_connect(connect,"127.0.0.1","root","548946","talkroom",0,NULL,0);
    if(connect) {
        printf("连接成功!\n");
    }
    else {
        printf("连接失败!\n");
    }
    
    signal(SIGINT,handler);

    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd < 0) {
        printf("创建套接字失败!\n");
        exit(1);
    }
    if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int)) < 0 ) {
        printf("设置自主性失败!\n");
        exit(1);
    }
    memset(&ser_addr,0,sizeof(struct sockaddr_in));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(4500);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);//就是这个端口,就是这个地址要将来通信

    if(bind(sock_fd,(struct sockaddr *)&ser_addr,sizeof(struct sockaddr_in)) < 0) {
        printf("绑定失败!\n");
        exit(1);
    }
    if(listen(sock_fd,LISTENQ) < 0) {
        printf("监听失败!\n");
        exit(1);
    }
    while(1) {
        int len_cli_addr = sizeof(struct sockaddr_in);
        conn_fd = accept(sock_fd,(struct sockaddr *)&cli_addr,&len_cli_addr);
        if(conn_fd < 0) {
            printf("接收连接请求失败!\n");
            break;
        }
        
        pthread_t pthread_id;
        pthread_create(&pthread_id,NULL,(void*)pthread_son_do,&conn_fd);
        //pthread_join(pthread_id,NULL);
    }   
    close(sock_fd);
    return 0;
}
