int getCursorPosition();


char ReadKey(){
  char c;
  int ret;
   while((ret=read(STDIN_FILENO , &c ,1))!=1){
    if(ret==-1 && errno!= EAGAIN)die("Read function.");
  } 
  return c;
}

void ProcessKey(){
  char c=ReadKey();

  switch (c) {
    case CTRL_KEY('q') :
      write(STDOUT_FILENO , "\x1b[2J" ,4);
      write(STDOUT_FILENO , "\x1b[H" , 3) ;
      exit(0);
	
      break;
    case 'k':
      write(STDOUT_FILENO , "\x1b[B",3);
      int m =getCursorPosition();
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
 // write(STDOUT_FILENO , "~",1);// its fo the last line of the editor
  //int m = getCursorPosition();
}





void RefreshScreen(){

  write(STDOUT_FILENO, "\x1b[2J", 4); // clearing the screen
  write(STDOUT_FILENO , "\x1b[H" , 3); //repositioning the cursor and making it ready for drawing lines
  draw_lines();
  write(STDOUT_FILENO , "\x1b[H" , 3); // repositioning the cursor after drawing lines
}


void initEditor(){
  if(getWindowsSize(&E.EditorRows ,&E.EditorCols)==-1)die("getWindowsSize");
}

