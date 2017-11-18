/*************************************************************************
	> File Name: test_gtk_a_main.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月08日 星期一 08时41分55秒
 ************************************************************************/

#include<stdio.h>
#include<gtk/gtk.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include"my_window.h"
GtkWidget *window;
void noc()
{
    printf("不许屏蔽!\n");
}
int main(int argc,char *argv[])
{
    int conn_fd ;
    struct sockaddr_in  serv_addr;
    signal(SIGINT,noc);

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4500);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    
    if((conn_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
        printf("创建TCP套接字失败!\n");
        exit(1);
    }
    
    
    if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) < 0) {
        printf("连接请求失败!\n");
        exit(1);
    }

    gtk_init(&argc,&argv);
    my_window(conn_fd);
}
