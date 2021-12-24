/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * my_linker_utils.c
 * Linker Submission
 *****************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "lib/tables.h"
#include "linker-src/linker_utils.h"

/*
 * Builds the symbol table and relocation data for a single file.
 * Read the .data, .text, .symbol, .relocation segments in that order.
 * The size of the .data and .text segments are read and saved in the
 * relocation table of the current file. For the .symbol and .relocation
 * segments, save the symbols in them in the corresponding locations.
 *
 * Return value:
 * 0 if no errors, -1 if error.
 *
 * Arguments:
 * input:            file pointer.
 * symtbl:           symbol table.
 * reldt:            pointer to a Relocdata struct.
 * base_text_offset: base text offset.
 * base_data_offset: base data offset.
 */
int
fill_data(FILE *input, SymbolTable *symtbl, RelocData *reldt, uint32_t base_text_offset, uint32_t base_data_offset) {
    char buf[LINKER_BUF_SIZE];
    while (fgets(buf, LINKER_BUF_SIZE, input)) {
        char *token = strtok(buf, LINKER_IGNORE_CHARS);
        if (!token) {
            return 0;
        }
        if (strcmp(token, ".data") == 0) {
            long sz = calc_data_size(input);
            if (sz < 0) {
                printf("fill_data: calc_data_size failed\n");
                return -1;
            }
            reldt->data_size = sz;
        } else if (strcmp(token, ".text") == 0) {
            int sz = calc_text_size(input);
            if (sz < 0) {
                printf("fill_data: calc_text_size failed\n");
                return -1;
            }
            reldt->text_size = sz;
        } else if (strcmp(token, ".symbol") == 0) {
            if (add_to_symbol_table(input, symtbl, base_text_offset, base_data_offset) < 0) {
                printf("fill_data: add_to_symbol_table failed\n");
                return -1;
            }
        } else if (strcmp(token, ".relocation") == 0) {
            if (add_to_symbol_table(input, reldt->table, 0, 0) < 0) {
                printf("fill_data: add_to_symbol_table failed\n");
                return -1;
            }
        }
    }
    return 0;
}

/*
 * Detect whether the given instruction needs relocation.
 *
 * Return value:
 * 1 if the instruction needs relocation, 0 otherwise.
 *
 * Arguments:
 * offset: Address offset of the instruction in the current file.
 * reltbl: relocation table corresponding to the current file.
 */
int inst_needs_relocation(SymbolTable *reltbl, uint32_t offset) {
    const char *name = get_symbol_for_addr(reltbl, offset);
    return name != NULL;
}

/*
 * Given an instruction that needs relocation, relocate the instruction based on
 * the given symbol and relocation table.
 *
 * Note that we need to handle the relocation symbols for the .data segment and
 * the relocation symbols for the .text segment separately.
 *
 * For the .text segment, the symbols will only appear in the jump instruction
 *
 * For .data, it will only appear in the lui and ori instructions,
 * and we have processed it in the assembler as label@Hi/Lo
 *
 * You should return error if:
 * (1). the addr is not in the relocation table.
 * (2). the symbol name is not in the symbol table.
 * Otherwise, assume that the relocation will perform successfully.
 *
 * Return:
 * the relocated instruction, -1 if error.
 *
 * Arguments:
 * inst: an instruction that needs relocate.
 * offset: the byte offset of the instruction in the current file.
 * symtbl: the symbol table.
 * reltbl: the relocation table.
 */
int32_t relocate_inst(uint32_t inst, uint32_t offset, SymbolTable *symtbl, SymbolTable *reltbl) {
    const char *name = get_symbol_for_addr(reltbl, offset);
    if (name == NULL) {
        return -1;
    }
    char *at = strchr(name, '@');
    if (at == NULL) {
        int32_t addr = get_addr_for_symbol(symtbl, name);
        if (addr == -1) {
            return -1;
        }

        addr = (addr >> 2) & 0x03ffffff;
        inst &= 0xfc000000; // zero bottom 26 bits
        inst |= addr;
    } else {
        *at = '\0';
        at += 1;
        int32_t addr = get_addr_for_symbol(symtbl, name);
        if (addr == -1) {
            return -1;
        }
        if (strcmp(at, "Hi") == 0) {
            addr >>= 16;
        } else if (strcmp(at, "Lo") != 0) {
            return -1;
        }
        addr &= 0x0000ffff;
        inst &= 0xffff0000;
        inst |= addr;
    }
    return inst;
}
