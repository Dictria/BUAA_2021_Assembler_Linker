/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * assembler_utils.c
 * Utilities for assembler
 *****************************************************************/

#include <stdio.h>
#include <string.h>

#include "../lib/tables.h"
#include "../lib/translate_utils.h"
#include "assembler_utils.h"

const char *ASSEMBLER_IGNORE_CHARS = " \f\n\r\t\v,()";
const int MAX_ARGS = 3;
const int ASSEMBLER_BUF_SIZE = 1024;

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
 * whether it is a valid label, and then tries to add it to the symbol table.
 *
 * INPUT_LINE is which line of the input file we are currently processing. Note
 * that the first line is line 1 and that empty lines are included in this count.
 *
 * BYTE_OFFSET is the offset of the NEXT instruction (should it exist).
 *
 * Four scenarios can happen:
 *  1. STR is not a label (does not end in ':'). Returns 0.
 *  2. STR ends in ':', but is not a valid label. Returns -1.
 *  3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
 *      Returns -1.
 *  3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
 *      Returns 1.
 */
int add_if_label(uint32_t input_line, char *str, uint32_t byte_offset,
                 SymbolTable *symtbl) {

    size_t len = strlen(str);
    if (str[len - 1] == ':') {
        str[len - 1] = '\0';
        if (is_valid_label(str)) {
            if (add_to_table(symtbl, str, byte_offset) == 0) {
                return 1;
            } else {
                return -1;
            }
        } else {
            raise_label_error(input_line, str);
            return -1;
        }
    } else {
        return 0;
    }
}


 void raise_label_error(uint32_t input_line, const char *label) {
    printf("Error - invalid label at line %d: %s\n", input_line, label);
}

/* Call this function if more than MAX_ARGS arguments are found while parsing
 * arguments.
 *
 * INPUT_LINE is which line of the input file that the error occurred in. Note
 * that the first line is line 1 and that empty lines are included in the count.
 *
 * EXTRA_ARG should contain the first extra argument encountered.
 */
 void raise_extra_arg_error(uint32_t input_line, const char *extra_arg) {
    printf("Error - extra argument at line %d: %s\n", input_line, extra_arg);
}

/* You should call this function if write_pass_one() or translate_inst()
 * returns -1.
 *
 * INPUT_LINE is which line of the input file that the error occurred in. Note
 * that the first line is line 1 and that empty lines are included in the count.
 */
void raise_inst_error(uint32_t input_line, const char *name, char **args, int num_args) {

    printf("Error - invalid instruction at line %d: ", input_line);
}

/*  A helpful helper function that parses instruction arguments. It raises an error
 *  if too many arguments have been passed into the instruction.
 */
int parse_args(uint32_t input_line, char **args, int *num_args) {
    char *token;
    while ((token = strtok(NULL, ASSEMBLER_IGNORE_CHARS))) {
        if (*num_args < MAX_ARGS) {
            args[*num_args] = token;
            (*num_args)++;
        } else {
            raise_extra_arg_error(input_line, token);
            return -1;
        }
    }
    return 0;
}

/* Truncates the string at the first occurrence of the '#' character. */
 void skip_comment(char *str) {
    char *comment_start = strchr(str, '#');
    if (comment_start) {
        *comment_start = '\0';
    }
}

