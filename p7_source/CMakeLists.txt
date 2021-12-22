cmake_minimum_required(VERSION 3.20)
project(p7_global C)

set(CMAKE_C_STANDARD 99)

#add_executable(assembler assembler.c assembler-src/assembler_utils.c assembler-src/assembler_utils.h
#        lib/tables.c lib/tables.h lib/translate.c lib/translate.h lib/translate_utils.c lib/translate_utils.h)

add_executable(linker linker.c linker-src/linker_utils.c linker-src/linker_utils.h
        lib/tables.c lib/tables.h lib/translate.c lib/translate.h lib/translate_utils.c lib/translate_utils.h)
