#include"command.h"

ICommand::ICommand()
{
  //it,s a bad idea.
  /* ICommand *p_IComm = NULL;

   p_IComm = new cdCommand();
   m_ICommMap["cd"] = p_IComm;

   p_IComm = new lsCommand();
   m_ICommMap["ls"] = p_IComm;

   p_IComm = new getCommand();
   m_ICommMap["get"]= p_IComm;

   p_IComm = new putCommand();
   m_ICommMap["put"] = p_IComm;

   p_IComm = new helpCommand();
   m_ICommMap["help"] = p_IComm;*/
}

ICommand::~ICommand()
{
  map<string,ICommand*>::const_iterator map_it = m_ICommMap.begin();
  for(;map_it != m_ICommMap.end();map_it++)
  {
    delete m_ICommMap[map_it->first];
  }
}
void ICommand::PrintFileData(int socket)
{
  char MsgRecv[MSG_SIZE];
  RESET(MsgRecv);
  while(read(socket, MsgRecv, MSG_SIZE)  > 0)
  {	   
    printf("%s",MsgRecv);
  }
}

int ICommand::RecvFileData(int socket,int fd)
{
  char MsgRecv[MSG_SIZE];
  int Counter = 0;
  int res;
  while((res=read(socket, MsgRecv, MSG_SIZE)) > 0)
  {
    Counter += res;
    write(fd,MsgRecv,res);
  }
  return Counter;
}

int ICommand::CreateFile(const char *Parameter)
{
  int fd;
  int Flag;
  if(access(Parameter,F_OK) == 0)
  {
    printf("file is existed delete it ? y/n");
    Flag = getchar();
    if((Flag == 'y') ||(Flag == 'Y'))
    {
      if(unlink(Parameter) == -1)
      {
        printf("error delete \n");
      }
      printf("delete ok\n");
    }
    else
    {
      //send message
      return -1;
    }
    }
    if((fd = open(Parameter,O_WRONLY|O_CREAT,0644)) == -1)
    {
      printf("Createfile error");
      return -1;
    }
    return fd;

}

int ICommand::CreateFileConnecttion(int socket,sockaddr_in addr)
{
  char PASV[5];
  char MsgRecv[MSG_SIZE];
  int filesocket;
  sprintf(PASV,"PASV\n");
  snd_msg_ftp(socket,PASV);
  RESET(MsgRecv);
  recv_msg_ftp(socket,MsgRecv);
  filesocket = RegisterDataAddr(MsgRecv,addr);
  return filesocket;
}

int ICommand::RegisterDataAddr(char *Msg,sockaddr_in addr)
{
  int filesocket;
  int port;
  if((filesocket = socket(AF_INET,SOCK_STREAM,0)) == -1)
  {
    printf("create file socket error\n");
    exit(-1);
  }
  GetDataAddr(Msg,port);
  m_dataaddr.sin_family = AF_INET;
  m_dataaddr.sin_port = htons(port);
  m_dataaddr.sin_addr.s_addr = addr.sin_addr.s_addr;//inet_addr(DataAddr);
  Connect(filesocket,m_dataaddr);
  return filesocket;
}

void ICommand::GetDataAddr(char *Msg,int &port)
{
  char *tmp;
  tmp = strrchr(Msg, ',');
  port = atoi(tmp+1);
  *tmp = '\0';
  tmp = strrchr(Msg, ',');
  port += atoi(tmp+1)*256;
}

ICommand *ICommand::getCommandProcesser(string command)
{
  ICommand *p_IComm = NULL;

  p_IComm = new cdCommand();
  m_ICommMap["cd"] = p_IComm;

  p_IComm = new lsCommand();
  m_ICommMap["ls"] = p_IComm;

  p_IComm = new getCommand();
  m_ICommMap["get"]= p_IComm;

  p_IComm = new putCommand();
  m_ICommMap["put"] = p_IComm;

  p_IComm = new helpCommand();
  m_ICommMap["help"] = p_IComm;
  return m_ICommMap.find(command)->second;
  //chose which command
 /* int CommandNum = GetCommandNum(command);
  ICommand *p_IComm = NULL;
  switch(CommandNum)
  {
    case -1:
      break;
    case 0:
      p_IComm = new cdCommand();
      break;
    case 1:
      p_IComm = new lsCommand();
      break;
    case 2:
      p_IComm = new getCommand();
      break;
    case 3:
      p_IComm = new putCommand();
      break;
    case 4:
      p_IComm = new helpCommand();
      break;
    defalut:
      return NULL;
  }
  return p_IComm;*/
}

