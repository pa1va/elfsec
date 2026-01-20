#include <string.h>
#include "protections.h"

int check_canary(elf_binary_t *bin) {

    for (int i = 0; i < bin->sym_count; i++) {
        if (strcmp(bin->dyn_symbols[i], "__stack_chk_fail") == 0)
            return PROT_ENABLED;
    }

    return PROT_DISABLED;
}
