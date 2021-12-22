CC = gcc
CFLAGS = -g -std=c99 -Wall

LIB_FILES = lib/tables.c lib/translate_utils.c lib/translate.c assembler-src/assembler_utils.c  linker-src/linker_utils.c
LIB_OBJS = tables.o translate_utils.o translate.o assembler_utils.o linker_utils.o
ASSEMBLER_FILES =  my_assembler_utils.c
LINKER_FILES =  my_linker_utils.c

all: assembler linker

p7_lib: $(LIB_FILES)
	$(CC) -c $(LIB_FILES)
	ar rs libP7.a $(LIB_OBJS)

assembler: p7_lib $(ASSEMBLER_FILES)
	$(CC) $(CFLAGS) -o assembler assembler.c $(ASSEMBLER_FILES) libP7.a

linker: p7_lib $(LINKER_FILES)
	$(CC) $(CFLAGS) -o linker linker.c $(LINKER_FILES) $(ASSEMBLER_FILES) libP7.a

clean:
	del *.o linker assembler libP7.a
