PROG=ZelmaHash.exe
SOURCES=main.cpp src/user.cpp src/zelmaHasher.cpp src/dbConnection.cpp src/userInterface.cpp src/zelmaCracker.cpp
DEPS=
CC=g++
CFLAGS=-Wall -std=c++20
LDFLAGS=-lssl -lcrypto
DEBUG?=

ifeq ($(DEBUG), 1)
    CFLAGS += -g
    OUTPUTDIR=bin/debug
    PROG=$(OUTPUTDIR)/ZelmaHash-debug.exe
else
    CFLAGS += -g0 -O3
    OUTPUTDIR=bin/release
    PROG=$(OUTPUTDIR)/ZelmaHash.exe
endif

OBJDIR=$(OUTPUTDIR)/obj
OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

$(shell mkdir -p $(OUTPUTDIR) $(OBJDIR))

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin/debug bin/release
