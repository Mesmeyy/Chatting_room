/*************************************************************************
	> File Name: man_to_man.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月03日 星期三 11时10分46秒
 ************************************************************************/

#include<stdio.h>
#include<gtk/gtk.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"my_men_to_men.h"
GtkWidget *entry_who;
GtkWidget *entry_add_who;
GtkWidget *entry_del_who;
GtkWidget *window;
GtkTextBuffer *buffer_history;
GtkTextBuffer *text_view_fre_buffer;
GtkWidget *fileview;
GtkTextIter start,end;
GtkTextIter st,en;
GtkTextIter ss,ee;
int men_conn_fd;
const char *whom;
GtkWidget *fre_print_window;
GtkWidget *window_history;
GtkWidget *window_I;
GtkTextBuffer *buffer_I;
GtkWidget *text_view_I;
GtkWidget *text_view_fre;
GtkWidget *text_view_history;
/*void men_choice_whom (void)
{
    const gchar *whom = gtk_entry_get_text(GTK_ENTRY(entry_who));
    printf("whom:%s\n",whom);
}*/
void men_add_friend_yes()
{
    const gchar *friend_name = gtk_entry_get_text(GTK_ENTRY(entry_add_who));
    char add_friend[128];
    memset(add_friend,0,128);
    strcpy(add_friend,"add/");
    strcat(add_friend,friend_name);
    send(men_conn_fd,add_friend,128,0);
}
void men_del_friend_yes()
{
    const gchar *friend_name = gtk_entry_get_text(GTK_ENTRY(entry_del_who));
    char del_friend[128];
    memset(del_friend,0,128);
    strcpy(del_friend,"del/");
    strcat(del_friend,friend_name);
    send(men_conn_fd,del_friend,128,0);

}
void men_print_friend()
{
     printf("已经显示好友了!\n");
}
void men_send_file()
{
    printf("send file!\n");
}

void men_send_talk()
{
        GtkTextIter start,end;
        GtkTextIter s,e;
        
        char *sen_buffer;
        sen_buffer = (char *)malloc(sizeof(char)*128);
        memset(sen_buffer,0,128);
        
        char *test;
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_I),&start,&end);
        s = start,e = end;
        test = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_I),&s,&e,FALSE);
        strcpy(sen_buffer,test);
        send(men_conn_fd,sen_buffer,128,0);
        printf("已经发送了!\n");
        gtk_text_buffer_set_text(buffer_I,"",0);//把最下面的文本框内容清零
}

void recvfun2(void)
{
    char recv_buffer[128];
    while(1) {
        memset(recv_buffer,0,128);
        char recv_buffer2[128];
        memset(recv_buffer2,0,128);
        if (recv(men_conn_fd,recv_buffer,128,0) > 0){
        //strcpy(recv_buffer2,"\n");
        strcpy(recv_buffer2,recv_buffer);
        printf("recv_buffer2((((((())))))%s*******\n",recv_buffer2);
        //sprintf(recv_buffer2,"%s%s","\n",recv_buffer2);
        strcat(recv_buffer2,"\n");
        
        printf("XXXXX!\n");
        if(strncmp(recv_buffer,"f:",2) == 0) {
            //GtkTextIter st,en;
            gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(text_view_fre_buffer),&st,&en);
            gtk_text_buffer_insert(text_view_fre_buffer,&en,recv_buffer2,strlen(recv_buffer));
            //绑定缓冲区
            //插入文字
            continue;
        }
        //GtkTextIter ss,ee;
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_history),&ss,&ee);
        gtk_text_buffer_insert(buffer_history,&ee,recv_buffer2,strlen(recv_buffer));
        }
    }
}
void bye(void)
{
    char bye[] = {'b','y','e','\0'};
    send(men_conn_fd,bye,4,0);
    gtk_widget_destroy(window);
}

