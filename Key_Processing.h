#include "Editor_config_func.h"

int getCursorPosition();


char ReadKey(){
  char c;
  int ret;
   while((ret=read(STDIN_FILENO , &c ,1))!=1){
    if(ret==-1 && errno!= EAGAIN)die("Read function.");
  } 
  return c;
}

int getWindowsSize(int* start , int* end);

void ProcessKey(int* start , int* end , int file_size){
    int wrow;
    int wcol;
    int temp3;

    temp3 = getWindowsSize(&wrow , &wcol);
    
    char c=ReadKey();

  switch (c) {
    case CTRL_KEY('q') :
      write(STDOUT_FILENO , "\x1b[2J" ,4);
      write(STDOUT_FILENO , "\x1b[H" , 3) ;
      exit(0);
	
      break;
    case 'k':
      if(E.cursor_row == wrow-1 ){
       if(*start+1 <file_size-wrow ) *start +=1;
        if(*end+1 < file_size)*end +=1;
      }else {
        E.cursor_row+=1;
      }
      break;
    case 'l':
      E.cursor_col+=1;
      break;
    case 'j':
      if(E.cursor_row == 1){
          if(*start-1 >1)*start -= 1; 
         if(*end-1 < wrow) *end -=1;
      }else if(E.cursor_row-1 >0){
        E.cursor_row-=1;
      }
      break;
    case 'h':
      E.cursor_col-=1;
      break;

 }
}
int getCursorPosition(int* c_row , int* c_col) {
  char buf[32];
  int i = 0;
  write(STDOUT_FILENO, "\x1b[6n", 4);
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';
  sscanf(&buf[2] , "%d;%d" ,c_row , c_col );
  
  return -1;
}


int SetCursorPos(int row  , int col){
  E.cursor_row=row;
  E.cursor_col = col;
  write(STDOUT_FILENO ,"\x1b[H" ,3);
  for(int i=1; i< row ; i++){
    write(STDOUT_FILENO , "\x1b[B" , 3);
  }
  for(int i =1 ; i< col ; i++){
    write(STDOUT_FILENO , "\x1b[C", 3);
  }
  return 0;
}


int getWindowsSize(int* rows , int* cols){
  struct winsize ws;
  if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws)==-1){return -1;}
  else{
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
  }
}

void screen_buff_print(int start , int end , char** File){
    for(int i=start ; i <=end ; i++){
        printf("%s" , File[i]);
    }
}
void draw_lines(char ** File , int start , int end  ){;
    screen_buff_print(start , end , File);
  //int m = getCursorPosition();
}


void RefreshScreen(char ** File , int start , int end ){
  write(STDOUT_FILENO, "\x1b[2J", 4); // clearing the screen
  write(STDOUT_FILENO , "\x1b[H" , 3); //repositioning the cursor and making it ready for drawing lines
  draw_lines( File , start , end  );
  //screen_buff_print(start , end , File);
    
  SetCursorPos(E.cursor_row , E.cursor_col); // repositioning the cursor after drawing lines

}


void initEditor(){
  if(getWindowsSize(&E.EditorRows ,&E.EditorCols)==-1)die("getWindowsSize");
}

