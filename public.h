#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<iostream>
#include<vector>

using namespace std;
#define MSG_SIZE 4096
#define RESET(x) memset(x,0,sizeof(x))
class Public
{
   public:
     void recv_msg_ftp(int Socket,char *Msg);
     void snd_msg_ftp(int Socket,char *Msg);
     void Connect(int Socket,sockaddr_in addr);
     int AnalyzeMsg(char *Msg);
   protected:
      struct sockaddr_in m_ftpaddr;
};
#endif
