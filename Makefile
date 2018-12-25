# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall
CCLINK = $(CC) -pthread
OBJS =  main.o Account.o Atm.o Bank.o Log.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o main $(OBJS)
# Creating the object files
Account.o: Account.cpp Account.h
main.o: main.cpp Account.h
Atm.o: Atm.cpp Atm.h
Bank.o: Bank.cpp Bank.h
Log.o: Log.cpp Log.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

