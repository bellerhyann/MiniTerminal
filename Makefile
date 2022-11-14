DEBUG = -g
OPTS = $(DEBUG) -Wall -ansi -pedantic -std=c++20

CC=g++ $(OPTS) -c
LN=g++

OBJS = main.o Command.o Token.o TokenStream.o

proj1: $(OBJS)
	$(LN) -o proj1 $(OBJS)

main.o: main.cpp Command.hpp Token.hpp TokenStream.hpp
	$(CC) main.cpp

Command.o: Command.cpp Command.hpp TokenStream.hpp Token.hpp
	$(CC) Command.cpp

Token.o: Token.cpp Token.hpp
	$(CC) Token.cpp

TokenStream.o: TokenStream.cpp Token.hpp TokenStream.hpp
	$(CC) TokenStream.cpp

clean:
	/bin/rm -rf *~ $(OBJS) proj1
