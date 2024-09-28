#include <termios.h>
//editor structure
struct editorConfig{
  struct termios original_term;
  int EditorRows;
  int EditorCols;
};
struct editorConfig E;




//error handling function

void die(char* s){
  
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO , "\x1b[H" , 3);
  perror(s);
  exit(1);
}


void disable_raw_mode(){
  if(tcsetattr(STDIN_FILENO , TCSANOW , &E.original_term)==-1)die("disable_raw_mode function failed to get the terminal attributes.");
}

void enable_raw_mode(){
//once we enable it we want to disable it at the end of the program . so we register the disable function here
  tcgetattr(STDIN_FILENO,&E.original_term);
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
  //raw.c_oflag &= ~(OPOST);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if(tcsetattr(STDIN_FILENO,TCSANOW,&raw)==-1)die("enable_raw_mode function failed to set the terminal attributes.");

}

