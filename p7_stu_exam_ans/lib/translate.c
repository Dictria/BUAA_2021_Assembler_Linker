/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * translate.c
 * Extend pseudoinstructions as well as translate instructions
 *****************************************************************/

#include <stdio.h>
#include <string.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"
#include "../my_assembler_utils.h"

const int TWO_POW_SEVENTEEN = 131072;    // 2^17

/*  Writes instructions during the assembler's first pass to OUTPUT. The case
 *  for general instructions has already been completed, but you need to write
 *  code to translate the li and other pseudoinstructions. Your pseudoinstruction
 *  expansions should not have any side effects.
 *
 *  NAME is the name of the instruction, ARGS is an array of the arguments, and
 *  NUM_ARGS specifies the number of items in ARGS.
 *
 *  Error checking for regular instructions are done in pass two. However, for
 *  pseudoinstructions, you must make sure that ARGS contains the correct number
 *  of arguments. You do NOT need to check whether the registers / label are
 *  valid, since that will be checked in part two.
 *
 *  Also for li:
 *   - make sure that the number is representable by 32 bits. (Hint: the number
 *       can be both signed or unsigned).
 *   - if the immediate can fit in the imm field of an addiu instruction, then
 *       expand li into a single addiu instruction. Otherwise, expand it into
 *       a lui-ori pair.
 *
 *  For mul, quo, and rem, the expansions should be pretty straight forward if
 *  you paid attention to $hi and $lo registers.
 *
 *  For la:
 *   - The la instruction will only use the symbols defined in the .data segment.
 *     Also, the symbols defined in the .data segment will only be used in the la instruction.
 *   - The la instruction is converted into a lui-ori pair to load
 *     the absolute address of the symbol into the corresponding register
 *   - Similar to read_data_segment(), we append a @Hi/Lo flag to the symbols that appear in la
 *     to indicate whether the load address is 16 bits higher or 16 bits lower at the time of
 *     conversion. In our implementation, all "la $reg, label" instructions are converted to
 *     "lui $at, label@Hi" followed by an "ori $reg, $at, label@Lo"
 *
 *  IMPORTANT
 *  MARS has slightly different translation rules for li, and it allows numbers
 *  larger than the largest 32 bit number to be loaded with li. You should follow
 *  the above rules if MARS behaves differently.
 *
 *  Use fprintf() to write. If writing multiple instructions, make sure that
 *  each instruction is on a different line.
 *
 *  Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE *output, const char *name, char **args, int num_args) {
    if (strcmp(name, "li") == 0) {
        long int imm;
        if (translate_num(&imm, args[1], INT32_MIN, INT32_MAX) == -1) {
            printf("%s Number is more than 32 bits. \n", args[1]);
            return 0;
        }
        int err = translate_num(&imm, args[1], INT16_MIN, INT16_MAX);
        if (err == 0) {
            fprintf(output, "addiu %s $0 %s\n", args[0], args[1]);
            return 1;
        } else {
            int upper = imm >> 16;
            fprintf(output, "lui $at %d\n", upper);

            int lower;
            lower = imm & 0xffff;
            fprintf(output, "ori %s $at %d\n", args[0], lower);
            return 2;
        }
    } else if (strcmp(name, "mul") == 0) {
        fprintf(output, "mult %s %s\n", args[1], args[2]);
        fprintf(output, "mflo %s\n", args[0]);
        return 2;
    } else if (strcmp(name, "quo") == 0) {
        fprintf(output, "div %s %s\n", args[1], args[2]);
        fprintf(output, "mflo %s\n", args[0]);
        return 2;
    } else if (strcmp(name, "rem") == 0) {
        fprintf(output, "rem %s %s\n", args[1], args[2]);
        fprintf(output, "mfhi %s\n", args[0]);
        return 2;
    } else if (strcmp(name, "la") == 0) {
        fprintf(output, "lui $at %s@Hi\n", args[1]);
        fprintf(output, "ori %s $at %s@Lo\n", args[0], args[1]);
        return 2;
    } else if (strcmp(name, "move") == 0) {
        fprintf(output, "addu %s $0 %s\n", args[0], args[1]);
        return 1;
    }
    write_inst_string(output, name, args, num_args);
    return 1;
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.
 *
 * NAME is the name of the instruction, ARGS is an array of the arguments, and
 * NUM_ARGS specifies the number of items in ARGS.
 *
 * The symbol table (SYMTBL) is given for any symbols that need to be resolved
 * at this step. If a symbol should be relocated, it should be added to the
 * relocation table (RELTBL), and the fields for that symbol should be set to
 * all zeros.
 *
 * You must perform error checking on all instructions and make sure that their
 * arguments are valid. If an instruction is invalid, you should not write
 * anything to OUTPUT but simply return -1.
 *
 * Returns 0 on success and -1 on error.
 */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args, uint32_t addr,
                   SymbolTable *symtbl, SymbolTable *reltbl) {
    if (strcmp(name, "addu") == 0)
        return write_rtype(0x21, output, args, num_args);
    else if (strcmp(name, "or") == 0) return write_rtype(0x25, output, args, num_args);
    else if (strcmp(name, "slt") == 0) return write_rtype(0x2a, output, args, num_args);
    else if (strcmp(name, "sltu") == 0) return write_rtype(0x2b, output, args, num_args);
    else if (strcmp(name, "sll") == 0) return write_shift(0x00, output, args, num_args);
    else if (strcmp(name, "jr") == 0) return write_jr(0x08, output, args, num_args);
    else if (strcmp(name, "addiu") == 0) return write_addiu(0x09, output, args, num_args);
    else if (strcmp(name, "ori") == 0) return write_ori(0x0d, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "lui") == 0) return write_lui(0x0f, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "lb") == 0) return write_mem(0x20, output, args, num_args);
    else if (strcmp(name, "lbu") == 0) return write_mem(0x24, output, args, num_args);
    else if (strcmp(name, "lw") == 0) return write_mem(0x23, output, args, num_args);
    else if (strcmp(name, "sb") == 0) return write_mem(0x28, output, args, num_args);
    else if (strcmp(name, "sw") == 0) return write_mem(0x2b, output, args, num_args);
    else if (strcmp(name, "beq") == 0) return write_branch(0x04, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bne") == 0) return write_branch(0x05, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "j") == 0) return write_jump(0x02, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "jal") == 0) return write_jump(0x03, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "mult") == 0) return write_rtype(0x18, output, args, num_args);
    else if (strcmp(name, "div") == 0) return write_rtype(0x1a, output, args, num_args);
    else if (strcmp(name, "mfhi") == 0) return write_rtype(0x10, output, args, num_args);
    else if (strcmp(name, "mflo") == 0) return write_rtype(0x12, output, args, num_args);

    else return -1;
}

