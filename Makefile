CC = gcc
CFLAGS = -O1 -g -Wall -lpthread
SRC = $(wildcard src/*.c)
INCLUDE = src/server
OUTPUT = out/main

all: ${SRC}
	${CC} ${CFLAGS} -I ${INCLUDE} ${SRC} -o ${OUTPUT}