#include "protections.h"

int check_nx(elf_binary_t *bin) {
	for (int i = 0; i < bin->phnum; i++) {
		if (bin->phdrs[i].p_type == PT_GNU_STACK) {
			if (bin->phdrs[i].p_flags & PF_X)
				return PROT_DISABLED;
			else
				return PROT_ENABLED;
		}
	}
	return PROT_UNKNOWN;
}
