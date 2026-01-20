#ifndef PROTECTIONS_H
#define PROTECTIONS_H

#include "../core/elf_parser.h"

#define PROT_DISABLED 0
#define PROT_ENABLED  1
#define PROT_PARTIAL  2
#define PROT_FULL     3
#define PROT_UNKNOWN -1

int check_nx(elf_binary_t *bin);
int check_pie(elf_binary_t *bin);
int check_relro(elf_binary_t *bin);
int check_canary(elf_binary_t *bin);
int check_fortify(elf_binary_t *bin);

const char *detect_arch(elf_binary_t *bin);

#endif
