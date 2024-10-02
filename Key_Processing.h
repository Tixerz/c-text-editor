
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
    

    case 'a' :
      exit(0);
      break;
  }
}

