/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * linker_utils.c
 * Utilities for linker
 *****************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../my_assembler_utils.h"
#include "../lib/translate_utils.h"
#include "../my_linker_utils.h"

/*
 * Compute number of bytes that the text section takes up. This fuction assumes
 * that when called, the file pointer is currently at the beginning of the text
 * section. It also assumes that there will be one instruction per line, and
 * that the .text section ends with a blank line.
 *
 * Return value:
 * size of the text section in bytes.
 *
 * Arguments:
 * input: file pointer, must be pointing to the beginning of the text section.
 */
int calc_text_size(FILE *input) {
    int counter = 0;
    char buf[LINKER_BUF_SIZE];

    while (fgets(buf, LINKER_BUF_SIZE, input)) {
        char *token = strtok(buf, LINKER_IGNORE_CHARS);
        if (!token) {
            return counter;
        }
        counter += 4;
    }
    return -1;
}

/*
 * Calculates the number of bytes occupied by the .data of the current file.
 * Note that after the assembler has processed it, we have already calculated
 * the number of bytes in the .data segment. It is saved at the beginning of
 * the .out file as ".data \n bytes".
 *
 * Return value:
 * Returns the number of bytes occupied by the .data of the file INPUT
 */
int calc_data_size(FILE *input) {
    long ret = 0;
    char buf[LINKER_BUF_SIZE];
    while (fgets(buf, LINKER_BUF_SIZE, input)) {
        char *token = strtok(buf, LINKER_IGNORE_CHARS);
        if (!token) {
            break;
        }
        if (translate_num(&ret, token, 0, UINT16_MAX) == -1) {
            printf("Error - parse data section size failed\n");
            return -1;
        }
    }
    return ret;
}

/*
 * Add entries from the .symbol OR .relocation section into the SymbolTable.
 * Each line must be of the format "<number>\t<string>\n", and the section ends
 * with a blank line. The file pointer must be at the beginning of the section
 * (the line immediately after .symbol or .relocation).
 *
 * Similarly, when we add symbols to the symbol table, we need to process the symbols
 * in the .data segment and the .text segment separately. The symbols in the .data
 * segment should have an offset based on base_data_offset, and the symbols in the
 * .text segment should have an offset based on base_text_offset. The two are
 * distinguished by the character % that the assembler adds to the beginning of the
 * .data segment symbols
 * The function will DELETE the % if it is a .data segment symbol. Then add the symbol
 * to the table
 *
 * Return:
 * 0 if no errors, -1 if error.
 */
int add_to_symbol_table(FILE *input, SymbolTable *table, uint32_t base_text_offset, uint32_t base_data_offset) {
    char buf[LINKER_BUF_SIZE];
    while (fgets(buf, LINKER_BUF_SIZE, input)) {
        char *token = strtok(buf, LINKER_IGNORE_CHARS);
        if (!token) {
            break;
        }

        int pos = 0;
        while (token[pos] != '\0') {
            if (!isdigit(token[pos])) {
                return -1;
            }
            pos++;
        }

        uint32_t offset = atoi(token);
        char *name = strtok(NULL, LINKER_IGNORE_CHARS);
        char *sub;
        if ((sub = strchr(name, '%')) != NULL) {
            offset += base_data_offset;
            add_to_table(table, sub + 1, offset);
        } else {
            offset += base_text_offset;
            add_to_table(table, name, offset);
        }
    }
    return 0;
}
