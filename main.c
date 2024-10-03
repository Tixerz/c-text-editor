#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>


#define CTRL_KEY(k) ((k)&0x1f) 

#include "Editor_config_func.h"
#include "Key_Processing.h"

int main(){
  char c;
	enable_raw_mode(); //disables flags and some terminal translations
 	initEditor(); //gets windows size 

	while (1) {
//    RefreshScreen(); //display the buffer each time
    ProcessKey(); 
  

  }
	return 0;
	
}
