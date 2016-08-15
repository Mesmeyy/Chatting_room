/*************************************************************************
	> File Name: my_choice.h
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月06日 星期六 08时52分35秒
 ************************************************************************/

#ifndef _MY_CHOICE_H
#define _MY_CHOICE_H
void on_button_back(GtkWidget *button,gpointer data);
void do_button1(GtkWidget *button1,gpointer data);
void do_button2(GtkWidget *button2,gpointer data);
void do_button3(GtkWidget *button3,gpointer data);
void recvfun(void);
int my_choice(int conn_fd);
#endif