/*int ICommand::GetCommandNum(string Command)//not use
{
  int i = 0;
  m_Command.push_back("cd");
  m_Command.push_back("ls");
  m_Command.push_back("get");
  m_Command.push_back("put");
  m_Command.push_back("help");
  for(i = 0;i<m_Command.size();i++)
  {
    if(m_Command[i] == Command)
    {
      return i;
    }
  }
  return -1;
}*/

void cdCommand::execute(int socket,vector<string> Command,sockaddr_in addr)
{
  char MsgSnd[MSG_SIZE];
  char MsgRecv[MSG_SIZE];
  for(int i = 0;i<Command.size();i++)
  {
    sprintf(MsgSnd,"CWD %s\n",Command[i].c_str());
    snd_msg_ftp(socket,MsgSnd);
    recv_msg_ftp(socket,MsgRecv);
  }
  return;
} 

void lsCommand::execute(int socket,vector<string> Command,sockaddr_in addr)
{
  int filesocket;
  char MsgSnd[MSG_SIZE];
  char MsgRecv[MSG_SIZE];
    filesocket = CreateFileConnecttion(socket,addr);
    sprintf(MsgSnd,"LIST\n");
    snd_msg_ftp(socket,MsgSnd);
    recv_msg_ftp(socket,MsgRecv);
    PrintFileData(filesocket);
    RESET(MsgRecv);
    recv_msg_ftp(socket,MsgRecv);
}

void getCommand::execute(int socket,vector<string> Command,sockaddr_in addr)
{
  int filesocket;
  int fd;
  int Num;
  char MsgSnd[MSG_SIZE];
  char MsgRecv[MSG_SIZE];
  for(int i = 0;i<Command.size();i++)
  {
    RESET(MsgSnd);
    sprintf(MsgSnd,"RETR %s\n",Command[i].c_str());
    filesocket = CreateFileConnecttion(socket,addr);
    snd_msg_ftp(socket,MsgSnd);
    recv_msg_ftp(socket,MsgRecv);
    Num = AnalyzeMsg(MsgRecv);
    if(Num == 550)
    {
      continue;
    }
    if((fd = CreateFile(Command[i].c_str())) == -1)
    {
      continue;
    }
    RecvFileData(filesocket,fd);
    RESET(MsgRecv);
    close(filesocket);
    recv_msg_ftp(socket,MsgRecv);
    close(fd);
  }
}

void putCommand::execute(int socket,vector<string> Command,sockaddr_in addr)
{
  int filesocket;
  int fd;
  int Num;
  char MsgSnd[MSG_SIZE];
  char MsgRecv[MSG_SIZE];
  for(int i = 0;i<Command.size();i++)
  {
    RESET(MsgSnd);
    sprintf(MsgSnd,"STOR %s\n",Command[i].c_str());
    filesocket = CreateFileConnecttion(socket,addr);
    snd_msg_ftp(socket,MsgSnd);
    recv_msg_ftp(socket,MsgRecv);
    if((fd = open(Command[i].c_str(),O_RDONLY)) == -1)
    {
      printf("CreateFile error\n");
      return;
    }

    RecvFileData(fd,filesocket);
    RESET(MsgRecv);
    close(filesocket);
    recv_msg_ftp(socket,MsgRecv);
    close(fd);
  }
}

void helpCommand::execute(int socket,vector<string> Command,sockaddr_in addr)
{
  printf("     Now only can use these commands:\n");
  printf("                                     put:send  file\n");
  printf("                                     get:download file\n");
  printf("                                     cd:comn in a dir\n");
  printf("                                     ls:dir,s info\n");
  printf("                                     help:help info\n");
}

void quitCommand::execute(int socket,vector<string> Command,sockaddr_in addr)
{
  close(socket);
}
