#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <elf.h>

typedef struct {

    int is_elf;
    int is_64;
    int is_pie;

    Elf64_Ehdr *ehdr;
    Elf64_Phdr *phdrs;
    int phnum;

    Elf64_Dyn *dynamic;
    int dyn_count;

    char **dyn_symbols;
    int sym_count;

} elf_binary_t;

int  parse_elf(const char *path, elf_binary_t *bin);
void free_elf(elf_binary_t *bin);

#endif