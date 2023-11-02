CC:=clang++
CFLAGS:=-Wall -c

DEBUGFLAGS:= 

RM:= rm -f
OBJS:= main.o lexer.o token.o alerts.o

all: main

debug: DEBUGFLAGS += -g
debug: CFLAGS += $(DEBUGFLAGS)
debug: main

main: main.o ./lexer/lexer.o ./lexer/token.o alerts.o
	$(CC) $(DEBUGFLAGS) $(OBJS) -o main
	$(info CREATED main)

main.o:	main.cpp
	$(CC) $(CFLAGS) main.cpp

./lexer/lexer.o: ./lexer/lexer.cpp ./lexer/lexer.hpp
	$(CC) $(CFLAGS) lexer/lexer.cpp

./lexer/token.o: ./lexer/token.cpp ./lexer/token.hpp
	$(CC) $(CFLAGS) lexer/token.cpp

alerts.o: alerts.cpp alerts.hpp
	$(CC) $(CFLAGS) alerts.cpp

clean:
	$(RM) $(OBJS)