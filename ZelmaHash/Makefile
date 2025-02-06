PROG=ZelmaHash.exe
SOURCES=main.cpp src/user.cpp src/zelmaHasher.cpp src/dbConnection.cpp src/userInterface.cpp src/zelmaCracker.cpp
DEPS=
CC=g++
CFLAGS=-Wall -std=c++20
LDFLAGS=-lssl -lcrypto
DEBUG?=1

ifeq ($(DEBUG), 1)
    CFLAGS += -g
    OUTPUTDIR=bin/debug
    PROG=programmet-ZelmaHash-debug.exe
else
    CFLAGS += -g0 -O3
    OUTPUTDIR=bin/release
endif

OBJS =  $(SOURCES:.cpp=.o)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $<