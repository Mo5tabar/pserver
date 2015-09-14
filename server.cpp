/*
 * File: server.cpp
 * Author: Sami Metoui
 * Description: server class methods
 * License: GPL 3 (http://www.gnu.org/licenses/gpl.html)
 */

#include <pthread.h> 

#include <sys/types.h>		
#include <sys/socket.h>		
#include <netinet/in.h>		

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>


#include <stdio.h>
#include <string.h>

#include <iostream>
#include "server.h"


#define _MAX_HOST_LENGTH_ 100
#define MAXBUF 100

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

using namespace std;

/************************************************
 * Classe server builder
 * Input: listening port number
 ************************************************/
server::server(int p) {
 port = p;
 running = false;
}

/************************************************
 * Init the server
 * Output: 0 ok, and 1 when an error occure
 ************************************************/
int server::init() {
 struct in_addr MyAddress;
 struct hostent *host;
 char HostName[_MAX_HOST_LENGTH_];


 if (gethostname(HostName, _MAX_HOST_LENGTH_) == SOCKET_ERROR) {
  cerr << "gethostname() error " << errno << endl;
  return 1;
 }

 if ((host = gethostbyname(HostName)) == NULL) {
  cerr << "gethostbyname() error " << errno << endl;
  return 1;
 }

 memcpy(&MyAddress.s_addr, host->h_addr_list[0], sizeof ( MyAddress.s_addr));

 ServerAddr.sin_family = AF_INET;
 ServerAddr.sin_port = htons(port);
 ServerAddr.sin_addr.s_addr = inet_addr(inet_ntoa(MyAddress));

 cout << "server init ok!" << endl;
 return 0;
}

/************************************************
 * Start the server
 * Output: 0 ok, and 1 when an error occure
 ************************************************/
int server::start() {
 //SOCKADDR_IN                 ClientAddr;
 struct sockaddr_in ClientAddr;
 int ClientAddrLen;

 //HANDLE                      hProcessThread;
 pthread_t t;

 //SOCKET                      NewConnection;
 int NewConnection;

 struct thread_param p;


 if ((ListeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {

  cerr << "Unable to create socket. Error : " << errno << endl;

  return 1;
 }

 if (bind(ListeningSocket, (sockaddr *) & ServerAddr, sizeof ( ServerAddr)) == SOCKET_ERROR) {


  cerr << "Bind faild. Error : " << errno << endl;

  cerr << "Port may be used by another process " << endl;
  close(ListeningSocket);


  return 1;
 }

 if (listen(ListeningSocket, 5) == SOCKET_ERROR) {

  cerr << "listen() fail - Error : " << errno << endl;
  close(ListeningSocket);
  return 1;
 }

 cout << "Server start listening port : " << port << endl;
 running = true;
 ClientAddrLen = sizeof ( ClientAddr);

 while (running) {

  if ((NewConnection = accept(ListeningSocket, (sockaddr*) & ClientAddr, (socklen_t*) & ClientAddrLen)) == INVALID_SOCKET) {
   cerr << "accept() fail - Error : " << errno << endl;
   close(ListeningSocket);
   return 1;
  }

  p.ser = this;
  p.soc = NewConnection;
  p.cliAddr = ClientAddr;

  cout << "Client connected ::  IP : " << inet_ntoa(ClientAddr.sin_addr) << " ,port = " << ntohs(ClientAddr.sin_port) << endl;

  if (pthread_create(&t, NULL, server::ThreadLauncher, &p) != 0) {
   cerr << "CreateThread fail -Error : " << endl;
  }
 }

 return 0;
}

int server::pause() {
 running = false;
 cout << "Serveur pause" << endl;
 close(ListeningSocket);
 return 0;
}

/* ======================================================================== */
/* ========================== thread proc ================================= */

/* ======================================================================== */


void* server::ClientThread(int soc, struct sockaddr_in cliAddr) {

 int rlen;
 char tbuf[MAXBUF];

 cout << "Client thread start" << endl;

 do {
  rlen = recv(soc, tbuf, MAXBUF, 0);
  if (rlen > 0) {
   tbuf[rlen] = '\0';
   cout << tbuf << endl;
   send(soc, tbuf, rlen, 0);
  } else break;

 } while (1);
 shutdown(soc, 2);
 cout << "Disconnected from client : " << inet_ntoa(cliAddr.sin_addr) << endl;

 return NULL;
}
