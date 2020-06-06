# makefile for rbtree

.SUFFIXES: .o .c .h

vpath %.c src tests
vpath %.h include

CC     = gcc
CFLAGS = -c -Wall -pedantic -Wextra -g

TARGET     = all
INCLUDEDIR = include
INCLUDE    = -I $(INCLUDEDIR)
SRC        = src
NAME       = rbtree
TNAME      = rbtree-tests
ENAME      = test-engine
BUILD      = build

default: $(TARGET)

all: rbtree.o
	@echo 'Starting linking process...'
	$(CC) rbtree.o -o $(BUILD)/$(NAME)
	@echo '...done!'

test: rbtree-tests.o rbtree.o errors.o
	@echo 'Starting linking process for test files...'
	$(CC) rbtree-tests.o rbtree.o errors.o -o $(BUILD)/$(TNAME)
	@echo '...done!'

engine: test-engine.o rbtree.o errors.o
	@echo 'Linking test engine program...'
	$(CC) test-engine.o rbtree.o errors.o -o $(BUILD)/$(ENAME)
	@echo '...done!'

test-engine.o: test-engine.c rbtree.h
	@echo 'Building test-engine module...'
	$(CC) $(CFLAGS) $(INCLUDE) $<
	@echo '...done!'

rbtree-tests.o: rbtree-tests.c rbtree.h
	@echo 'Building Red-Black Tree tests!'
	$(CC) $(CFLAGS) $(INCLUDE) $<
	@echo 'done!'

rbtree.o: rbtree.c rbtree.h errors.h
	@echo 'Building rbtree.'
	$(CC) $(CFLAGS) $(INCLUDE) $<
	@echo 'done!'

errors.o: errors.c errors.h
	@echo 'Building error module'
	$(CC) $(CFLAGS) $(INCLUDE) $<
	@echo 'done!'

clean:
	@echo 'Cleaning...'
	rm -f *.o
	rm -f core
	rm -f ${INCLUDEDIR}/*.h~
	rm -f ${SRC}/*.c~
	rm -f makefile~
	@echo '...done!'
