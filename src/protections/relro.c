#include <stdio.h>
#include <elf.h>
#include "protections.h"

int check_relro(elf_binary_t *bin) {
    int has_relro = 0;
    int bind_now = 0;

    if (!bin)
        return PROT_UNKNOWN;

    for (int i = 0; i < bin->phnum; i++) {
        if (bin->phdrs[i].p_type == PT_GNU_RELRO) {
            has_relro = 1;
            break;
        }
    }

    if (!has_relro)
        return PROT_DISABLED;

    if (!bin->dynamic || bin->dyn_count == 0)
        return PROT_PARTIAL;

    for (int i = 0; i < bin->dyn_count; i++) {
        Elf64_Dyn d = bin->dynamic[i];

        if (d.d_tag == DT_NULL)
            break;

        if (d.d_tag == DT_BIND_NOW) {
            bind_now = 1;
            break;
        }

        if (d.d_tag == DT_FLAGS_1) {
            if (d.d_un.d_val & DF_1_NOW) {
                bind_now = 1;
                break;
            }
        }

        if (d.d_tag == DT_FLAGS) {
            if (d.d_un.d_val & DF_BIND_NOW) {
                bind_now = 1;
                break;
            }
        }
    }

    return bind_now ? PROT_FULL : PROT_PARTIAL;
}
