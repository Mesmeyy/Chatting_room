/*************************************************************************
	> File Name: window.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年07月31日 星期日 21时23分01秒
 ************************************************************************/

#include<stdio.h>
#include<gtk/gtk.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include"my_choice.h"
#include"my_window.h"
 GtkWidget* entry1;
 GtkWidget* entry2;
 GtkWidget* entry3;
 GtkWidget* entry4;
 GtkWidget* entry5;

GtkWidget *window;
int win_conn_fd ;
void ding() {
    entry1 = gtk_entry_new_with_max_length(20);
    entry2 = gtk_entry_new_with_max_length(20);
    entry3 = gtk_entry_new_with_max_length(20);
    entry4 = gtk_entry_new_with_max_length(20);
    entry5 = gtk_entry_new_with_max_length(20);
}
int  on_button_clicked()
{
    while(1){
        
        char recv_username[32];
        char recv_password[32];
        memset(recv_username,0,32);
        memset(recv_password,0,32);

        const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry1));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry2));
        
        send(win_conn_fd,username,strlen(username),0);
        recv(win_conn_fd,recv_username,32,0);
        printf("%s,recv_username\n",recv_username);
        send(win_conn_fd,password,strlen(password)+1,0);
        recv(win_conn_fd,recv_password,32,0);
        printf("%s,recv_password\n",recv_password);

        if((strcmp("yes",recv_username) == 0) &&(strcmp("yes",recv_password) == 0)) {
            //gtk_widget_destroy(window);
            my_choice(win_conn_fd);
            gtk_widget_destroy(window);
        }
        else {
            send(win_conn_fd,"wrong",strlen("wrong")+1,0);
            exit(0);
        }
    }
    return 0;
}

