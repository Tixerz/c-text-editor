#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>


#define CTRL_KEY(k) ((k)&0x1f) 

#include "Editor_config_func.h"
#include "Window_Config.h"
#include "Key_Processing.h"
//main functions

int main(){
  char c;
	enable_raw_mode();
 	initEditor();

	while (1) {
     		 RefreshScreen();
      		ProcessKey(); 
  }
	return 0;
	
}
