#include <stdio.h>
#include <stdlib.h>

int count(char* str1 ){
    int num = 0 ; 
    while(str1[num]!= '\0'){
        num++;
    }
    return num;
}


void write_data(char** main , char* buf  , int line ){
    for(int i = 0 ; i<count(buf) ; i++){
        main[line][i] = buf[i];
    }
}
char** pull_data(char* path , int * size){
    *size = 0;
    char** main_buffer =(char**) malloc(sizeof(char*));
    char buffer[300];
    int times = 1; 
    
    FILE* pFile = fopen(path , "r");
        while(fgets(buffer , 300 ,pFile )!=NULL){
            main_buffer[times -1 ] = (char*)malloc(sizeof(char*));
            write_data(main_buffer , buffer , times-1);
           *size+=1;
           // printf("%s\n------------------\n" ,main_buffer[times-1]);
            times++;
            main_buffer = realloc(main_buffer , times*sizeof(char*));
        }
    fclose(pFile);
    return main_buffer;

}

