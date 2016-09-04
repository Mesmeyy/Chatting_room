/*************************************************************************
	> File Name: server_to_client_do.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月05日 星期五 09时40分05秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include"server_to_client_do.h"
#include"my_struct.h"
#include<errno.h>
#include<time.h>
#include"mysql.h"
time_t nowtime;
int cou = 0;
struct user* read_in(void);
struct add_friend *read_friend(char *string_home);
void write_in (struct user *phead);
int  add_friend(char *string,char *string_home);
int find_friend(char *string,char *string_home);
int read_server_line(char *string);
int del_friend(char *string,char *string_home);
int built_s_history(struct user *psearch);
int built_c_history(struct user *psearch);
void write_in_history(char *nei,char *string_home);
void send_history(struct user *psearch);
void send_file(char *file_name,char *string_home);
void send_message(char *string,char *string_home);
void do_the_son_do(int count_choice,int conn_fd_address)
{

    int  count = count_choice;
    char recv_buffer[512];//用来接收客户端发来了的消息的缓冲区
    char send_buffer[512];
    struct user *phead = read_in();
    struct user *psearch = phead;
    
    int flag = 0;
    while(1) {
        
        memset(recv_buffer,0,512);
        memset(send_buffer,0,512);
        if(recv(conn_fd_address,recv_buffer,512,0) <=  0) {
            printf("服务器接收数据失败!\n");
            return ;
        }
        if(strcmp(recv_buffer,"wrong") == 0) {
            pthread_exit(NULL);
        }
        printf("recv_buffer:%s\n",recv_buffer);
        if(strncmp(recv_buffer,"new_user/",9) == 0) {

            char *chun_name;
            chun_name = (char *)malloc(sizeof(char) * 32);
            memset(chun_name,0,32);
            char *chun_password;
            chun_password = (char *)malloc(sizeof(char)*32);
            memset(chun_password,0,32);
            int  i = 0;
            int first_ = 0;
            int second_ = 0;
            while(recv_buffer[i] != '\0'){
                if((recv_buffer[i] == '/') && (first_ == 0)){
                    first_ = i;
                }
                if((recv_buffer[i] == '/')&&(first_ != 0)) {
                    second_ = i;
                }//记录下'/'的位置
                i++;
            }
            i = 0;
            int k = 0;
            while(recv_buffer[i] != '\0') {
                if((i > first_) &&(i < second_)) {
                    chun_name[k++] = recv_buffer[i];
                } 
                i++;
            }
            i = 0;
            k = 0;
            while(recv_buffer[i] != '\0') {
                if(i > second_) {
                    chun_password[k++] = recv_buffer[i];
                }
                i++;
            }
            
            struct user *pread = phead;
            struct user *pbefore = phead;
            while(pread != NULL){
                
                pbefore = pread;
                pread = pread->next;
            }//pread 存的是最后一个结构
            
            pread = (struct user *)malloc(sizeof(struct user));
            pread->conn_user_fd = 0;
            strcpy(pread->username,chun_name);
            strcpy(pread->password,chun_password);
            printf("chun_name:%s\n",chun_name);
            strcat(chun_name,"\0");
            printf("chun_password:%s\n",chun_password);
            strcat(chun_password,"\0");
            
            MYSQL *connect;
            connect = mysql_init(NULL);
            char insert[256];
            memset(insert,0,256);
            if(connect == NULL) {
                printf("初始化数据库失败!\n");
            }
            connect = mysql_real_connect(connect,"127.0.0.1","root","548946","talkroom",0,NULL,0);
            strcpy(insert,"INSERT INTO users (name,password) VALUES ('");
            strcat(insert,chun_name);
            strcat(insert,"','");
            strcat(insert,chun_password);
            strcat(insert,"')");
        
            printf("insert:%s\n",insert);
            int f = mysql_query(connect,insert);
            

            if(f == 0) {
            printf("插入数据成功!\n");
            }
            if(f != 0) {
                perror("**********");
            }
            printf("f = %d\n",f);
            mysql_close(connect);
                         
               
            
            pread->next = NULL;
            pbefore->next = pread;
            pbefore = phead;
            char path_buffer[64];
            memset(path_buffer,0,64);
            strcpy(path_buffer,"/home/zhuziyu/C_language/test/server/");
            strcat(path_buffer,chun_name);
            mkdir(path_buffer,0775);
            char friend[128];
            memset(friend,0,128);
            strcpy(friend,path_buffer);
            strcat(friend,"/");
            strcat(friend,chun_name);
            strcat(friend,".txt");
            printf("friend:%s\n",friend);
            int c = open(friend,O_CREAT|O_APPEND|O_RDWR,0664);
            close(c);
            char history[128];
            memset(history,0,128);
            strcpy(history,path_buffer);
            strcat(history,"/");
            strcat(history,chun_name);
            strcat(history,".t");
            c = open(history,O_CREAT|O_APPEND|O_RDWR,0664);
            close(c);
            
            write_in(phead);
            
            continue;
        }
        if(strncmp(recv_buffer,"changepassword/",15) == 0) {
            int first_ = 0;
            int second_ = 0;
            int thred_ = 0;
            int i = 0;
            int p = 0;
            int q = 0;
            int r = 0;
            char g_chun_name[32];
            char g_chun_pass1[32];
            char g_chun_pass2[32];
            memset(g_chun_name,0,32);
            memset(g_chun_pass1,0,32);
            memset(g_chun_pass2,0,32);

            while(recv_buffer[i] != '\0') {
                if(recv_buffer[i] == '/')  {
                    first_ = i;
                    break;
                }
                i++;
            }
            i++;
            while(recv_buffer[i] != '\0') {
                if(recv_buffer[i] == '/') {
                    second_ = i;
                    break;
                }
                i++;
            }
            i++;
            while(recv_buffer[i] != '\0') {
                if(recv_buffer[i] == '/') {
                    thred_ = i;
                }
                i++;
            }
            i = 0;
            while(recv_buffer[i] != '\0') {
                if((i > first_) && (i < second_)) {
                    g_chun_name[p++] = recv_buffer[i];
                }
                if((i > second_) && (i < thred_)) {
                    g_chun_pass1[q++] = recv_buffer[i];
                }
                if(i > thred_) {
                    g_chun_pass2[r++] = recv_buffer[i];
                }
                i++;
            }//分离出来用户,新密码,和旧密码
            
            
            struct user *g_psearch = phead;
            while(g_psearch != NULL) {

                if(strcmp(g_psearch->username,g_chun_name) == 0) {

                    break;
                }
                g_psearch = g_psearch->next;
            }
            if(g_psearch == NULL) {
                send(conn_fd_address,"no",2,0);
                close(conn_fd_address);
                pthread_exit(0);
            }
            if(strcmp(g_psearch->password,g_chun_pass1) != 0) {
                send(conn_fd_address,"no",2,0);
                close(conn_fd_address);
                pthread_exit(0);
            }
            strcpy(g_psearch->password,g_chun_pass2);
            write_in(phead);
            phead =read_in();
            send(conn_fd_address,"yes",3,0);
            continue;
        }
        
        if(count == 1) {
            
            while(psearch != NULL) {
                if((strcmp(psearch->username,recv_buffer) == 0)) {
                    if(psearch->conn_user_fd != 0 ) {
                        send(conn_fd_address,"sec",4,0);
                        count++;
                        break;
                    }
                    
                    printf("这个人之前没有上线!\n");
                    count++;
                    flag = 1;
                    psearch->conn_user_fd = conn_fd_address;
                    send(psearch->conn_user_fd,"yes",3,0);
                    built_s_history(psearch);
                    
                    write_in(phead);
                    break;
                }
                
                psearch = psearch->next;
            }
            if(psearch  == NULL) {
                send(conn_fd_address,"no",2,0);
            }
            
            //if(psearch != NULL) {
                continue;
            
        }
        if(count == 2) {
            if(flag == 1){
                if(strcmp(recv_buffer,psearch->password) == 0) {
                    count++;
                    send(psearch->conn_user_fd,"yes",3,0);
                    send_history(psearch);
                    continue;
                
                }
                else {
                send(conn_fd_address,"no",2,0);
                close(conn_fd_address);
                pthread_exit(0);
                }
                
            }
            if(flag == 0) {
                send(conn_fd_address,"no",2,0);
                close(conn_fd_address);
                pthread_exit(0);
            }
            
        }
        if(strcmp(recv_buffer,"bye") == 0) {

            printf("感谢使用!\n");
            psearch->conn_user_fd = 0;
            write_in(phead);
            break;
        }
        if((count != 1) && (count != 2)) {
            if(strncmp(recv_buffer,"qun",3) == 0) {
                char recv_buffer3[128];
                while(1) {
                    printf("您当前在群聊窗口!\n");
                    memset(recv_buffer3,0,128);
                    if(strncmp(recv_buffer3,"qun",3) == 0) {
                        break;
                    }
                    recv(psearch->conn_user_fd,recv_buffer3,128,0);
                    printf("recv_buffer3:%s\n",recv_buffer3);
                    if(strncmp(recv_buffer3,"add/",4) == 0) {
                        if(add_friend(recv_buffer3,psearch->username) == 0) {
                            printf("添加好友成功!\n");
                        }
                        continue;
                    }
                    if(strncmp(recv_buffer3,"del/",4) == 0) {
                        del_friend(recv_buffer3,psearch->username);
                        continue;
                    }
                    struct add_friend *show_phead = read_friend(psearch->username);
                    struct add_friend *show_psearch = show_phead->next;
                    while(show_psearch != NULL) {
                        usleep(50);
                        char send_friend[64];
                        memset(send_friend,0,64);
                        strcpy(send_friend,"f:");
                        strcat(send_friend,show_psearch->add_friends);
                        strcat(send_friend,"\n");
                        send(psearch->conn_user_fd,send_friend,strlen(send_friend),0);
                        show_psearch = show_psearch->next;
                    }
                    if(strncmp(recv_buffer3,"bye",3) == 0) {
                        
                        break;
                    }
                    sleep(1);
                    send_message(recv_buffer3,psearch->username);
                }
                
                continue;
            }
            
            if(strncmp(recv_buffer,"add/",4) == 0) {

                if(add_friend(recv_buffer,psearch->username) == 0){
                    printf("添加成功!\n");
                }
                continue;
            }
            if(strncmp(recv_buffer,"del/",4) == 0) {
                del_friend(recv_buffer,psearch->username);
                continue;
            }
            if(strncmp(recv_buffer,"file/",5) == 0) {
                
                
                int locat = 0;
                int i = 0;
                int k = 0;
                char *to_name;
                
                to_name = (char *)malloc(sizeof(char) * 32);
                memset(to_name,0,32);
               
                for(i = 5;recv_buffer[i] != '\0';i++) {
                    if((recv_buffer[i] == '/') && (locat == 0)) {
                        locat = i;
                        break;
                    }
                }
                for(i = 5; i < locat ;i++) {
                        to_name[k++] = recv_buffer[i];//解析出来给谁发
                }
                k = 0;
                char file_name[64];
                memset(file_name,0,64);
                
                for(i = locat +1;recv_buffer[i] != '\0';i++) {
                   
                    file_name[k++] =recv_buffer[i];//解析出来了文件名
                
                }
                file_name[k] = '\0';
                
                char *fwq_file_name;
                fwq_file_name = (char *)malloc(sizeof(char) * 128);
                memset(fwq_file_name ,0,128);
                
                strcpy(fwq_file_name,"/home/zhuziyu/C_language/test/server/");
                strcat(fwq_file_name,to_name);
                strcat(fwq_file_name,".");
                strcat(fwq_file_name,file_name);

                
                /*在服务器上建立文件*/
                int y = 0;
                int f;
                
                char recv_buffer2[128];
                memset(recv_buffer2,0,128);

                if((f = open(fwq_file_name,O_CREAT|O_APPEND|O_RDWR,0664)) <0 ) {
                    printf("服务器文件建立失败!\n");
                }

                while(1) {
                    
                    recv(psearch->conn_user_fd,recv_buffer2,128,0);
                    if(strncmp(recv_buffer2,"over",4) == 0) {
                        close(f);
                        printf("服务器文件已经写完了!");
                        break;
                    }
                    write(f,recv_buffer2,strlen(recv_buffer2));
                    memset(recv_buffer2,0,128);
                }
                send_file(fwq_file_name,psearch->username);

                
                continue;
            }
            struct add_friend *show_phead = read_friend(psearch->username);
            struct add_friend *show_psearch = show_phead->next;
            while(show_psearch != NULL) {
                sleep(1);
                char send_friend[64];
                memset(send_friend,0,64);
                strcpy(send_friend,"f:");
                strcat(send_friend,show_psearch->add_friends);
                strcat(send_friend,"\n");
                send(psearch->conn_user_fd,send_friend,strlen(send_friend)+1,0);
                show_psearch = show_psearch->next;
            }//显示好友并刷新
            
            int i = 0;
            int k = 0;
            int locate = 0;
            char chun_send_name[32];
            char chun_send_nei[128];
            memset(chun_send_name,0,32);
            memset(chun_send_nei,0,128);
            while(recv_buffer[i] != '\0') {
                if(recv_buffer[i] == '/') {
                    locate = i;
                }
                i++;
            }
            for(i = 0;i < locate;i++) {
                chun_send_name[i] = recv_buffer[i];
            }
            for(i = i+1;recv_buffer[i] != '\0';i++) {
                chun_send_nei[k++] =recv_buffer[i];
            }
            printf("饲料对象:%s\n",chun_send_nei);
            if(find_friend(chun_send_name,psearch->username) == 0) {
                int to_fd = read_server_line(chun_send_name);
                char send_finally_o[256];
                char send_finally_m[256];
                
                memset(send_finally_m,0,256);
                memset(send_finally_o,0,256);
                
                time(&nowtime);
                strcpy(send_finally_o,ctime(&nowtime));
                //strcat(send_finally_o,"/");
                strcat(send_finally_o,psearch->username);
                strcat(send_finally_o," :");
                strcat(send_finally_o,"\n");
                strcat(send_finally_o,chun_send_nei);
                strcat(send_finally_o,"\n");
                send(to_fd,send_finally_o,strlen(send_finally_o)+1,0);
                write_in_history(send_finally_o,chun_send_name);
                
                usleep(100);
                
                strcpy(send_finally_m,ctime(&nowtime));
                //strcat(send_finally_m,"/");
                strcat(send_finally_m,"I : \n");
                strcat(send_finally_m,chun_send_nei);
                strcat(send_finally_m,"\n");
                send(psearch->conn_user_fd,send_finally_m,strlen(send_finally_m)+1,0);
                write_in_history(send_finally_m,psearch->username);
                

            }else {
                char nofriend[32];
                memset(nofriend,0,32);
                strcpy(nofriend,"nf");
                strcat(nofriend,chun_send_name);
                printf("不是好有时发送的内容:%s\n",nofriend);
                send(psearch->conn_user_fd,nofriend,strlen(nofriend),0);
                continue;
            }
        }
        count++;
    }
}
struct user* read_in()
{
    struct user *pnew = NULL;
    struct user *phead= NULL;
    struct user *pend = NULL;
    FILE *fp;
    int cou = 0;
    fp = fopen("/home/zhuziyu/C_language/test/server/user_info.txt","r");
    phead = (struct user*)malloc(sizeof(struct user));
    pend = phead;
    phead ->conn_user_fd = 0;
    strcpy(phead->username,"root");
    phead->next = NULL;
    
