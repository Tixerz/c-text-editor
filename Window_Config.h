#include <sys/ioctl.h>

int getWindowsSize(int* rows , int*cols){
  struct winsize ws;
  if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws)==-1){return -1;}
  else{
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
  }
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


void initEditor(){
  if(getWindowsSize(&E.EditorRows ,&E.EditorCols)==-1)die("getWindowsSize");
}

