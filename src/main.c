#include <stdio.h>

#include "core/elf_parser.h"
#include "protections/protections.h"

#define VERMELHO "\x1b[31m"
#define VERDE    "\x1b[32m"
#define AMARELO  "\x1b[33m"
#define RESET    "\x1b[0m"

static void print_prot(const char *name, int v) {
    const char *status = "UNKNOWN";
    const char *color  = AMARELO;

    switch (v) {
        case PROT_ENABLED:
            status = "ENABLED";
            color  = VERDE;
            break;

        case PROT_FULL:
            status = "FULL";
            color  = VERDE;
            break;

        case PROT_DISABLED:
            status = "DISABLED";
            color  = VERMELHO;
            break;

        case PROT_PARTIAL:
            status = "PARTIAL";
            color  = AMARELO;
            break;

        default:
            status = "UNKNOWN";
            color  = AMARELO;
            break;
    }

    printf("  %-10s : %s%s%s\n", name, color, status, RESET);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: checkbinary <elf-file> [elf-file...]\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {

        elf_binary_t bin;

        printf("\n[*] Analyzing: %s\n", argv[i]);

        if (parse_elf(argv[i], &bin) < 0) {
            printf("[-] Not a valid ELF file\n");
            continue;
        }

        printf("  %-10s : %s\n", "Arch", detect_arch(&bin));
        print_prot("NX",      check_nx(&bin));
        print_prot("PIE",     check_pie(&bin));
        print_prot("RELRO",   check_relro(&bin));
        print_prot("CANARY",  check_canary(&bin));
        print_prot("FORTIFY", check_fortify(&bin));

        free_elf(&bin);
    }

    return 0;
}
