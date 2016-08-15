/*************************************************************************
	> File Name: frame.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月03日 星期三 14时29分27秒
 ************************************************************************/

#include<stdio.h>
#include<gtk/gtk.h>
GtkWidget *window;
void on_button_close(GtkWidget *window,gpointer *data)
{
   
    gtk_widget_destroy((GtkWidget *)data);
}
int my_frame()
{
    
    GtkWidget *box_whole;
    GtkWidget *butyes;
    GtkWidget *butno;

    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window),200,100);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL); 
    
    box_whole = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box_whole);
    GtkWidget *label = gtk_label_new("用户名或密码输入错误!");
    gtk_box_pack_start(GTK_BOX(box_whole),label,FALSE,FALSE,5);

    GtkWidget *box_c = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box_whole),box_c,FALSE,FALSE,5);
    butyes = gtk_button_new_with_label("确定");
    butno = gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box_c),butyes,FALSE,FALSE,5);
    gtk_box_pack_end(GTK_BOX(box_c),butno,FALSE,FALSE,5);

    g_signal_connect(G_OBJECT(butyes),"clicked",G_CALLBACK(on_button_close),window);
    g_signal_connect(G_OBJECT(butno),"clicked",G_CALLBACK(on_button_close),window);
    
    
    gtk_widget_show_all(window);
    gtk_main();
    return FALSE;

}
