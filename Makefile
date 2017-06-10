CXX = g++
FLAGS = -std=c++11
CFLAGS = -g -Wall
LIBS = -lsqlite3

FILEPATH := $(realpath $(lastword $(MAKEFILE_LIST)))
CURDIR := $(shell cd $(dir $(FILEPATH));pwd)
DBDIR = $(CURDIR)/db
TESTDIR = $(CURDIR)/unittest
INC = -I$(CURDIR)

BIN = filters
OBJ = grpinfo.o \
	inputfile.o \
	$(DBDIR)/db.o


all: $(BIN)

$(BIN): $(OBJ) filters.o
	$(CXX) $(OBJ) filters.o  -o $(BIN) $(LIBS)

.cpp.o:
	$(CXX) $(FLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN) filters.o $(TESTDIR)/*.catch

catch: $(patsubst $(TESTDIR)/%.cpp, $(TESTDIR)/%.catch, $(wildcard $(TESTDIR)/*.cpp))

$(TESTDIR)/%.catch: $(TESTDIR)/%.cpp
	$(CXX) $(FLAGS) $(CFLAGS) $< $(OBJ) $(INC) -o $@ 

.PHONY: all clean catch
