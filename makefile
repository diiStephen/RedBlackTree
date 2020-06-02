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
BUILD      = build

default: $(TARGET)

all: rbtree.o
	@echo 'Starting linking process...'
	$(CC) rbtree.o -o $(BUILD)/$(NAME)
	@echo '...done!'

test: rbtree-tests.o rbtree.o
	@echo 'Starting linking process for test files...'
	$(CC) rbtree-tests.o rbtree.o -o $(BUILD)/$(TNAME)
	@echo '...done!'

rbtree-tests.o: rbtree-tests.c rbtree.h
	@echo 'Building Red-Black Tree tests!'
	$(CC) $(CFLAGS) $(INCLUDE) $<
	@echo 'done!'

rbtree.o: rbtree.c rbtree.h
	@echo 'Building rbtree.'
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


