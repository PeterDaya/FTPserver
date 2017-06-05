CC  = gcc
CXX = g++

INCLUDES =
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS =

ftpserver:

ftpserver.o:

.PHONY: valgrind
valgrind: ftpserver
	valgrind --leak-check=yes ./ftpserver 20000

.PHONY: clean
clean:
	rm -f *.o *~ a.out core ftpserver
.PHONY: all
all: clean default

run: ftpserver
	./ftpserver 15000
