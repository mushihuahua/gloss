CC:=clang++
CFLAGS:=-Wall -std=c++17 -c -I../include/

DEBUGFLAGS:= 

RM:= rm -f
OBJS:= main.o ./lexer/lexer.o ./lexer/token.o alerts.o ast.o

all: main

debug: DEBUGFLAGS += -g
debug: CFLAGS += $(DEBUGFLAGS)
debug: main

main: $(OBJS)
	$(CC) $(DEBUGFLAGS) $(OBJS) -o gloss
	$(info CREATED main)

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

ast.o: parser/ASTs/ExprAST.hpp parser/visitors/AstPrinterVisitor.cpp
	$(CC) $(CFLAGS) parser/visitors/AstPrinterVisitor.cpp -o ast.o

clean:
	$(RM) $(OBJS) gloss
