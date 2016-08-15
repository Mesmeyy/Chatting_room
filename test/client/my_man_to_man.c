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
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"my_man_to_man.h"
GtkWidget *entry_who;
GtkWidget *entry_add_who;
GtkWidget *entry_del_who;
GtkTextBuffer *buffer_history;
GtkTextBuffer *text_view_fre_buffer;
GtkWidget *window;
GtkWidget *fileview;
GtkTextIter start,end;
GtkWidget *text_view_fre;
GtkWidget *window_I;
GtkWidget *window_history;
GtkWidget *text_view_I;
GtkWidget *fre_print_window;
GtkWidget *text_view_history;
struct send__file 
{
    char sen_name[64];
    char sen_nei[128];
};
//struct send__file send_file_all;
//struct send__file recv_buffer2;
const char *whom;
GtkTextBuffer *buffer_I;
int man_conn_fd;
void choice_whom (void)
{
    whom = gtk_entry_get_text(GTK_ENTRY(entry_who));
    printf("whom:%s\n",whom);
}
void add_friend_yes()
{
    const gchar *friend_name = gtk_entry_get_text(GTK_ENTRY(entry_add_who));
    char *final_add_friend ;
    final_add_friend = (char *)malloc(sizeof(char) * 64);
    memset(final_add_friend,0,64);
    strcpy(final_add_friend,"add/");
    strcat(final_add_friend,friend_name);
    printf("final_add_friend,%s\n",final_add_friend);
    send(man_conn_fd,final_add_friend,strlen(final_add_friend)+1,0);
    free(final_add_friend);
    
    
}
void del_friend_yes()
{
    const gchar *friend_name = gtk_entry_get_text(GTK_ENTRY(entry_del_who));
    char *final_del_friend ;
    final_del_friend = (char *)malloc(sizeof(char) * 64);
    memset(final_del_friend,0,64);
    strcpy(final_del_friend,"del/");
    strcat(final_del_friend,friend_name);
    printf("final_del_friend:%s\n",final_del_friend);
    send(man_conn_fd,final_del_friend,strlen(final_del_friend)+1,0);
    free(final_del_friend);

}
void print_friend()
{
    printf("已经显示了");
}
void send_file(GtkWidget *win,gpointer data)
{
    const char *filename;
    filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(fileview));
    char *chun_name;
    chun_name = (char *) malloc(sizeof(char) * 64);
    memset(chun_name,0,64);
    int i = 0;
    int k = 0;
    int location = 0;
    for(i = 0;filename[i] != '\0';i++) {
        if(filename[i] == '/') {
            location = i;
        }
    }
    for(i = location+1 ; filename[i] != '\0' ; i++) {
        chun_name[k++] = filename[i];
    }
    printf("filename:%s\n",filename);
    printf("chun_name:%s\n",chun_name);
    k = 0;
    char *send_file_name_before;
    char *send_file_name_all;

    send_file_name_before = (char *)malloc(sizeof(char )*64);
    send_file_name_all    = (char *)malloc(sizeof(char) * 256);

    memset(send_file_name_all,0,256);
    memset(send_file_name_before,0,64);

    strcpy(send_file_name_before,"file/");
    strcat(send_file_name_before,whom);
    strcat(send_file_name_before,"/");
    strcat(send_file_name_before,chun_name);//这是文件名
    

    int fd ;
    int ret;
    char read_buffer[128];

    memset(read_buffer,0,128);

    if((fd = open(filename,O_RDONLY)) < 0) {
        printf("打开文件失败!\n");
    }
    printf("send_file_name_before:%s\n",send_file_name_before);
    send(man_conn_fd,send_file_name_before,64,0);
   
    while((ret = read(fd,read_buffer,sizeof(read_buffer))) > 0) {
        usleep(100);
        send(man_conn_fd,read_buffer,sizeof(read_buffer),0);
        memset(read_buffer,0,128);
    }
    send(man_conn_fd,"over",5,0);
    printf("文件发送完毕!已经全部到达服务器!\n");

    free(chun_name);
    
    close(fd);

}
void choice_file(GtkWidget *widget,void * pointer)
{
    fileview = gtk_file_selection_new("choice :");
    gtk_widget_hide(pointer);
    g_signal_connect(G_OBJECT(fileview),"destory",G_CALLBACK(show),pointer);
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(fileview) -> cancel_button),"clicked",G_CALLBACK(select_file_quit),pointer);
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(fileview) -> ok_button),"clicked",G_CALLBACK(send_file),pointer);
    gtk_widget_show(fileview);

}
void select_file_quit(GtkWidget *a,void *p)
{
    gtk_widget_show(p);
    gtk_widget_destroy(fileview);
}
void show(GtkWidget *wu,void * p)
{
    gtk_widget_show(p);
}
void recvfromo(void)
{
    GtkTextIter iter;
    char recv_buffer[256];
    
    while(1) {

        memset(recv_buffer,0,256);
       
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_history),&start,&end);
        recv(man_conn_fd,recv_buffer,256,0);
        printf("私聊的recv_buffer:%s\n",recv_buffer);
        if(strncmp(recv_buffer,"f:",2) == 0) {
    
            
            GtkTextIter s,e;
            gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(text_view_fre_buffer),&s,&e);
            gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_view_fre_buffer),&e,recv_buffer,strlen(recv_buffer));
            continue;
        }
        if(strncmp(recv_buffer,"H:",2) == 0) {
            int i = 0;
            int k = 0;
            int locate = 0;
            while(recv_buffer[i] != '\0') {
                if(recv_buffer[i] == '/') {
                    locate = i;
                }
                i++;
            }
            

            char *file_name;
            char *file_nei;
            
            file_name= (char *) malloc(sizeof(char ) * 128);
            file_nei= (char *)malloc(sizeof(char ) * 128);
            
            memset(file_name,0,128);
            memset(file_nei,0,128);
            
            for( i = 2;i < locate;i++ ) {
                file_name[k++] = recv_buffer[i];
            }
            printf("\nfile_name:%s\n",file_name);
            k = 0;
            for(i = locate +1;recv_buffer[i] != '\0';i++) {
                file_nei[k++] = recv_buffer[i];
            }
            int f ;
            if((f = open(file_name,O_CREAT|O_RDWR|O_APPEND,0664)) < 0) {
                printf("文件建立失败!\n");
            }
            write(f,file_nei,strlen(file_nei));
            close(f);
            //free(file_name);
            //free(file_nei);
            continue;
        }
       
        if(strncmp(recv_buffer,"nf",2) == 0) {

            /* GtkWidget *dialog;
             *     GtkMessageType type;
            *         gchar *message;
            *        type = GTK_MESSAGE_ERROR;
            *       dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,type,GTK_BUTTONS_OK,"%s",message);
            *                                                                                                  gtk_dialog_run(GTK_DIALOG(dialog));
            *                                                                                                      gtk_widget_destroy(dialog);
            *                                                                                         }
            *                                     }
            */
            int  i = 0;
            int  k = 0;
            char not_friend_name[128];
            memset(not_friend_name,0,128);
            for(i = 0;i < strlen(recv_buffer);i++) {
                not_friend_name[k++] = recv_buffer[i];
            }
            strcat(not_friend_name,"不是你的好友,请添加后再发送消息,你这样撩妹是犯法的!");
            not_friend_name[strlen(not_friend_name)] = '\0';
            printf("not_friend_name:%s\n",not_friend_name);
            //gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_history),&start,&end);
            gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_history),&end,not_friend_name,strlen(not_friend_name));
        }
        if(strncmp(recv_buffer,"file/",5) == 0) {
        
            char recv_buffer2[128];
            char chun_name[64];
            char chun_nei[128];

            memset(chun_name,0,64);
            memset(chun_nei,0,128);
            
            memset(recv_buffer2,0,128);
            int i = 0;
            int locat = 0;
            int k = 0;
            for( i = 5;recv_buffer[i] != '\0';i++ ) {
                chun_name[k++] =recv_buffer[i];
            }
            printf("chun_name:%s\n",chun_name);

            char final_file_name[128];
            memset(final_file_name,0,128);
            strcpy(final_file_name,"/home/zhuziyu/C_language/test/client/");
            strcat(final_file_name,chun_name);

            printf("文件名称:%s\n",final_file_name);
            int fd ;
            if((fd = open(final_file_name,O_CREAT|O_RDWR|O_APPEND,0664)) <  0) {
                printf("文件建立或者打开失败!\n");
            }
            while(1) {
                memset(recv_buffer2,0,128);
                printf("xxx!\n");
                recv(man_conn_fd,recv_buffer2,128,0);
                printf("recv_buffer2ddddd:%s*********************\n",recv_buffer2);
                recv_buffer2[strlen(recv_buffer2)] = '\0';
                if(strncmp(recv_buffer2,"over",4) != 0) {
                    printf("recv_buffer2:%s\n",recv_buffer2);
                    printf("yes!\n");
                    write(fd,recv_buffer2,strlen(recv_buffer2));
                    usleep(10);
                    
                    continue;
                }
                else {
                    printf("no!\n");
                    printf("另一个客户端的文件已经读写完毕!\n");
                    close(fd);
                    break;
                }
            }
            continue;
        }
        //GtkTextIter start,end;
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_history),&start,&end);
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer_history),&end,recv_buffer,strlen(recv_buffer));
    }
}
void send_talk()
{
    GtkTextIter start,end;
    GtkTextIter s,e;
    
    char *sen_buffer;
    sen_buffer = (char *)malloc(sizeof(char)*256);
    memset(sen_buffer,0,256);
    
    char *test;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer_I),&start,&end);
    s = start,e = end;
    test = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer_I),&s,&e,FALSE);
    printf("whom:%s\n",whom);
    strcpy(sen_buffer,whom);
    strcat(sen_buffer,"/");
    strcat(sen_buffer,test);
    printf("sen_buffer:%s\n",sen_buffer);
    send(man_conn_fd,sen_buffer,strlen(sen_buffer)+1,0);
    gtk_text_buffer_set_text(buffer_I,"",0);

}
int my_man_to_man(int conn_fd)
{
    pthread_t thid ;
    pthread_create(&thid,NULL,(void*)recvfromo,NULL);
    man_conn_fd = conn_fd;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window),500,600);
    gtk_window_set_title(GTK_WINDOW(window),"man to man");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    
    
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
    GtkWidget *button_who = gtk_button_new_with_label("choice whom");
    gtk_box_pack_start(GTK_BOX(box_who),button_who,FALSE,FALSE,5);
    entry_who = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_who),entry_who,TRUE,TRUE,5);
    g_signal_connect(G_OBJECT(button_who),"clicked",G_CALLBACK(choice_whom),NULL);
    
    GtkWidget *fre_add = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),fre_add,FALSE,FALSE,0);
    GtkWidget *fre_add_button = gtk_button_new_with_label("add friend");
    gtk_box_pack_start(GTK_BOX(fre_add),fre_add_button,FALSE,FALSE,0);
    
    entry_add_who = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(fre_add),entry_add_who,TRUE,TRUE,0);
    GtkWidget *fre_add_button_yes = gtk_button_new_with_label("添加好友");
    gtk_box_pack_start(GTK_BOX(fre_add),fre_add_button_yes,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(fre_add_button_yes),"clicked",G_CALLBACK(add_friend_yes),NULL);//***************

    GtkWidget *fre_del = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),fre_del,FALSE,FALSE,0);
    GtkWidget *fre_del_button = gtk_button_new_with_label("删除好友");
    gtk_box_pack_start(GTK_BOX(fre_del),fre_del_button,FALSE,FALSE,0);
    
    entry_del_who = gtk_entry_new ();
    gtk_box_pack_start(GTK_BOX(fre_del),entry_del_who,TRUE,TRUE,0);
    GtkWidget *fre_del_button_yes = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(fre_del),fre_del_button_yes,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(fre_del_button_yes),"clicked",G_CALLBACK(del_friend_yes),NULL);//***************

    GtkWidget *fre_print = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_to_who),fre_print,FALSE,FALSE,0);
    GtkWidget *fre_print_button = gtk_button_new_with_label("显示好友");
    gtk_box_pack_start(GTK_BOX(fre_print),fre_print_button,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(fre_print_button),"clicked",G_CALLBACK(print_friend),NULL);//*********************
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
    
    //entry_talk = gtk_entry_new();
    //gtk_box_pack_start(GTK_BOX(box_I),entry_talk,TRUE,TRUE,0);
    GtkWidget *box_son = gtk_vbox_new(TRUE,5);
    GtkWidget *button_file = gtk_button_new_with_label("file");
    GtkWidget *button_talk = gtk_button_new_with_label("talk");
    gtk_box_pack_start(GTK_BOX(box_I),box_son,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box_son),button_file,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box_son),button_talk,FALSE,FALSE,5);//对I的梳理
    g_signal_connect(G_OBJECT(button_file),"clicked",G_CALLBACK(choice_file),NULL);//**********
    g_signal_connect(G_OBJECT(button_talk),"clicked",G_CALLBACK(send_talk),NULL);//******
    
    
    GtkWidget *image = gtk_image_new_from_file("/home/zhuziyu/图片/de.jpg");
    gtk_box_pack_end(GTK_BOX(box_shu2),image,FALSE,FALSE,0);
    
    gtk_widget_show_all(window);
    gtk_main();
    return FALSE;

    
    
}
