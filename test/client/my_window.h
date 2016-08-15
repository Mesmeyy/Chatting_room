/*************************************************************************
	> File Name: my_window.h
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月06日 星期六 08时40分49秒
 ************************************************************************/

#ifndef _MY_WINDOW_H
#define _MY_WINDOW_H
int  on_button_clicked(void);
char * my_destroy(GtkWidget *window,gpointer *data);
void chang_passwd(void);
void new_user(void);
void ding(void);
int my_window(int conn_fd);
#endif
