/*************************************************************************
	> File Name: choice.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月01日 星期一 16时18分11秒
 ************************************************************************/

#include<stdio.h>
#include<gtk/gtk.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include"my_choice.h"
#include"my_man_to_man.h"
#include"my_men_to_men.h"
GtkWidget *window;
int choi_conn_fd ;
char recvbuffer[256];
void recvfun(void);
void  on_button_back(GtkWidget *button,gpointer data)
{

    send(choi_conn_fd,"bye",4,0);
    gtk_widget_destroy(window);
    exit(0);
    
}
void do_button1(GtkWidget *button1,gpointer data)
{
    //printf("!!!!!!!on button1!\n");
    //gtk_main_quit();
    
    my_man_to_man(choi_conn_fd);
}
void do_button2(GtkWidget *button2,gpointer data)
{
    //printf("#####on button2!\n");
    //gtk_main_quit();
    send(choi_conn_fd,"qun",3,0);//给服务器发送群聊标记记住这是群聊
    my_men_to_men(choi_conn_fd);
}
void do_button3(GtkWidget *button3,gpointer data)
{
    printf("@@@@@on button3!\n");
}
void recvfun()
{
    int i = 0;
    int locat = 0;
    int fd = 0;
    char recv_buffer[512];
    while(1) {
        memset(recv_buffer,0,512);
        usleep(100);
        recv(choi_conn_fd,recv_buffer,strlen(recv_buffer),0);
        if(strncmp(recv_buffer,"H:",2) == 0) {
            char neirong[512];
            char xingming[32];
            int k = 0;
            int j = 0;
            memset(neirong,0,512);
            for(i = 0;recv_buffer[i] != '\0';i++) {
                if(recv_buffer[i] == '/') {
                  locat = i;  
                }
            }
            for(i = locat+1;recv_buffer[i] != '\0';i++) {
                neirong[k++] = recv_buffer[i];
            }
            for(i = 2;i<locat;i++) {
                xingming[j++] = recv_buffer[i];
            }
            printf("neirong:%s\n",neirong);
            char history_file_name[128];
            memset(history_file_name,0,128);
            strcpy(history_file_name,"/home/zhuziyu/C_language/test/client/");
            strcat(history_file_name,xingming);
            strcat(history_file_name,".t");
            
            if((fd = open(history_file_name,O_CREAT|O_APPEND|O_RDWR),0664) < 0) {
                printf("建立文件失败!\n");
            }
            write(fd,neirong,strlen(neirong));
            close(fd);
            continue;
        }
        else {
            break;
        }
    }
    pthread_exit(0);
}
int my_choice(int conn_fd)
{
    pthread_t tid;
    pthread_create(&tid,NULL,(void *)recvfun,NULL);
    
    
    choi_conn_fd = conn_fd;

    GtkWidget *box0;
    GtkWidget *box1;

    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *go_button;
    GtkWidget *back_button;

    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window),"choice one from these");
    gtk_window_set_default_size(GTK_WINDOW(window),300,400);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(on_button_back),NULL);//以上是对总窗口的梳理

    box0 = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box0);

    GtkWidget *box_head = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box0),box_head,FALSE,FALSE,0);
    GtkWidget *image = gtk_image_new_from_file("/home/zhuziyu/图片/qie.jpg");
    gtk_box_pack_start(GTK_BOX(box_head),image,FALSE,FALSE,0);
    
    GtkWidget *box_low = gtk_hbox_new(FALSE,5);
    gtk_box_pack_start(GTK_BOX(box0),box_low,FALSE,FALSE,0);

    GtkWidget *table = gtk_table_new(4,4,TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(table),10);
    gtk_box_pack_start(GTK_BOX(box_low),table,FALSE,FALSE,0);


    button1 = gtk_button_new();
    gtk_table_attach(GTK_TABLE(table),button1,0,1,1,3,GTK_FILL,GTK_FILL,0,0);
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(do_button1),NULL);
    GtkWidget *image_siliao = gtk_image_new_from_file("/home/zhuziyu/图片/siliao.jpg");
    gtk_container_add(GTK_CONTAINER(button1),image_siliao);

    button2 = gtk_button_new();
    gtk_table_attach(GTK_TABLE(table),button2,3,4,1,3,GTK_FILL,GTK_FILL,0,0);
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(do_button2),NULL);
    GtkWidget *image_qunliao = gtk_image_new_from_file("/home/zhuziyu/图片/qunliao.jpg");
    gtk_container_add(GTK_CONTAINER(button2),image_qunliao);
 
    back_button = gtk_button_new_with_label("CLOSE");
    g_signal_connect(G_OBJECT(back_button),"clicked",G_CALLBACK(on_button_back),window);
    gtk_table_attach(GTK_TABLE(table),back_button,0,4,3,4,GTK_FILL,GTK_FILL,0,0);
    
    gtk_widget_show_all(window);
    gtk_main();
    return FALSE;
}
