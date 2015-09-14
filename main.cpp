/*
 * File: main.cpp
 * Author: Sami Metoui
 * Description: server program main function
 * License: GPL 3 (http://www.gnu.org/licenses/gpl.html)
 */

#include <sys/types.h>		
#include <sys/socket.h> 	
#include <netinet/in.h>		

#include <iostream>

#include "server.h"

#define PORTNUM 7

//#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(void) {

 server *MyServer = new server(PORTNUM);
 if (MyServer->init() != 0) {
  cerr << "Unable to init server" << endl;
  return 1;
 }

 if (MyServer->start() != 0) {
  cerr << "Unable to start server" << endl;
  return 1;
 }

 return 0;
}
