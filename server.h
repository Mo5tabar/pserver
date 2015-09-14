/*
 * File: server.h
 * Author: Sami Metoui
 * Description: server class 
 * License: GPL 3 (http://www.gnu.org/licenses/gpl.html)
 */

#ifndef __server_h__
#define __server_h__ 

#include <sys/types.h>		
#include <sys/socket.h> 	
#include <netinet/in.h>		

#include <iostream>

class server;

struct thread_param {
 server* ser;
 int soc;
 struct sockaddr_in cliAddr;
};

class server {
private:
 int port;
 int ListeningSocket;
 bool running;
 struct sockaddr_in ServerAddr;

 void* ClientThread(int, sockaddr_in);

public:
 server(int);
 int init();
 int start();
 int pause();

 static void* ThreadLauncher(void *p) {
  struct thread_param *Obj = reinterpret_cast<struct thread_param*> (p);
  server *s = Obj->ser;
  return s->ClientThread(Obj->soc, Obj->cliAddr);
 }
};

#endif
