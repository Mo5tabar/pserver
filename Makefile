# File: Makefile
# Author: Sami Metoui 
# Description: posixServer Makefile
# License: GPL 3 (http://www.gnu.org/licenses/gpl.html)

all: server

server: server.o main.o
	g++ -o server server.o main.o -lpthread

server.o: server.cpp
	g++ -o server.o -c server.cpp -W -Wall -ansi -pedantic

main.o: main.cpp server.h
	g++ -o main.o -c main.cpp -W -Wall -ansi -pedantic

clean:
	rm -rf *.o

mrproper: clean
	rm -rf server
