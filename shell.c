/*************************************************************************
	> File Name: shell.c
	> Author:zhuziyu 
	> Mail: 
	> Created Time: 2016年07月25日 星期一 15时52分06秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

int count_ci = 0;
void please();
int write1(char *mlh);
int think(char divice[][256],char *mlh);
int chque(char divice[][256],int count);
int find_path(char *comm);
int main()
{
    char *mlh = NULL;
    char divice[32][256];
    int count = 0;
    int i,j;
    mlh = (char *)malloc(sizeof(char)*256);

    while(1) {
        fflush(stdin);
        please();
       for(i = 0;i < 5;i++) { 
            for(j = 0; j<16;j++){
           
                divice[i][j] = '\0';//一定要清理缓冲区!

         }   
        }
        
        if(write1(mlh) == -2) {

            exit(0);
        }
        count = 0;
        count = think(divice,mlh);
        if(chque(divice,count) == -2){
            exit(0);
        }
        else {
            continue;
        }
    }
    return 0;
    
} 

void please()                       //write my shell
{
    char pathth[128];
    getcwd(pathth,128);
    printf("\033[01;32mzhuziyu@zhuziyu-x555L\033[00m:\033[01;34m~%s\033[00m",pathth);
}

int write1(char *mlh)                //write in and promise it's right
{
    int len=0;
    char l;
    fflush(stdin);
    memset(mlh,0,256);
    
    while(len <= 255) {

        l = getchar();
        
        if(l == '\n' && len != 0) {
         
            //printf("finish the write\n");
            break;
        
        }
        if(l == '\n' && len == 0) {
            
            return -1;
        
        }

        mlh[len] = l;
        len++;
    }
    if(strncmp(mlh,"exit",4) == 0) {
        return -2;
    }
    if(len == 256) {

        printf("the write is too large !\n ");
        return -1;
    
    }

    mlh[len] = '\0';
    if(strcmp(mlh,"exit") == 0) {
        return -1;
    }

    //printf("the write is : %s\n",mlh);
    
    return 0;
}

int think(char divice[][256] , char *mlh)
{
    int i = 0;
    int j = -1;
    int k = 0;
    int fl = 0;
    for(i = 0;i < 32;i++) {

        memset(*divice,0,256);
    
    }
        for(i = 0;mlh[i] != '\0';i++) {
            
            if(mlh[i] == ' '){
                
                fl = 0;
                continue;

            }
            else {

                if(fl == 0) {
                    j++;
                    k = 0;
                    fl = 1;
                    divice[j][k++] = mlh[i];
                }
                else {
                    divice[j][k++] = mlh[i];
                }
            }
        } 
    


    return ++j;
}
int find_path(char *comm) 
{
    DIR *dir;
    char* path[ ] = {"./","/usr/bin","/bin",NULL};
    struct dirent *dirent;
    int i = 0;

    
    if(strncmp(comm,"./",2) == 0) {
        comm+=2;
    }
    while(path[i] != NULL) {
        if((dir = opendir(path[i])) == NULL) {
             printf("open the %s is fail\n",path[i]);
        }
        while((dirent = readdir(dir)) != NULL){
            if(strcmp(dirent->d_name,comm) == 0) {
                closedir(dir);
                return 0;
            }
        }
        i++;
    }
    printf("Can't fine the commond!\n");
    return -1;
}
/*
int move_to_path(char *path) 
{
    
    char path1[128];
    char path2[128];
    char chun_point[128];
    char chun_path[128];
    memset(path1,0,128);
    memset(path2,0,128);
    memset(chun_point,0,128);
    memset(chun_path,0,128);
    int i,j,k = 0;
    fflush(stdin);
    count_ci++;
    
    
    if((strncmp(path,"../",3) != 0) && (strncmp(path,"./",2) != 0)) {
        
        if(strncmp(path,"/home/zhuziyu/",14) != 0) {
            char pa[128];
            char pa2[128];
            
            memset(pa,0,128);
            memset(pa2,0,128);
            

            
            getcwd(chun_point,128);
            printf("chun_point:!!!%s\n",chun_point);
          
            if(chun_point[strlen(chun_point)-1] != '/') {
               strcat(chun_point,"/");
           }
            printf("0path:%s\n",path);
            strcpy(pa,path);
            printf("0pa:%s\n",pa);
            strcat(chun_point,pa);
            
        }
        printf("chun_point:%s\n",chun_point);
        
        if((chdir(chun_point) != 0) &&(count_ci == 1)) {
            printf("not find path\n");
            return -1;
        }
        else {

            return 0;
        }
    }
    if(strncmp(path,"../",3) == 0) {
        getcwd(path1,64);
        for(i = 0;path1[i] != '\0'; i++) {
            if(path1[i] == '/') {
                j = i;
            }
        }
        for(i = 0;i <= j;i++) {
            path2[i] = path1[i];
        }
        //printf("path2:%s\n",path2);
        chdir(path2);
        for(i = 3;path[i] != '\0';i++) {
            chun_path[k++] = path[i];
        }
        strcpy(path,chun_path);
        //printf("path:%s\n",path);
        move_to_path(path);
        return 0;
    }
       
}*/
int chque(char divice[][256],int count)
{
    
    int back = 0;
    int small = 0;
    int big = 0;
    int flag = 0;
    int guan = 0;
    int i,j,k;
    char* divice2[count + 1];
    char *file;
    char* divice3[count + 1];
    pid_t pid;
    int sta;

    fflush(stdin);
    for(i = 0;i < count + 1;i++) {
        
        divice2[i] = NULL;
    
    }
    for(i = 0;i < count + 1;i++) {
        divice3[i] = NULL;
    }
    for(i = 0; i < count; i++) {
       
        divice2[i] = divice[i];
    
    }

    //for(i = 0;divice2[i] != NULL;i++) {
    //  
    //    printf("divice2[%d] : %s\n",i,divice2[i]);
    //
    //}
    if(strcmp("exit",divice2[0]) == 0) {
        return -2;
    } 
    if(strcmp(divice2[count - 1],"&")==0) {
        back = 1;
        divice2[count - 1] = NULL;
        
    }
    

    for(i = 0;divice2[i] != NULL; i++) {
        
        if(strcmp(divice2[i],">") == 0) {

            flag++;
            big++;
            if(divice2[i+1] == NULL) {
               
                flag++;
            }
        }

        if(strcmp(divice2[i],"<") == 0) {
            
            flag++;
            small++;
            if(i == 0) {
                flag++;
            }
        }
        
        if(strcmp(divice2[i],"|") == 0) {

            flag++;
            guan++;
            if(i == 0) {
                flag++;
            }
            if(divice2[i+1] == NULL) {
                flag++;
            }
        }
        
        if(flag > 1) {
           
            printf("Error in !\n");
            return -1;
        }
    }
    
    if(big == 1) {
        
        for(i = 0; divice2[i] != NULL;i++) {
            if(strcmp(divice2[i],">") == 0) {
                file = divice2[i+1];
                divice2[i] = NULL;
                break;
            }
        }
     }
    if(small == 1) {
        for(i = 0;divice2[i] != NULL;i++) {
            if(strcmp(divice2[i],"<") ==0) {
                file = divice2[i+1];
                divice2[i] = NULL;
                break;
            }
        }
    }
    if(guan == 1) {
        k = 0;
        for(i = 0;divice2[i] != NULL;i++) {
            if(strcmp(divice2[i],"|") == 0) {
                for(j = i + 1;divice2[j] != NULL;j++) {
                    divice3[k++] = divice2[j];
                } 
                break;
            }
        }
        divice3[k] = NULL;
        divice2[i] = NULL;
        //for(i = 0;divice3[i] != NULL;i++) {
        //    printf("divice3[%d] : %s\n",i,divice3[i]);
        //}
    }
    if(strcmp(divice2[0],"cd") == 0) {
        
        
        //move_to_path(divice2[1]);
        if(divice2[2] != NULL) {
            printf("cd <path>\n");
            return -1;
        }
        else if(divice2[1] == NULL) {
            char path[128];

            memset(path,0,128);
            getcwd(path,128);
            chdir(path);
            
            return -1;
        }
        else {
            printf("divice[1]:%s\n",divice2[1]);
            if(chdir(divice2[1]) != 0) {
                perror("divice2[1]:");
                printf("not find the path!\n");
                
                return -1;
            }
            else {
                
                return 0;
            }
        }

        
    }   
    
    if((pid = fork())<0) {
        printf("Creat the son is fail\n");
        return -1;
    }
    if(pid == 0) {
        if(strcmp("history",divice2[0]) == 0) {

            int fd;
            char buf[512];
            if((fd = open("/home/zhuziyu/.bash_history",O_RDONLY)) < 0) {
                perror("open the file:");
                printf("\n");
                return -1;
            }
            while((read(fd,buf,512)) != 0) {
                printf("%s\n",buf);
                memset(buf,0,512);
            }
            exit(0);
        } 
        if(find_path(divice2[0]) == 0) {
            if(big == 1) {
                int fd ;
                if((fd=open(file,O_CREAT|O_RDWR,664))<0) {
                    printf("fail open!\n");
                }
                dup2(fd,1);
                execvp(divice2[0],divice2);
                exit(0);
            }
            if(small == 1) {
                int fd;
                fd = open(file,O_RDONLY);
                dup2(fd,0);
                execvp(divice2[0],divice2);
                exit(0);
            }
            if(guan == 1) {

                int fd2;
                pid_t pid2;
                int staloc;

                pid2 = fork();
                if(pid2 < 0) {
                    printf("creat the son is faill!\n");
                    return -1;
                }
                
                if(pid2 == 0) {
                    
                    if((fd2 = open("/home/zhuziyu/C_language/my_shell/temp.txt",O_CREAT|O_RDWR,S_IRWXU))<0) {
                       printf("error!open the new file!\n");
                    }
                    
                    dup2(fd2,1);
                    execvp(divice2[0],divice2);
                    exit(0);
                }
                if(waitpid(pid2,&staloc,0) == -1) {
                    printf("Wait is error!\n");
                }
                fd2 = open("/home/zhuziyu/C_language/my_shell/temp.txt",O_RDONLY);
        
                dup2(fd2,0);
                execvp(divice3[0],divice3);
                exit(0);
                
            }

     
            if((flag == 0)&&(strcmp("history",divice2[0]) != 0)) {
                
                execvp(divice2[0],divice2);
                exit(0);
            }
            
        
        }
        
    }
    if(back == 1) {
        printf("[pid] : %d\n",pid);
        return -1;
    }
    if(waitpid(pid,&sta,0) == -1) {
        printf("wait is fail!\n");
    }
    return 0;

    
}





