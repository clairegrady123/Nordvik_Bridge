COMPILER = gcc
CFLAGS = -Wall -pedantic -pthread
EXES =  nordvik_bridge

all: ${EXES}

nordvik_bridge: nordvik_bridge.c 
	${COMPILER} ${CFLAGS} -o nordvik_bridge nordvik_bridge.c

%.o: %.c %.h  makefile
	${COMPILER} ${CFLAGS} $< -c

clean:
	rm -f *.o *~ ${EXES}

run:
	./nordvik_bridge
