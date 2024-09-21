#include <ctype.h>
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
  atexit(disable_raw_mode);
  
  //making another termios structure to set anything we want to it
  struct termios raw;
  tcgetattr(STDIN_FILENO , &raw);
  raw.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(STDIN_FILENO,TCSANOW,&raw);

}

int main(){
  tcgetattr(STDIN_FILENO,&original_term);
  original_term.c_lflag &= ~(ICANON);
  char c;
	enable_raw_mode();
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) { // checking if the pressed key is a control character or not (control characters are not printable)
      printf("%d\n", c); //display the key ascii code if the character is a control character.
    } else {
      printf("%d ('%c')\n", c, c); //display the key ascii code and the printable part. 
    }
  }
	return 0;
}