/* A helper function for writing most R-type instructions. You should use
 * translate_reg() to parse registers and write_inst_hex() to write to
 * OUTPUT. Both are defined in translate_utils.h.
 */
int write_rtype(uint8_t funct, FILE *output, char **args, size_t num_args) {
    int op = 0;
    int rs = 0;
    int rt = 0;
    int rd = 0;
    int shamt = 0;

    if (num_args > 3) {
        printf("invalid number of arguments. \n");
        return -1;
    }
    if (num_args == 1) {
        rd = translate_reg(args[0]);
    } else if (num_args == 2) { //mult, div
        rs = translate_reg(args[0]);
        rt = translate_reg(args[1]);
    } else {
        rd = translate_reg(args[0]);
        rs = translate_reg(args[1]);
        rt = translate_reg(args[2]);
    }

    if ((rd == -1) | (rs == -1) | (rt == -1)) {
        printf("Invalid register. \n");
        return -1;
    }

    uint32_t instruction = 0;

    instruction = instruction | funct;
    instruction = instruction | (shamt << 6);
    instruction = instruction | (rd << 11);
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (op << 26);

    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing shift instructions. You should use
 * translate_num() to parse numerical arguments. translate_num() is defined
 * in translate_utils.h.
 */
int write_shift(uint8_t funct, FILE *output, char **args, size_t num_args) {
    if (num_args != 3) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    long int shamt;
    int rs = 0;
    int op = 0;
    int rd = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int err = translate_num(&shamt, args[2], 0, 31);

    if ((rd == -1) | (rt == -1)) {
        printf("Invalid register. \n");
        return -1;
    }
    if (err == -1) {
        printf("%li is not a a valid shift amount. \n", shamt);
        return -1;
    }

    uint32_t instruction = 0;
    instruction = instruction | funct;
    instruction = instruction | (shamt << 6);
    instruction = instruction | (rd << 11);
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (op << 26);

    write_inst_hex(output, instruction);
    return 0;
}

int write_addiu(uint8_t opcode, FILE *output, char **args, size_t num_args) {

    if (num_args != 3) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    long int imm;
    uint32_t op = opcode;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int err = translate_num(&imm, args[2], INT16_MIN, INT16_MAX);

    if ((rs == -1) | (rt == -1)) {
        printf("Invalid register. \n");
        return -1;
    }
    if (err == -1) {
        printf("%li is not a valid amount to add. \n", imm);
        return -1;
    }

    uint32_t instruction = 0;
    imm = imm & 0x0000ffff;
    instruction = instruction | imm;
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (op << 26);

    write_inst_hex(output, instruction);
    return 0;
}

int write_ori(uint8_t opcode, FILE *output, char **args, size_t num_args, uint32_t addr, SymbolTable *reltbl) {
    if (num_args != 3) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    long int imm;
    uint32_t op = opcode;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int err = translate_num(&imm, args[2], 0, UINT16_MAX);

    if ((rs == -1) | (rt == -1)) {
        printf("Invalid register. \n");
        return -1;
    }
    if (err == -1) {
        add_to_table(reltbl, args[2], addr);
        imm = 0;
    }

    uint32_t instruction = 0;
    instruction = instruction | imm;
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (op << 26);

    write_inst_hex(output, instruction);
    return 0;
}

int write_mem(uint8_t opcode, FILE *output, char **args, size_t num_args) {
    if (num_args != 3) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[2]);
    int err = translate_num(&imm, args[1], INT16_MIN, INT16_MAX);

    if ((rs == -1) | (rt == -1)) {
        printf("Invalid register. \n");
        return -1;
    }
    if (err == -1) {
        printf("%li is not a valid number. \n", imm);
        return -1;
    }

    uint32_t instruction = 0;
    instruction = instruction | (imm & 0x0000ffff);
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (opcode << 26);
    write_inst_hex(output, instruction);
    return 0;
}

