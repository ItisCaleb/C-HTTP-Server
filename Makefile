CC = gcc
CFLAGS = -O1 -g -Wall -pthread
SRC = $(wildcard src/*.c)
INCLUDE = src/server
OUTPUT = out/server

${OUTPUT}: ${SRC}
	${CC} ${CFLAGS} -I ${INCLUDE} ${SRC} -o ${OUTPUT}

valgrind: ${OUTPUT}
	valgrind --leak-check=full ${OUTPUT}