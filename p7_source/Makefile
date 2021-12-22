CC = gcc
CFLAGS = -g -std=c99 -Wall

LIB_FILES = lib/tables.c lib/translate_utils.c lib/translate.c assembler-src/assembler_utils.c
ASSEMBLER_FILES = $(LIB_FILES)
LINKER_FILES = $(LIB_FILES) linker-src/linker_utils.c

all: linker assembler

abc:
	$(CC) $(CFLAGS) -o mytest src/translate_utils.c src/tables.c src/utils.c mytest.c
	./mytest

p7_lib: 
	$(CC) -c ${LIB_FILES}
	ar rs libP7.a *.o
	rm -f *.o

linker: clean
	$(CC) $(CFLAGS) -o linker linker.c $(LINKER_FILES)
assembler: clean
	$(CC) $(CFLAGS) -o assembler assembler.c $(ASSEMBLER_FILES)
clean:
	rm -f *.o linker assembler
