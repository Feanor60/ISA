# @file:	Makefile
# @author:	Vojtěch Bůbela
# @login:	xbubel08
# @brief:	Makefile for ISA project - TFTP client

# compiler
CXX = g++
CC = $(CXX)

# dirs
SRC_DIR := ./src
OBJ_DIR := ./obj

# flags
CPP_FLAGS = -g -Wall -Werror
LD_FLAGS = -lnet

# source files
SRC_FILES = $(SRC_DIR)/*.cpp
HEADER_FILES = $(SRC_DIR)/*.hpp

# target file
TARGET = mytftpclient 

all: $(TARGET)

$(TARGET): $(SRC_FILES) #$(HEADER_FILES)
	$(CC) $(CPP_FLAGS) -o $(TARGET) $(SRC_FILES) $(LD_FLAGS)

clean:
	rm $(TARGET)

