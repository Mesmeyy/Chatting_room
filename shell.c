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
    mlh = (char *)malloc(sizeof(char)*256);

    while(1) {
        please();
        if(write1(mlh) == -1) {

            continue;
        }
        count = 0;
        count = think(divice,mlh);
        if(chque(divice,count) == -1){
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
    if((pid = fork())<0) {
        printf("Creat the son is fail\n");
        return -1;
    }
    if(pid == 0) {
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

     
            if(flag == 0) {
                
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