/*  A helper function to determine if a destination address
 *  can be branched to
 */
static int can_branch_to(uint32_t src_addr, uint32_t dest_addr) {
    int32_t diff = dest_addr - src_addr;
    return (diff >= 0 && diff <= TWO_POW_SEVENTEEN) || (diff < 0 && diff >= -(TWO_POW_SEVENTEEN - 4));
}


int write_branch(uint8_t opcode, FILE *output, char **args, size_t num_args, uint32_t addr, SymbolTable *symtbl) {
    if (num_args != 3) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    int rs = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int label_addr = get_addr_for_symbol(symtbl, args[2]);

    if ((rs == -1) | (rt == -1)) {
        printf("Invalid register. \n");
        return -1;
    }

    if (label_addr == -1) {
        printf("This label does not exist. \n");
        return -1;
    }

    int32_t offset = (label_addr - (addr + 4)) / 4;
    if (!can_branch_to(addr, label_addr)) {
        printf("cannot branch to given destination address.");
        return -1;
    }

    uint32_t instruction = 0;
    instruction = instruction | (offset & 0x0000ffff);
    instruction = instruction | (rt << 16);
    instruction = instruction | (rs << 21);
    instruction = instruction | (opcode << 26);
    write_inst_hex(output, instruction);
    return 0;
}

int write_jump(uint8_t opcode, FILE *output, char **args, size_t num_args, uint32_t addr, SymbolTable *reltbl) {
    if (num_args != 1) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    char *imm = args[0];

    if (!is_valid_label(imm)) {
        printf("The label name is invalid.");
        return -1;
    }

    add_to_table(reltbl, imm, addr);

    uint32_t instruction = 0;
    instruction = instruction | (opcode << 26);
    write_inst_hex(output, instruction);
    return 0;
}

int write_jr(uint8_t funct, FILE *output, char **args, size_t num_args) {
    if (num_args != 1) {
        printf("invalid number of arguments. \n");
        return -1;
    }

    int rs = translate_reg(args[0]);
    if (rs == -1) {
        printf("Invalid register. \n");
        return -1;
    }

    uint32_t instruction = 0;
    instruction = instruction | funct;
    instruction = instruction | (rs << 21);
    write_inst_hex(output, instruction);
    return 0;
}

