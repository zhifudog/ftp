#ifndef _MYFTP_H_
#define _MYFTP_H_
#include"public.h"
#include"command.h"
class myftp:public Public
{
  public:
    myftp();
    ~myftp();
    int Register(char *Addr);
    int Login(char *Addr);
    void GetCommand(int socket);
    char *ReadLine(char *Msg);
    int AnalyzeMsg(char *Msg);
    void split(const string Msg,char Split,vector<string> &Result);
  protected:
    struct sockaddr_in m_ftpaddr;

};

#endif
