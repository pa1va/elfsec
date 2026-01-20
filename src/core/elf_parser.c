#include "elf_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

static void  *g_base = NULL;
static size_t g_size = 0;

static int map_file(const char *path) {
    int fd;
    struct stat st;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return -1;

    if (fstat(fd, &st) < 0) {
        close(fd);
        return -1;
    }

    g_size = st.st_size;

    g_base = mmap(NULL, g_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (g_base == MAP_FAILED) {
        g_base = NULL;
        return -1;
    }

    return 0;
}

static int is_valid_elf(Elf64_Ehdr *eh) {
    return memcmp(eh->e_ident, ELFMAG, SELFMAG) == 0;
}

static void parse_dynamic(elf_binary_t *bin) {
    for (int i = 0; i < bin->phnum; i++) {
        if (bin->phdrs[i].p_type == PT_DYNAMIC) {
            bin->dynamic =
                (Elf64_Dyn *)((char *)g_base + bin->phdrs[i].p_offset);
            bin->dyn_count =
                bin->phdrs[i].p_filesz / sizeof(Elf64_Dyn);
            return;
        }
    }
}

static void parse_dynsym(elf_binary_t *bin) {

    if (!bin->ehdr->e_shoff || !bin->ehdr->e_shnum)
        return;

    Elf64_Shdr *shdr =
        (Elf64_Shdr *)((char *)g_base + bin->ehdr->e_shoff);

    const char *shstr =
        (char *)g_base + shdr[bin->ehdr->e_shstrndx].sh_offset;

    for (int i = 0; i < bin->ehdr->e_shnum; i++) {

        if (strcmp(&shstr[shdr[i].sh_name], ".dynsym") == 0) {

            Elf64_Sym *syms =
                (Elf64_Sym *)((char *)g_base + shdr[i].sh_offset);

            int count =
                shdr[i].sh_size / sizeof(Elf64_Sym);

            Elf64_Shdr *strtab = &shdr[shdr[i].sh_link];
            const char *strings =
                (char *)g_base + strtab->sh_offset;

            bin->dyn_symbols = calloc(count, sizeof(char *));
            bin->sym_count = 0;

            for (int j = 0; j < count; j++) {
                if (syms[j].st_name) {
                    bin->dyn_symbols[bin->sym_count++] =
                        strdup(&strings[syms[j].st_name]);
                }
            }
            return;
        }
    }
}

int parse_elf(const char *path, elf_binary_t *bin) {

    memset(bin, 0, sizeof(*bin));

    if (map_file(path) < 0)
        return -1;

    bin->ehdr = (Elf64_Ehdr *)g_base;

    if (!is_valid_elf(bin->ehdr))
        return -1;

    if (bin->ehdr->e_ident[EI_CLASS] != ELFCLASS64)
        return -1;

    bin->is_elf = 1;
    bin->is_64  = 1;
    bin->is_pie = (bin->ehdr->e_type == ET_DYN);

    bin->phdrs =
        (Elf64_Phdr *)((char *)g_base + bin->ehdr->e_phoff);
    bin->phnum = bin->ehdr->e_phnum;

    parse_dynamic(bin);
    parse_dynsym(bin);

    return 0;
}

void free_elf(elf_binary_t *bin) {

    for (int i = 0; i < bin->sym_count; i++)
        free(bin->dyn_symbols[i]);

    free(bin->dyn_symbols);

    if (g_base) {
        munmap(g_base, g_size);
        g_base = NULL;
        g_size = 0;
    }
}

