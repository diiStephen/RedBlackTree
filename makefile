# makefile for rbtree

.SUFFIXES: .o .c .h

vpath %.c src tests
vpath %.h include

CC        = gcc
CFLAGS    = -c -Wall -pedantic -Wextra -g
SLIBFLAGS = -c -Wall -pedantic -Wextra -g -fPIC
LFLAGS    = -shared

TARGET     = all
INCLUDEDIR = include
INCLUDE    = -I $(INCLUDEDIR)
SRC        = src
NAME       = rbtree.so
TNAME      = rbtree-tests
ENAME      = test-engine
BUILD      = build

default: $(TARGET)

all: rbtree.o errors.o
	@echo 'Starting linking process...'
	$(CC) $(LFLAGS) rbtree.o errors.o -o $(BUILD)/$(NAME)
	@echo '...done!'

engine: test-engine.o rbtree.o errors.o
	@echo 'Linking test engine program...'
	$(CC) test-engine.o rbtree.o errors.o -o $(BUILD)/$(ENAME)
	@echo '...done!'

test-engine.o: test-engine.c rbtree.h
	@echo 'Building test-engine module...'
	$(CC) $(CFLAGS) $(INCLUDE) $<
	@echo '...done!'

rbtree.o: rbtree.c rbtree.h errors.h
	@echo 'Building RBT module...'
	$(CC) $(SLIBFLAGS) $(INCLUDE) $<
	@echo '...done!'

errors.o: errors.c errors.h
	@echo 'Building errors module...'
	$(CC) $(SLIBFLAGS) $(INCLUDE) $<
	@echo '...done!'

clean:
	@echo 'Cleaning...'
	rm -f *.o
	rm -f core
	rm -f ${INCLUDEDIR}/*.h~
	rm -f ${SRC}/*.c~
	rm -f makefile~
	@echo '...done!'

cleanTEX:
	@echo 'Removing latex temp. files...'
	rm -f *.aux
	rm -f *.log
	rm -f *.out
	rm -f *.pdf
	@echo '...done!'
