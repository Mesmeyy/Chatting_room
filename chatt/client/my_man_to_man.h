/*************************************************************************
	> File Name: my_man_to_man.h
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月06日 星期六 10时10分15秒
 ************************************************************************/

#ifndef _MY_MAN_TO_MAN_H
#define _MY_MAN_TO_MAN_H
void choice_whom(void);
void add_friend_yes(void);
void del_friend_yes(void);
void print_friend(void);
void send_file(GtkWidget *window,gpointer data);
void recvfromo(void);
void send_talk(void);
int  my_man_to_man(int conn_fd);
void choice_file(GtkWidget *wid,void *pointer);
void select_file_quit(GtkWidget*a,void *b);
void show(GtkWidget *w,void *p);
#endif
