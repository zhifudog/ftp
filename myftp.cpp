#include "myftp.h"
myftp::myftp()
{
}

myftp::~myftp()
{
  
}

int myftp::Register(char *Addr)
{
  int mysocket;
  if((mysocket = socket(AF_INET,SOCK_STREAM,0)) == -1)
  {
    printf("create socket error \n");
    exit(-1);
  }
  m_ftpaddr.sin_family = AF_INET;
  m_ftpaddr.sin_port = htons(21);
  m_ftpaddr.sin_addr.s_addr = inet_addr(Addr);
  return mysocket;
}

int myftp:: AnalyzeMsg(char *Msg)
{
   int MsgNum;
   char Num[4];
   strncpy(Num,Msg,3);
   Num[4] = '\0';
   MsgNum = atoi(Num);
   return MsgNum;
}

int myftp::Login(char *Addr)
{
  int MsgNum;
  int Socket;
  char MsgRecv[MSG_SIZE];
  char MsgSnd[MSG_SIZE];
  char Timp[10];
  Socket = Register(Addr);
  Connect(Socket,m_ftpaddr);
  recv_msg_ftp(Socket,MsgRecv);
  MsgNum = AnalyzeMsg(MsgRecv);
  if(MsgNum == 220)
  {
    printf("USER:");
    fgets(Timp,sizeof(Timp),stdin);
    sprintf(MsgSnd,"USER %s",Timp);
    snd_msg_ftp(Socket,MsgSnd);
    RESET(MsgRecv);
    recv_msg_ftp(Socket,MsgRecv);
    MsgNum = AnalyzeMsg(MsgRecv);
    if(MsgNum == 331)
    {
       RESET(MsgSnd);
       printf("PASSWORD:");
       RESET(Timp);
       fgets(Timp,sizeof(Timp),stdin);
       sprintf(MsgSnd,"PASS %s",Timp);
       snd_msg_ftp(Socket,MsgSnd);
       RESET(MsgRecv);
       sleep(1);
       recv_msg_ftp(Socket,MsgRecv);
       MsgNum = AnalyzeMsg(MsgRecv);
       if(MsgNum == 230)
       {
	 return Socket;
       }
       else
       {
         printf("password error\n");
	     return -1;
       }
    }//end if(MsgNum == 331)
    else
    {
      printf("user error\n");
      return -1;
    }
  }//end if(MsgNum == 220)
  else
  {
    printf("connect error\n");
    return -1;
  }
}

void myftp::GetCommand(int socket)
{
  char Command[MSG_SIZE];
  char MsgSnd[MSG_SIZE];
  char MsgRecv[MSG_SIZE];
  vector<string> Parameter;
  ICommand *p_IComm = NULL;
  ICommand cmd;
  while(1)
  {
    Parameter.clear();
    printf("myftp>");
    RESET(Command);
    RESET(MsgSnd);
    RESET(MsgRecv);
    ReadLine(Command);
    string CommandTimp = Command;
    split(CommandTimp,' ',Parameter);
    if(!Parameter.empty())
    {
       p_IComm = cmd.getCommandProcesser(Parameter[0]);
       if(p_IComm != NULL)
       {
         vector<string> option(Parameter.begin()+1,Parameter.end());
         p_IComm->execute(socket,option,m_ftpaddr);
       }
       delete p_IComm;
    }//end if(!Parameter.empty())
    if(strcmp(Command,"quit") == 0)
    {
      printf("thanks for using\n");
      break;
    }
  }//end while(1)
}

char *myftp::ReadLine(char *Msg)
{
   int len = 0;
   int ch = 0;
   while((ch = getchar()) != '\n') 
   {
     Msg[len] = ch;
     len++;
   }
   len = strlen(Msg);
   Msg[len] = '\0';
   return Msg;	
}

void myftp::split(const string Msg,char Split,vector<string> &Result)
{
  size_t first = 0;
  size_t pos =0;
  if(!Msg.empty())
  {
  for(first=0; first < Msg.size(); first =pos+1)
  {
    pos = first;
    while(Msg[pos]!= Split && pos<Msg.size())
    {
      pos++;
    }
    string str = Msg.substr(first,pos-first);
    Result.push_back(str);
  }
  }
}