    pnew = (struct user *)malloc(sizeof(struct user));
    while(fread(pnew,sizeof(struct user),1,fp) != 0) {
        pend->next = pnew;
        pnew->next = NULL;
        pend = pnew;
        pnew = (struct user *)malloc(sizeof(struct user));
    }
    /*while(fread(pnew,sizeof(struct user),1,fp) != 0){
        cou++;
        if(cou == 1) 
        {
            pnew->next = NULL;
            phead = pnew;
            pend = pnew;
        }
        if(cou != 1) {
            pend->next = pnew;
            pnew->next = NULL;
            pend = pnew;
        }
        pnew = (struct user *)malloc(sizeof(struct user));
    }*/
    fclose(fp);
    struct user *psearch = phead->next;
    while(psearch != NULL)
    {
        printf("注册者:%s\n",psearch->username);
        psearch = psearch->next;
    }
    return phead;
}
void write_in(struct user *phead)
{
    FILE *fp ;
    struct user *p = phead->next;//写进去的时候不能带头节点!
    fp = fopen("./user_info.txt","wt+");
    while(p != NULL) {
        
        fwrite(p,sizeof(struct user),1,fp);
        printf("psearch->u:%s\n",p->username);
        
        p = p -> next;
    }
    fclose(fp);

}
int add_friend(char *string,char *home)
{
    if(strcmp(string,home) == 0) {
        printf("不能添加自己的名字作为好友!\n");
        return 0;
    }
    int location = 0;
    int i = 0;
    int k = 0;
    char *chun_add_name;
    chun_add_name = (char *)malloc(sizeof(char) * 32);
    memset(chun_add_name,0,32);
    while(string[i] != '\0') {
 
        if(string[i] == '/') {
            location = i;
            break;
        }
       i++;
    }
    for(i = location+1;string[i] != '\0';i++) {

        chun_add_name[k++] = string[i];
    }
    
    FILE *fp;
    int fd =0;
    char home_name[128];
    memset(home_name,0,128);
    strcpy(home_name,"/home/zhuziyu/C_language/test/server/");
    strcat(home_name,home);
    strcat(home_name,"/");
    strcat(home_name,home);
    strcat(home_name,".txt");
    
    
    if((fd = open(home_name,O_CREAT|O_APPEND|O_RDWR,0664)) < 0) {
       perror("file:"); 
    }

    close(fd);
    
    fp = fopen(home_name,"r");
    
    struct add_friend *phead = NULL;
    struct add_friend *pend = NULL;
    struct add_friend *pnew = NULL;
    phead = (struct add_friend *)malloc(sizeof(struct add_friend));
    memset(phead,0,sizeof(struct add_friend));
    phead->next = NULL;
    pend = phead;
    pnew = (struct add_friend *)malloc(sizeof(struct add_friend));
    memset(pnew,0,sizeof(struct add_friend));
    while(fread(pnew,sizeof(struct add_friend),1,fp) != 0){

        pnew->next = NULL;
        pend->next = pnew;
        pend = pnew;
        pnew = (struct add_friend *)malloc(sizeof(struct add_friend));
        memset(pnew,0,sizeof(struct add_friend));
    }
    fclose(fp);
    printf("end:%s\n",pend->add_friends);
    struct user *read_phead = read_in();
    struct user *ptr = read_phead;
    while(ptr != NULL) {
        
        if(strcmp(ptr->username,chun_add_name) == 0) {
            
            struct add_friend *new ;
            new = (struct add_friend *)malloc(sizeof(struct add_friend));
            strcpy(new->add_friends ,chun_add_name);
            new->next = NULL;
            pend->next = new;
            pend = new;
            struct add_friend *www = phead->next;
            while(www != NULL) {
                printf("www:%s\n",www->add_friends);
                www = www->next;
            }
            
            fp = fopen(home_name,"wt+");
            struct add_friend *add_friend_line = phead->next;//phead代表好友列表的头
            while(add_friend_line != NULL) {
                fwrite(add_friend_line,sizeof(struct add_friend),1,fp);
                add_friend_line = add_friend_line->next;
            }
        
            fclose(fp);
            return 0; 
        }
        ptr = ptr->next;
    }
    return -1;
}
struct add_friend *read_friend(char *whom)
{
    char *friend_file;
    friend_file = (char *)malloc(sizeof(char) * 64);
    memset(friend_file,0,64);
    strcpy(friend_file,"/home/zhuziyu/C_language/test/server/");
    strcat(friend_file,whom);
    strcat(friend_file,"/");
    strcat(friend_file,whom);
    strcat(friend_file,".txt");
    FILE *fp;
    if((fp = fopen(friend_file,"r")) == NULL){
        printf("文件打开失败!\n");
    }
    struct add_friend *phead = NULL;
    struct add_friend *pend = NULL;
    struct add_friend *pnew = NULL;
    phead = (struct add_friend *)malloc(sizeof(struct add_friend));
    memset(phead,0,sizeof(struct add_friend));
    phead->next = NULL;
    pend = phead;
    pnew = (struct add_friend *)malloc(sizeof(struct add_friend));
    while(fread(pnew,sizeof(struct add_friend),1,fp) != 0){
        
        pnew->next = NULL;
        pend->next = pnew;
        pend = pnew;
        pnew = (struct add_friend *)malloc(sizeof(struct add_friend));
        
    }
    fclose(fp);
    
