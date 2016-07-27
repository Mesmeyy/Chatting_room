#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>

void my_error ( char *string );
int cp_file ( char *file1 , char *file2 ) ;
int fuzhimulu_and_file ( char *path , char *to_path , int flag) ;
void my_error ( char *string ) {                                                     //my_error

    perror(string);

    printf("\n");

    exit(1) ;
}


int cp_file ( char *file1 , char *file2 ) {                                                //file file

    int f;
    int fn;
    int i=0;
    int j=0;
    int k=0;
    int rea;
    char buffer[1024];
    struct stat buf;
    char chun_file1[64];
    char chun_file2[64];
    char chun_path1[128];
    char chun_path2[128];
    char lastold[128];
    char lastnew[128];
    
    memset ( chun_file1 , 0 , 64 ) ;
    memset ( chun_file2 , 0 , 64 ) ;
    memset ( chun_path1 , 0 , 128) ;
    memset ( chun_path2 , 0 , 128) ;
    memset ( lastold , 0 , 128 ) ;
    memset ( lastnew , 0 , 128 ) ;

    for ( i = 0 ; i < strlen ( file1 ) ; i++ ) {
       
        if ( file1[i] == '/' ) {
            j=i;
        }
    }
    if ( j == 0 ) {

        strcpy ( chun_file1 , file1 ) ;
        strcpy ( lastold ,chun_file1 ) ;
    
    }
    if ( j != 0 ) {

        for ( i = 0 ; i <= j ; i++ ){
    
            chun_path1[i] = file1[i] ;
    
    }

        chun_path1[i+1] = '\0' ;

        for( i = j+1 ; i < strlen(file1) ; i++ ) {
    
        chun_file1[k++] = file1[i];
        }
    
        chun_file1[k] = '\0';

        strcpy(lastold,chun_path1);
        strcat(lastold,chun_file1);
    }
    
    if(file2[ strlen ( file2 ) - 1 ] == '/' ) {
    
        strcpy ( lastnew ,file2 ) ;
        strcat ( lastnew ,chun_file1 ) ;
    }
    else{
    
        strcpy ( lastnew , file2 ) ;
    }
    if((f = open (lastold , O_RDONLY ) ) < 0 ) {
                    
        my_error ( "open the old file :   " ) ;
    }
    
    stat ( lastold , &buf ) ;
    
    while( (rea = read ( f , buffer , 512 ) ) != 0 ) {
               
                   
        if ( (fn = open ( lastnew , O_CREAT | O_APPEND | O_RDWR , buf.st_mode ) ) < 0 ) {
                    
                                         
            my_error ( "open the new file : " ) ;
    
        }

        if (write ( fn , buffer , rea ) < 0 ) {
                       
            my_error ( "write : " ) ;
       
        }

        memset ( buffer , 0,1024 ) ;
    }
    
    close ( f ) ;
    close ( fn ) ;
    return 0;
                                    
}


int fuzhimulu_and_file ( char *path , char *to_path , int flag )                   //1 : r + f ; 0 :r 
{
    char sonpath[128];
    char sontopath[128];
    DIR *dir = NULL;
    char r[128];
 
    memset ( sonpath , 0 , 128 ) ;
    memset ( sontopath , 0 ,128 ) ;
    
 
    if ( path [ strlen ( path ) - 1 ] != '/' ) {

        path [ strlen ( path ) ] = '/';
    
    }

    if(to_path[strlen(to_path)-1] != '/'){

        to_path[strlen(to_path)]='/';
    
    }
       
    if( ( dir = opendir ( path ) ) == NULL ) {
        
        return 0;
    
    }
    
    if(dir != NULL){
        
        struct dirent *dirent;
        int count = 0;
        
        
        while( ( dirent = readdir ( dir ) ) != NULL ) {
 
            if( dirent->d_name[0] == '.' ) {
             
                continue;
            }
            
            struct stat buf;

            chdir ( path ) ;
            stat ( dirent->d_name , &buf ) ;
            if( S_ISDIR ( buf.st_mode ) ) {
               
                chdir ( to_path ) ;
                mkdir ( dirent->d_name , buf.st_mode ) ;
                chdir ( path ) ;

                strcpy ( sonpath , path ) ;
                strcat (sonpath , dirent->d_name ) ;
              
                strcpy ( sontopath , to_path ) ;
                strcat ( sontopath , dirent->d_name ) ;
                fuzhimulu_and_file ( sonpath , sontopath , flag ) ;

            }
            if ( !( S_ISDIR ( buf.st_mode ) ) && ( flag==1 ) ) {
            
               
                int f;
                int fn;
                char path_buffer[128];
                char filename[128];
                char file_buffer[512];
                int rea;

                memset ( path_buffer , 0 , 128 ) ;
                strcpy ( path_buffer , path ) ;
                strcat ( path_buffer , dirent->d_name ) ;
               
               
                memset ( filename , 0 , 128 ) ;
                strcpy ( filename , to_path ) ;
                strcat ( filename , dirent->d_name ) ;
              
                             
                if( ( f = open ( path_buffer , O_RDONLY ) ) < 0 ) {
                    
                    my_error ( "open the old file :   " ) ;
                
                }
                                    
                stat ( path_buffer , &buf ) ;
                                   
                while( ( rea = read ( f , file_buffer , 512 ) ) != 0 ) {
               
                             
                    if( ( fn = open ( filename , O_CREAT | O_APPEND | O_RDWR , buf.st_mode ) ) <0 ) {
                   
                                                                              
                         my_error ( "open the new file : " ) ;
                                     
                    }

                    if ( write ( fn , file_buffer , rea ) < 0 ) {
                       
                        my_error ( "write : " ) ;
                      
                    }

                    memset ( file_buffer , 0 , 512 ) ;
                    
                }

                close ( f ) ;
                close ( fn ) ;
                
                memset ( path_buffer , 0 , 128 ) ;
                memset ( filename , 0 , 128 ) ;
                    
            }                     
        }
    }
    return 0;
}

int main ( int argc , char **argv )
{
    int a;
    if ( strcmp ( argv[1] , "-r" ) == 0 ) {

        a = 1;

    }

    else if ( strcmp ( argv[1] , "-or" ) == 0 ) {
        
        a = 2;
    }

    else{

        a = 0;
    }


    switch ( a ) {
        
    case  1 :  fuzhimulu_and_file ( argv[2] , argv[3] , 1 ) ; break ;

    case  2 :  fuzhimulu_and_file ( argv[2] , argv[3] , 0 ) ; break ;

    case  0 :  cp_file ( argv[1] , argv[2] ) ; break ;
    
    }

    return 0;
}


    

    
    
    
