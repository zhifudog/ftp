#ifndef _COMMAND_H_
#define _COMMAND_H_
#include"public.h"
#include<map>
using namespace std;

class ICommand:public Public
{
  public:
    ICommand();
    ~ICommand();
    virtual void execute(int socket,vector<string> parameter,sockaddr_in addr){};
    void PrintFileData(int socket);
    int RecvFileData(int socket,int fd);
    int CreateFileConnecttion(int socket,sockaddr_in addr); 
    ICommand *getCommandProcesser(string command);
    int CreateFile(const char *Parmeter);
   // int GetCommandNum(string Command);
    int RegisterDataAddr(char *Msg,sockaddr_in addr);
    void GetDataAddr(char *Msg,int &port);
  private:
    vector<string> m_Command;
    struct sockaddr_in m_dataaddr;
    map<string,ICommand*> m_ICommMap; 
};

class cdCommand:public ICommand
{
  public:
    void execute(int socket,vector<string> parameter,sockaddr_in addr);
};

class lsCommand:public ICommand
{
  public:
    void execute(int socket,vector<string> parameter,sockaddr_in addr);
};

class getCommand:public ICommand
{
  public:
  void execute(int socket,vector<string> parameter,sockaddr_in addr);
};

class putCommand:public ICommand
{
  public:
  void execute(int socket,vector<string> parameter,sockaddr_in addr);
};

class helpCommand:public ICommand
{
  public:
  void execute(int socket,vector<string> parameter,sockaddr_in addr);
};

class quitCommand:public ICommand
{
  public:
  void execute(int socket,vector<string> parameter,sockaddr_in addr);
};

#endif
