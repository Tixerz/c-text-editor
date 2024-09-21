#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
//creating a global copy of the original terminal
struct termios original_term; 
void disable_raw_mode(){
  tcsetattr(STDIN_FILENO , TCSANOW , &original_term);
}

void enable_raw_mode(){
  //once we enable it we want to disable it at the end of the program . so we register the disable function here
  tcgetattr(STDIN_FILENO,&original_term);
  atexit(disable_raw_mode);
  
  //making another termios structure to set anything we want to it
  struct termios raw;
  tcgetattr(STDIN_FILENO , &raw);
  raw.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(STDIN_FILENO,TCSANOW,&raw);

}

int main(){
	char n;
	enable_raw_mode();
  while(read(STDIN_FILENO,&n , 1) == 1 && n!='q');
	return 0;
}
