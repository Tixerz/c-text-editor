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
  //disable echo and canonical mode
  raw.c_lflag &= ~(ECHO|ICANON);

  //disable CTRL-Z and CTRL-C signals
  raw.c_lflag &= ~(ISIG);

  //disable ctrl-s and ctrl-q signals
  raw.c_iflag &= ~(IXON);
  
  //disabling the ctrl-v signal
  raw.c_lflag &= ~(IEXTEN);

  //fix ctrl-M signal
  raw.c_iflag &= ~(ICRNL);

  //turning of all output processing (/r/n). 
  raw.c_oflag &= ~(OPOST);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  tcsetattr(STDIN_FILENO,TCSANOW,&raw);

}
void event(char* ch){
  if(iscntrl(*ch)){ //check and see if the character is a control character (a control character is a nonprintable character).
    printf("%d\r\n" , *ch);}
  else{
    printf("%d:%s\r\n" , *ch ,ch);
  }

}


int main(){
  tcgetattr(STDIN_FILENO,&original_term);
  original_term.c_lflag &= ~(ICANON); // ill move it soon
  char c;
	enable_raw_mode();
    while (1) {
      read(STDIN_FILENO , &c,1);
      event(&c); //send the input to the even handler function
      if(c=='q')break;
  }
	return 0;
}
