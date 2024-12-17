#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>


#define CTRL_KEY(k) ((k)&0x1f) 

//#include "Editor_config_func.h"
#include "Key_Processing.h"
#include "data_handling.h"

int main(int argc , char* argv[]){
  char c;
	enable_raw_mode(); //disables flags and some terminal translations
 	initEditor(); //gets windows size 
    
    // main loop 
    // things we have to do in the main loop : 
    // 1. read data and send it to refresh screen function 
    // 2. process key
    int size  = 0; 
    char** File = pull_data(argv[1] , &size);
    while (1) {
    RefreshScreen(File , size); //display the buffer each time
    ProcessKey(); 
  

  }
	return 0;
	
}