void  my_destory(GtkWidget *window,gpointer *data)
{
    
    
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry3));
    const gchar *password1 = gtk_entry_get_text(GTK_ENTRY(entry4));
    const gchar *password2 = gtk_entry_get_text(GTK_ENTRY(entry5));
    printf("password1:%s,,,password2:%s\n",password1,password2);
    if(strcmp(password1,password2) != 0) {
        send(win_conn_fd,"wrong",strlen("wrong")+1,0);
        exit(0);
    }
    char *send_buffer;
    send_buffer = (char *)malloc(sizeof(char) * 96);
    strcpy(send_buffer,"new_user/");
    strcat(send_buffer,username);
    strcat(send_buffer,"/");
    strcat(send_buffer,password1);
    send(win_conn_fd,send_buffer,strlen(send_buffer)+1,0);
    
    gtk_widget_destroy((GtkWidget *)data);
    
}
void my_destory2(GtkWidget *window,gpointer data)
{
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry3));
    const gchar *password1 = gtk_entry_get_text(GTK_ENTRY(entry4));
    const gchar *password2 = gtk_entry_get_text(GTK_ENTRY (entry5));
    char *send_buffer ;
    send_buffer = (char *) malloc(sizeof(char) * 96);
    memset(send_buffer,0,96);
    strcpy(send_buffer,"changepassword/");
    strcat(send_buffer,username);
    strcat(send_buffer,"/");
    strcat(send_buffer,password1);
    strcat(send_buffer,"/");
    strcat(send_buffer,password2);
    send(win_conn_fd,send_buffer,strlen(send_buffer)+1,0);
    char recv_buffer[5];
    memset(recv_buffer,0,5);
    recv(win_conn_fd,recv_buffer,5,0);
    if(strcmp(recv_buffer,"no") == 0) {
        exit(0);
    }
    gtk_widget_destroy((GtkWidget*)data);
    
}
void chang_passwd()
{
    //GtkWidget *window;

    
    GtkWidget *box_whole;

    GtkWidget *box1;
    GtkWidget *button1;
    

    GtkWidget *box2;
    GtkWidget *button2;
    

    GtkWidget *box3;
    GtkWidget *button3;
    

    GtkWidget *button;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"chang passwd");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    box_whole = gtk_vbox_new(TRUE,0);
    gtk_container_add(GTK_CONTAINER(window),box_whole);

    box1 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box1,FALSE,FALSE,5);
    button1 = gtk_button_new_with_label("账号名");
    gtk_box_pack_start(GTK_BOX(box1),button1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box1),entry3,FALSE,FALSE,5);

    box2 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box2,FALSE,FALSE,5);
    button2 = gtk_button_new_with_label("第一次输入密码");
    gtk_box_pack_start(GTK_BOX(box2),button2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),entry4,FALSE,FALSE,5);

    box3 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box3,FALSE,FALSE,5);
    button3 = gtk_button_new_with_label("第二次输入密码");
    gtk_box_pack_start(GTK_BOX(box3),button3,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box3),entry5,FALSE,FALSE,5);

    button = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box_whole),button,FALSE,FALSE,5);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(my_destory2),window);

    gtk_widget_show_all(window);
}
void new_user()
{    
    
    GtkWidget *box_whole;

    GtkWidget *box1;
    GtkWidget *button1;
    

    GtkWidget *box2;
    GtkWidget *button2;
    

    GtkWidget *box3;
    GtkWidget *button3;
    

    GtkWidget *button;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"new user");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    box_whole = gtk_vbox_new(TRUE,0);
    gtk_container_add(GTK_CONTAINER(window),box_whole);

    box1 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box1,FALSE,FALSE,5);
    button1 = gtk_button_new_with_label("账号名");
    gtk_box_pack_start(GTK_BOX(box1),button1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box1),entry3,FALSE,FALSE,5);

    box2 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box2,FALSE,FALSE,5);
    button2 = gtk_button_new_with_label("第一次输入密码");
    gtk_box_pack_start(GTK_BOX(box2),button2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),entry4,FALSE,FALSE,5);

    box3 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box3,FALSE,FALSE,5);
    button3 = gtk_button_new_with_label("第二次输入密码");
    gtk_box_pack_start(GTK_BOX(box3),button3,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box3),entry5,FALSE,FALSE,5);

    button = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box_whole),button,FALSE,FALSE,5);
 g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(my_destory),window);

    gtk_widget_show_all(window);
}
int my_window(int conn_fd)
{
    ding();
    entry1 = gtk_entry_new_with_max_length(16);
    entry2  = gtk_entry_new_with_max_length(16);
    win_conn_fd = conn_fd;
    //GtkWidget *window;
    GtkWidget *box;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *button;
    GtkWidget *button2;
    GtkWidget *sep;
    
    //entry1 = gtk_entry_new_with_max_length(16);
    //entry2 = gtk_entry_new_with_max_length(16);
    //entry3 = gtk_entry_new_with_max_length(16);
    //entry4 = gtk_entry_new_with_max_length(16);
    //entry5 = gtk_entry_new_with_max_length(16);
    

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"登陆窗口");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),200,150);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);

    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box);

    box1 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,5);

    box2 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box2,FALSE,FALSE,5);

    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,5);

    label1 = gtk_label_new("用户名");
    gtk_box_pack_start(GTK_BOX(box1),label1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box1),entry1,FALSE,FALSE,5);

    label2 = gtk_label_new("  密码");
    gtk_entry_set_visibility(GTK_ENTRY(entry2),FALSE);
    gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),entry2,FALSE,FALSE,5);

    button = gtk_button_new_with_label("确认");
    g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),NULL);
    gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
    

    
    button2 = gtk_button_new_with_label("修改密码");
    gtk_box_pack_start(GTK_BOX(box),button2,FALSE,FALSE,5);
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(chang_passwd),NULL);
    
    
    GtkWidget *button3 = gtk_button_new_with_label("注册新用户");
    gtk_box_pack_start(GTK_BOX(box),button3,FALSE,FALSE,5);
    g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(new_user),NULL);
    
    gtk_widget_show_all(window);
    gtk_main();
    
    
    

}

