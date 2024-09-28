#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#define CTRL_KEY(k) ((k)&0x1f) 



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

//main functions

int getWindowsSize(int* rows , int*cols){
  struct winsize ws;
  if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws)==-1){return -1;}
  else{
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
  }
}
void initEditor(){
  if(getWindowsSize(&E.EditorRows ,&E.EditorCols)==-1)die("getWindowsSize");
}

char ReadKey(){
  char c;
  int ret;
   while((ret=read(STDIN_FILENO , &c ,1))!=1){
    if(ret==-1 && errno!= EAGAIN)die("Read function.");
  } 
  return c;
}
void draw_lines(){
  for(int i=0;i<E.EditorRows;i++){
    
    write(STDOUT_FILENO,"~\r\n" ,3);
  }
  write(STDOUT_FILENO , "~",1);// its fo the last line of the editor
}
void RefreshScreen(){

  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO , "\x1b[H" , 3);
  draw_lines();
  write(STDOUT_FILENO , "\x1b[H" , 3);
}



void ProcessKey(){
  char c=ReadKey();

  switch (c) {
    case CTRL_KEY('q') :
      write(STDOUT_FILENO , "\x1b[2J" ,4);
      write(STDOUT_FILENO , "\x1b[H" , 3) ;
      exit(0);

      break;
    
  }
}



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
