#include "protections.h"

int check_pie(elf_binary_t *bin){
	
	if (bin->ehdr->e_type == ET_DYN)
		return PROT_ENABLED;
	return PROT_DISABLED;
}
