#include"public.h"
void Public::recv_msg_ftp(int Socket,char *Msg)
{
  if(recv(Socket,Msg,MSG_SIZE,0) == -1)
  {
    printf("recv error\n");
  }
  printf("%s",Msg);
}

void Public::snd_msg_ftp(int Socket,char *Msg)
{
  if( send(Socket,Msg,strlen(Msg),0) == -1)
  {
    printf("send error\n");
  }
}

void Public::Connect(int socket,sockaddr_in ftpaddr)
{
  if(connect(socket,(struct sockaddr *)&ftpaddr,sizeof(struct sockaddr_in)) == -1)
  {
    printf("connect error ERR: %s \n",strerror(errno));
    exit(-1);
  }
}

int Public:: AnalyzeMsg(char *Msg)
{
  int MsgNum;
  char Num[4];
  strncpy(Num,Msg,3);
  Num[4] = '\0';
  MsgNum = atoi(Num);
  return MsgNum;
}
