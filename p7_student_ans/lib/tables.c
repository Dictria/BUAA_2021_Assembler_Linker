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
const int SCALING_FACTOR = 2;

#define INITIAL_SIZE 5

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
char *create_copy_of_str(const char *str) {
    size_t len = strlen(str) + 1;
    char *buf = (char *) malloc(len);
    if (!buf) {
        allocation_failed();
    }
    strncpy(buf, str, len);
    return buf;
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
 * with address ADDR is not present in TABLE, return -1.
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


