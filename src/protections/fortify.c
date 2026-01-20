#include <string.h>
#include "protections.h"

int check_fortify(elf_binary_t *bin) {

    for (int i = 0; i < bin->sym_count; i++) {
        if (strstr(bin->dyn_symbols[i], "_chk"))
            return PROT_ENABLED;
    }

    return PROT_DISABLED;
}