    return phead;//读出来的算是带头节点的链表.

}
int find_friend(char *string,char *string_home)
{
    struct add_friend *phead = read_friend(string_home);
    struct add_friend *psearch_friend = phead;
    while(psearch_friend != NULL) {
        if(strcmp(string,psearch_friend->add_friends) == 0) {
            return 0;
        }
        psearch_friend = psearch_friend ->next;
    }
    return -1;
}
int read_server_line(char *string)
{
    struct user *phead = read_in();
    struct user *psearch_line = phead;
    while(psearch_line != NULL) {
        if(strcmp(psearch_line->username,string) == 0) {
            return psearch_line->conn_user_fd;
        }
        psearch_line = psearch_line -> next;
    }
    return -1;
}

int del_friend(char *string,char *string_home)//string是要删的,string_home是主人
{
    int locati = 0;
    int i = 0;
    int k = 0;
    for(i = 0; string[i] != '\0';i++){
        if(string[i] == '/') {
            locati = i;
            break;
        }
    }
    char chun_del_name[32];
    memset(chun_del_name,0,32);
    
    for(i = locati+1;string[i] != '\0';i++) {
        chun_del_name[k++] = string[i];
    }
    

    char del_friend_file[128];
    memset(del_friend_file,0,128);
    strcpy(del_friend_file,"/home/zhuziyu/C_language/test/server/");
    strcat(del_friend_file,string_home);
    strcat(del_friend_file,"/");
    strcat(del_friend_file,string_home);
    strcat(del_friend_file,".txt");
    FILE *fp;
    fp = fopen(del_friend_file,"r");
    struct add_friend *del_friend_phead = NULL;
    struct add_friend *del_friend_pnew = NULL;
    struct add_friend *del_friend_pend = NULL;
    del_friend_phead = (struct add_friend *)malloc(sizeof(struct add_friend));
    memset(del_friend_phead,0,sizeof(struct add_friend));
    del_friend_phead->next = NULL;
    del_friend_pend = del_friend_phead;
    del_friend_pnew = (struct add_friend *)malloc(sizeof(struct add_friend));
    memset(del_friend_pnew,0,sizeof(struct add_friend));
    while(fread(del_friend_pnew,sizeof(struct add_friend),1,fp) != 0) {
        strcat(del_friend_pnew->add_friends,"\0");
        del_friend_pnew ->next = NULL;
        del_friend_pend->next = del_friend_pnew;
        del_friend_pend = del_friend_pnew;
        del_friend_pnew = (struct add_friend *)malloc(sizeof(struct add_friend));
        memset(del_friend_pnew,0,sizeof(struct add_friend));
    }//已经读出所有好友
    fclose(fp);
    del_friend_pnew = del_friend_phead;
    while(del_friend_pnew != NULL) {
        
        del_friend_pnew = del_friend_pnew->next;
    }
    struct add_friend *del_friend_pbefore = del_friend_phead;
    struct add_friend *del_friend_psearch = del_friend_phead;
    while(del_friend_psearch != NULL) {
        printf("del_friend_psearch:%s\n",del_friend_psearch->add_friends);
        if(strcmp(del_friend_psearch->add_friends,chun_del_name) == 0) {
            printf("hahaha!\n");
            del_friend_pbefore->next = del_friend_psearch->next;
            free(del_friend_psearch);
            break;
        }
        del_friend_pbefore = del_friend_psearch;
        del_friend_psearch = del_friend_psearch->next;
    }//已经删除了那个好友
    del_friend_psearch = del_friend_phead;
    while(del_friend_psearch != NULL) {
        
        del_friend_psearch = del_friend_psearch->next;
    }
    FILE *f;
    f = fopen(del_friend_file,"wt+");
    del_friend_psearch = del_friend_phead->next;
    while(del_friend_psearch != NULL) {
        
        fwrite(del_friend_psearch,sizeof(struct add_friend),1,f);
        del_friend_psearch = del_friend_psearch->next;
    }
    fclose(f);
    return 0;

}
int built_s_history(struct user *home)
{
    int fd ;
    char s_filename[128];
    memset(s_filename,0,128);
    strcpy(s_filename,"/home/zhuziyu/C_language/test/server/");
    strcat(s_filename,home->username);
    strcat(s_filename,"/");
    strcat(s_filename,home->username);
    strcat(s_filename,".t");
    printf("s_filename:%s\n",s_filename);
    if((fd = open(s_filename,O_CREAT|O_APPEND|O_RDWR,0664)) < 0) {
        perror("open1:");
    }
    close(fd);//已经创建了服务器上的文件
}
int built_c_history(struct user *home)
{
    int fd;
    char c_filename[128];
    memset(c_filename,0,128);
    strcpy(c_filename,"/home/zhuziyu/C_language/test/client/");
    strcat(c_filename,home->username);
    strcat(c_filename,".t");
    if((fd = open(c_filename,O_CREAT|O_APPEND|O_RDWR)) < 0) {
        perror("open2:");
    }
    close(fd);//已经建立了服务器上的文件
}
void write_in_history(char *string,char *string_home)
{
    char *find_file_name;
    int fd ;
    find_file_name = (char*)malloc(sizeof(char) * 128);
    memset(find_file_name,0,128);
    strcpy(find_file_name,"/home/zhuziyu/C_language/test/server/");
    strcat(find_file_name,string_home);
    strcat(find_file_name,"/");
    strcat(find_file_name,string_home);
    strcat(find_file_name,".t");
    printf("find_file_name:%s\n",find_file_name);
    if((fd = open(find_file_name,O_CREAT|O_RDWR|O_APPEND,0664)) < 0) {
        perror("open3:");
    }
    printf("string:%s\n",string);
    if(write(fd,string,strlen(string))< 0) {
        printf("写有问题!\n");
    }
    close(fd);
}
void send_history(struct user *home)
{
    char send_file_name[128];
    char recv_file_name[128];
    int s_fd;
    int c_fd;
    char *file_buffer;
    file_buffer = (char *)malloc(sizeof(char) * 128);
    int ret;

    memset(send_file_name,0,128);
    memset(recv_file_name,0,128);
    memset(file_buffer,0,128);

    strcpy(send_file_name,"/home/zhuziyu/C_language/test/client/");
    strcat(send_file_name,home->username);
    
    strcat(send_file_name,".t/");

    char send_file_history[128];
    memset(send_file_history,0,128);
    strcpy(send_file_history,"/home/zhuziyu/C_language/test/server/");
    strcat(send_file_history,home->username);
    strcat(send_file_history,"/");
    strcat(send_file_history,home->username);
    strcat(send_file_history,".t");

    printf("send_file_history:%s\n",send_file_history);
    if((s_fd = open(send_file_history,O_RDONLY)) < 0) {
        perror("open send_file_history:!\n");
    }
    char file_buffer_finally[256];
    memset(file_buffer_finally,0,256);
    while( (ret = read(s_fd,file_buffer,128))  != 0) {

        memset(file_buffer_finally,0,256);
        strcpy(file_buffer_finally,"H:");
        strcat(file_buffer_finally,send_file_name);
        strcat(file_buffer_finally,file_buffer);
               
        send(home->conn_user_fd,file_buffer_finally,strlen(file_buffer_finally),0); 
        
        sleep(1);
        
    }
    close(s_fd);
}
void send_file(char *file_name,char *string_home)//服务器上的文件名,还有当前的主人
{
    int i = 0;
    char to[64];
    char chun_filename[64];
    int k = 0;
    int locat0 = 0;
    int locat1 = 0;

    memset(to,0,64);
    memset(chun_filename,0,64);
    
    for(i = 0;file_name[i] != '\0';i++) {
        if(file_name[i] == '/') {
            locat0 = i;//记录最后一个'/'的位置
        }
        if(file_name[i] == '.') {
            locat1 = i;
            break;//记录给谁和文件名中间的那个点!
        }
    }
    for( i = locat0+1;i < locat1;i++ ) {
        to[k++] = file_name[i];
    }

    k = 0;
    
    for(i = locat1+1;file_name[i] != '\0';i++) {
        chun_filename[k++] = file_name[i];
    }

    char send_flag[64];
    memset(send_flag,0,64);
    strcpy(send_flag,"file/");
    strcat(send_flag,chun_filename);

    struct add_friend *friend_phead = read_friend(string_home);
    struct add_friend *friend_psearch = friend_phead;
    while(friend_psearch != NULL) {
        
        if(strcmp(friend_psearch->add_friends,to) == 0) {
        
            struct user *user_phead = read_in();
            struct user *user_psearch = user_phead;
            while(user_psearch != NULL) {
                
                if(strcmp(user_psearch->username,to) == 0) {
                    int fd;
                    if((fd = open (file_name,O_RDONLY)) < 0) {
                        printf("服务器文件打开失败!\n");
                    }
                    
                    char read_buffer[128];
                    memset(read_buffer,0,128);

                    int c_read ;
                    send(user_psearch->conn_user_fd,send_flag,strlen(send_flag),0);
                    while(( c_read = read(fd,read_buffer,128)) > 0) {
                        usleep(100);
                        send(user_psearch->conn_user_fd,read_buffer,c_read,0);
                        printf("hahaha!***********\n");
                        memset(read_buffer,0,128);
                    }
                    sleep(1);
                    char end[6] ;
                    strcpy(end,"over");
                    end[strlen(end)] = '\0';
                    printf("服务器上的文件已经发送完毕了!\n");
                    send(user_psearch->conn_user_fd,"over",4,0);
                    printf("end:%s\n",end);
                    close(fd);
                    return ;
                    
                }
                user_psearch = user_psearch->next;
            }
        }
        friend_psearch = friend_psearch ->next;
    }
    printf("没有这个好友,不能发送!");
}
void send_message(char *string,char *string_home)
{
    struct user *online_phead = read_in();
    
    struct user *online_psearch = online_phead -> next;
    while(online_psearch != NULL) {
        
        if(online_psearch->conn_user_fd != 0) {
            
            printf("************在线的是:%s\n",online_psearch->username);

            if(strcmp(online_psearch->username,string_home) == 0) {
                char I_say[128];
                memset(I_say,0,128);
                strcpy(I_say,"I:\n");
                strcat(I_say,string);
                strcat(I_say,"\n");
                printf("I_say:%s\n",I_say);
                send(online_psearch->conn_user_fd,I_say,strlen(I_say),0);
                write_in_history(I_say,online_psearch->username);
            }

            if(strcmp(online_psearch->username,string_home) != 0) {
                char he_say[128];
                memset(he_say,0,128);
                strcpy(he_say,string_home);
                strcat(he_say,":\n");
                strcat(he_say,string);
                strcat(he_say,"\n");
                printf("he_say:%s\n",he_say);
                send(online_psearch->conn_user_fd,he_say,strlen(he_say),0);
                write_in_history(he_say,string_home);
            } 
        }
        online_psearch = online_psearch -> next;
    }
    return ;
}
