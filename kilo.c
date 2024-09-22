#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>


#define CTRL_KEY(k) ((k)&0x1f) 
//creating a global copy of the original terminal
struct termios original_term; 


//error handling function

void die(char* s){
  perror(s);
  exit(1);
}


void disable_raw_mode(){
  if(tcsetattr(STDIN_FILENO , TCSANOW , &original_term)==-1)die("disable_raw_mode function failed to get the terminal attributes.");
}

void enable_raw_mode(){
  //once we enable it we want to disable it at the end of the program . so we register the disable function here
  atexit(disable_raw_mode);
  
  //making another termios structure to set anything we want to it
  struct termios raw;
  if(tcgetattr(STDIN_FILENO , &raw)==-1)die("enable_raw_mode failed to get the terminal attributes.");
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
  if(tcsetattr(STDIN_FILENO,TCSANOW,&raw)==-1)die("enable_raw_mode function failed to set the terminal attributes.");

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
      if(read(STDIN_FILENO , &c,1)==-1 && errno!=EAGAIN)die("Read");
      event(&c); //send the input to the even handler function
      if(c== CTRL_KEY('q'))break; //or we can just check and see if the acsii value of it is equal to 17 or not . but its cooler way.
  }
	return 0;
}
