/*************************************************************************
	> File Name: my_struct.h
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年08月05日 星期五 17时18分26秒
 ************************************************************************/

#ifndef _MY_STRUCT_H
#define _MY_STRUCT_H
struct user
{
    int  conn_user_fd;
    char username[32];
    char password[32];
    struct user *next;
};
struct add_friend
{
    char add_friends[24];
    struct add_friend *next;
};
#endif
