/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * tables.c
 * Tools for Symbol Table
 *****************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

#define INITIAL_SIZE 5
#define SCALING_FACTOR 2

/*******************************
 * Helper Functions
 *******************************/

/* raise an error */
void allocation_failed() {
    printf("Error: allocation failed\n");
    exit(1);
}

/* raise an error */
void addr_alignment_incorrect() {
    printf("Error: address is not a multiple of 4.\n");
}

/* raise an error */
void name_already_exists(const char *name) {
    printf("Error: name '%s' already exists in table.\n", name);
}

/* Output symbols and offsets to the specified file */
void write_symbol(FILE *output, uint32_t addr, const char *name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
 * table. Multiple SymbolTables may exist at the same time.
 * If memory allocation fails, you should call allocation_failed().
 * Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
 * to store this value for use during add_to_table().
 */
SymbolTable *create_table(int mode) {
    SymbolTable *table = (SymbolTable *) malloc(sizeof(SymbolTable));
    if (!table) {
        allocation_failed();
    }
    table->len = 0;
    table->cap = INITIAL_SIZE;
    table->tbl = (Symbol *) malloc(table->cap * sizeof(Symbol));
    if (!table->tbl) {
        allocation_failed();
    }
    table->mode = mode;
    return table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable *table) {
    Symbol *curr = table->tbl;
    int counter = 0;
    while (counter < table->len) {
        free(curr->name);
        curr = curr + 1;
        counter++;
    }
    free(table->tbl);
    free(table);
}

/* A suggested helper function for copying the contents of a string. */
static char *create_copy_of_str(const char *str) {
    size_t len = strlen(str) + 1;
    char *buf = (char *) malloc(len);
    if (!buf) {
        allocation_failed();
    }
    strncpy(buf, str, len);
    return buf;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
 * ADDR is given as the byte offset from the first instruction. The SymbolTable
 * must be able to resize itself as more elements are added.
 *
 * Note that NAME may point to a temporary array, so it is not safe to simply
 * store the NAME pointer. You must store a copy of the given string.
 *
 * If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
 * return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
 * in the table, you should call name_already_exists() and return -1. If memory
 * allocation fails, you should call allocation_failed().
 *
 * Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable *table, const char *name, uint32_t addr) {
    //unique entries
    if (table->mode == 1) {
        //if the name already exists
        if (get_addr_for_symbol(table, name) != -1) {
            name_already_exists(name);
            return -1;
        }
    }

    //addr is not word-alligned. (a multiple of 4 b/c machine architecture we use is byte-addressed. The addresses are one byte away from each other. increments in bytes.)
    if ((addr % 4) != 0) {
        addr_alignment_incorrect();
        return -1;
    }

    if (table->len == table->cap) {
        //resize table
        table->cap = table->cap * SCALING_FACTOR;
        table->tbl = (Symbol *) realloc(table->tbl, table->cap * sizeof(Symbol));

        if (!table->tbl) {
            allocation_failed();
            return -1;
        }
    }

    // since name may point to a temporary array, must create a copy of it.
    char *name_copy;
    name_copy = create_copy_of_str(name); //this function mallocs data itself

    if (!name_copy) {
        allocation_failed();
        return -1;
    }

    //store the symbol name and address in the table
    Symbol *symbol;
    //(table->tbl)[table->len] = &symbol;

    symbol = &(table->tbl)[table->len];

    symbol->name = name_copy;
    symbol->addr = addr;
    (table->len)++;
    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
 * NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->len; i++) {
        if (strcmp(name, (table->tbl)[i].name) == 0) {
            return (table->tbl)[i].addr;
        }
    }
    return -1;
}

/* Returns the address symbol name of the given address (byte offset). If a symbol
 * with address ADDR is not present in TABLE, return NULL.
 */
const char *get_symbol_for_addr(SymbolTable *table, const int32_t addr) {
    for (int i = 0; i < table->len; i++) {
        if (addr == ((table->tbl)[i].addr)) {
            return (table->tbl)[i].name;
        }
    }
    return NULL;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
 * perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable *table, FILE *output) {
    for (int i = 0; i < table->len; i++) {
        write_symbol(output, (table->tbl)[i].addr, (table->tbl)[i].name);
    }
}


