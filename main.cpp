/*
  this program is a simple ftp client for unix.
  only file commands can use.
  type help for more help information.
  you can use or modify it .
  hope that it will be useful.
  >_<
*/
#include "myftp.h"
int main(int argc,char **argv)
{
  sigset_t signew;
  sigset_t sigold;
  sigfillset(&signew);
  sigprocmask(SIG_SETMASK,&signew,&sigold);
  int socket = -1;
  if(argc != 2)
  {
    printf("you should set a ftp server address\n");
    exit(0);
  }
  
  myftp ftp;
  while(socket == -1)
  {
    socket = ftp.Login(argv[1]); 
  }

  ftp.GetCommand(socket);
  sigprocmask(SIG_SETMASK,&sigold,NULL);
}