int my_men_to_men(int conn_fd)
{
    pthread_t th;
    pthread_create(&th,NULL,(void*)recvfun2,NULL);
     
    men_conn_fd = conn_fd;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window),500,600);
    gtk_window_set_title(GTK_WINDOW(window),"men to men");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window),"clicked",(void*)bye,NULL);
    
    
    GtkWidget *box_whole = gtk_hbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box_whole);


    
    GtkWidget *box_shu1 = gtk_vbox_new(FALSE,0);
    GtkWidget *box_shu2 = gtk_vbox_new(TRUE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box_shu1,FALSE,FALSE,5);
    gtk_box_pack_end(GTK_BOX(box_whole),box_shu2,FALSE,FALSE,5);
    


    GtkWidget *box_to_who = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_shu1),box_to_who,FALSE,FALSE,0);
    
    GtkWidget *box_who = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),box_who,FALSE,FALSE,5);
    /*GtkWidget *button_who = gtk_button_new_with_label("choice whom");
    gtk_box_pack_start(GTK_BOX(box_who),button_who,FALSE,FALSE,5);
    entry_who = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_who),entry_who,TRUE,TRUE,5);
    g_signal_connect(G_OBJECT(button_who),"clicked",G_CALLBACK(men_choice_whom),NULL);
    */

    GtkWidget *fre_add = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),fre_add,FALSE,FALSE,0);
    GtkWidget *fre_add_button = gtk_button_new_with_label("添加好友");
    gtk_box_pack_start(GTK_BOX(fre_add),fre_add_button,FALSE,FALSE,0);
    
    entry_add_who = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(fre_add),entry_add_who,TRUE,TRUE,0);
    GtkWidget *fre_add_button_yes = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(fre_add),fre_add_button_yes,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(fre_add_button_yes),"clicked",G_CALLBACK(men_add_friend_yes),NULL);//***************

    GtkWidget *fre_del = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),fre_del,FALSE,FALSE,0);
    GtkWidget *fre_del_button = gtk_button_new_with_label("删除好友");
    gtk_box_pack_start(GTK_BOX(fre_del),fre_del_button,FALSE,FALSE,0);
    
    entry_del_who = gtk_entry_new ();
    gtk_box_pack_start(GTK_BOX(fre_del),entry_del_who,TRUE,TRUE,0);
    GtkWidget *fre_del_button_yes = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(fre_del),fre_del_button_yes,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(fre_del_button_yes),"clicked",G_CALLBACK(men_del_friend_yes),NULL);//***************

    GtkWidget *fre_print = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),fre_print,FALSE,FALSE,0);
    GtkWidget *fre_print_button = gtk_button_new_with_label("显示好友");
    gtk_box_pack_start(GTK_BOX(fre_print),fre_print_button,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(fre_print_button),"clicked",G_CALLBACK(men_print_friend),NULL);//*********************
    fre_print_window = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(fre_print_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
    gtk_box_pack_start(GTK_BOX(fre_print),fre_print_window,TRUE,TRUE,0);
    text_view_fre = gtk_text_view_new();
    text_view_fre_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_fre));
    gtk_container_add(GTK_CONTAINER(fre_print_window),text_view_fre);

    
    
    
    
    GtkWidget *box_history = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_shu1),box_history,FALSE,FALSE,10);
    GtkWidget *button_history = gtk_button_new_with_label("history");
    gtk_container_add(GTK_CONTAINER(box_history),button_history);
    GtkWidget *box_history_son = gtk_vbox_new(FALSE,0);
    gtk_widget_set_size_request(box_history_son,100,300);
    gtk_box_pack_start(GTK_BOX(box_history),box_history_son,FALSE,FALSE,0);
    window_history = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window_history),10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (window_history),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);     
    gtk_box_pack_start (GTK_BOX (box_history_son), window_history,TRUE,TRUE,5);
    text_view_history = gtk_text_view_new();
    buffer_history = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_history));
    gtk_container_add(GTK_CONTAINER(window_history),text_view_history);
    

    
    GtkWidget *box_I =gtk_hbox_new(FALSE,5);
    gtk_widget_set_size_request(box_I,100,100);
    gtk_box_pack_start(GTK_BOX(box_shu1),box_I,FALSE,FALSE,10);
    GtkWidget *button_I;
    button_I = gtk_button_new_with_label("I say");
    gtk_box_pack_start(GTK_BOX(box_I),button_I,FALSE,FALSE,3);
    window_I = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window_I),5);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(window_I),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
    gtk_box_pack_start(GTK_BOX(box_I),window_I,TRUE,TRUE,5);
    text_view_I = gtk_text_view_new();
    buffer_I = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_I));
    gtk_container_add(GTK_CONTAINER(window_I),text_view_I);
    GtkWidget *box_son = gtk_vbox_new(TRUE,5);
    GtkWidget *button_talk = gtk_button_new_with_label("talk");
    gtk_box_pack_start(GTK_BOX(box_I),box_son,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box_son),button_talk,FALSE,FALSE,5);//对I的梳理
    g_signal_connect(G_OBJECT(button_talk),"clicked",G_CALLBACK(men_send_talk),NULL);//******
    
    
    GtkWidget *image = gtk_image_new_from_file("/home/zhuziyu/图片/2.jpg");
    gtk_box_pack_end(GTK_BOX(box_shu2),image,FALSE,FALSE,0);
    
    gtk_widget_show_all(window);
    gtk_main();
    return FALSE;

    
    
}
