CXX = g++
FLAGS = -std=c++11
CFLAGS = -g -Wall -fPIC
LIBS = -lsqlite3

FILEPATH := $(realpath $(lastword $(MAKEFILE_LIST)))
CURDIR := $(shell cd $(dir $(FILEPATH));pwd)
DBDIR = $(CURDIR)/db
IODIR = $(CURDIR)/io
TESTDIR = $(CURDIR)/unittest
INC = -I$(CURDIR)

BIN = filters
OBJ = grpinfo.o

RPATH = -Wl,-rpath=$(DBDIR) -L$(DBDIR) -Wl,-rpath=$(IODIR) -L$(IODIR) 
RLIBS = -ldb -linputfile

all: db io $(BIN) db

db: $(DBDIR)/db.o
	$(CXX) -shared -o $(DBDIR)/libdb.so $(DBDIR)/db.o 

io: $(IODIR)/inputfile.o
	$(CXX) -shared -o $(IODIR)/libinputfile.so $(IODIR)/inputfile.o 

$(BIN): $(OBJ) filters.o
	$(CXX) $(OBJ) filters.o  -o $(BIN) $(RPATH) $(LIBS) $(RLIBS)

.cpp.o:
	$(CXX) $(FLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN) *.o */*.o $(TESTDIR)/*.catch

catch: $(patsubst $(TESTDIR)/%.cpp, $(TESTDIR)/%.catch, $(wildcard $(TESTDIR)/*.cpp))

$(TESTDIR)/%.catch: $(TESTDIR)/%.cpp
	$(CXX) $(FLAGS) $(CFLAGS) $< $(OBJ) $(INC) -o $@ 

.PHONY: all clean catch
