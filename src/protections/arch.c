#include <elf.h>

#include "protections.h"

const char *detect_arch(elf_binary_t *bin) {

    int elf_class = bin->ehdr->e_ident[EI_CLASS];
    int machine   = bin->ehdr->e_machine;

    if (machine == EM_386 && elf_class == ELFCLASS32)
        return "x86 (32-bit)";

    if (machine == EM_X86_64 && elf_class == ELFCLASS64)
        return "x86-64";

    if (machine == EM_ARM && elf_class == ELFCLASS32)
        return "ARM (32-bit)";

    if (machine == EM_AARCH64 && elf_class == ELFCLASS64)
        return "AArch64";

    if (machine == EM_MIPS && elf_class == ELFCLASS32)
        return "MIPS (32-bit)";

    if (machine == EM_MIPS && elf_class == ELFCLASS64)
        return "MIPS64";

    if (machine == EM_RISCV && elf_class == ELFCLASS32)
        return "RISC-V (32-bit)";

    if (machine == EM_RISCV && elf_class == ELFCLASS64)
        return "RISC-V (64-bit)";

    return "UNKNOWN";
}
