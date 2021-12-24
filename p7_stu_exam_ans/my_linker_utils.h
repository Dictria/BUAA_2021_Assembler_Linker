/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * my_linker_utils.h
 * Linker Functions Description
 *****************************************************************/
#ifndef MY_LINKER_UTILS_H
#define MY_LINKER_UTILS_H
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
fill_data(FILE *input, SymbolTable *symtbl, RelocData *reldt, uint32_t base_text_offset, uint32_t base_data_offset);

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
int inst_needs_relocation(SymbolTable *reltbl, uint32_t offset);

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
int32_t relocate_inst(uint32_t inst, uint32_t offset, SymbolTable *symtbl, SymbolTable *reltbl);

#endif
