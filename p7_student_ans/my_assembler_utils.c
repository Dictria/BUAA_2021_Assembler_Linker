/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * my_assembler_utils.c
 * Assembler Submission
 *****************************************************************/
#include "my_assembler_utils.h"
#include "assembler-src/assembler_utils.h"
#include "lib/translate_utils.h"

#include <string.h>
#include <stdlib.h>

/*
 * This function reads .data symbol from INPUT and add them to the SYMTBL
 * Note that in order to distinguish in the symbol table whether a symbol
 * comes from the .data segment or the .text segment, we append a % to the
 * symbol name when adding the .data segment symbol. Since only underscores and
 * letters will appear in legal symbols, distinguishing them by adding % will
 * not cause a conflict between the new symbol and the symbol in the assembly file.
 *
 * Return value:
 * Return the number of bytes in the .data segment.
 */
int read_data_segment(FILE *input, SymbolTable *symtbl) {
    char buf[ASSEMBLER_BUF_SIZE];
    int byte_offset = 0;
    fgets(buf, ASSEMBLER_BUF_SIZE, input);
    if (strcmp(strtok(buf, ASSEMBLER_IGNORE_CHARS), ".data") != 0) {
        printf("Error - get .data\n");
        return -1;
    }
    while (fgets(buf, ASSEMBLER_BUF_SIZE, input)) {
        int has_comment = strchr(buf, '#') != NULL;
        skip_comment(buf);
        char *token = strtok(buf, ASSEMBLER_IGNORE_CHARS);
        if (token == NULL && has_comment == 0) {
            return byte_offset;
        } else if (token == NULL && has_comment == 1) {
            continue;
        }
        char *name = token;
        size_t len = strlen(name);
        if (name[len - 1] != ':') {
            printf("Error - get global label\n");
            continue;
        }
        name[len - 1] = '\0';
        if (!is_valid_label(name)) {
            printf("Error - invalid gloal label\n");
            continue;
        }
        token = strtok(NULL, ASSEMBLER_IGNORE_CHARS);
        if (strcmp(token, ".space") != 0) {
            printf("Error - invalid directive\n");
            continue;
        }
        token = strtok(NULL, ASSEMBLER_IGNORE_CHARS);
        long int space;
        translate_num(&space, token, 0, 31);
        char label[ASSEMBLER_BUF_SIZE];
        sprintf(label, "%%%s", name);
        add_to_table(symtbl, label, byte_offset);
        byte_offset += space;
    }
    return -1;
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
 * allocation fails, you should call allocation_failed().And alloction_failed()
 * will print error message and exit with error code 1.
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

/*
 * Convert lui instructions to machine code. Note that for the imm field of lui,
 * it may be an immediate number or a symbol and needs to be handled separately.
 * Output the instruction to the **OUTPUT**.(Consider using write_inst_hex()).
 * 
 * Return value:
 * 0 on success, -1 otherwise.
 * 
 * Arguments:
 * opcode:     op segment in MIPS machine code
 * args:       args[0] is the $rt register, and args[1] can be either an imm field or 
 *             a .data segment label. The other cases are illegal and need not be considered
 * num_args:   length of args array
 * addr:       Address offset of the current instruction in the file
 */
int write_lui(uint8_t opcode, FILE *output, char **args, size_t num_args, uint32_t addr, SymbolTable *reltbl) {
    if (num_args != 2) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    long int imm;
    int rs = 0;
    int rt = translate_reg(args[0]);
    int err = translate_num(&imm, args[1], 0, UINT16_MAX);

    if (rt == -1) {
        printf("Invalid register. \n");
        return -1;
    }
    if (err == -1) {
        add_to_table(reltbl, args[1], addr);
        imm = 0;
    }

    uint32_t instruction = 0;

    instruction = instruction | imm;
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (opcode << 26);

    write_inst_hex(output, instruction);
    return 0;
}

