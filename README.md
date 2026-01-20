# elfsec

A simple tool responsible for performing binary checks. For now, he is only checking the binary architecture and the protections present in the binary!

Version: 1.0

# Installation Guide

## Step 1

```bash
~/pa1va
❯ sudo apt update
❯ sudo apt upgrade
❯ sudo apt install git
❯ git clone https://github.com/pa1va/elfsec.git
```

## Step 2

```bash
~/pa1va/elfsec
❯ make
gcc -Wall -Wextra -g -c src/main.c -o src/main.o
gcc -Wall -Wextra -g -c src/core/elf_parser.c -o src/core/elf_parser.o
gcc -Wall -Wextra -g -c src/protections/arch.c -o src/protections/arch.o
gcc -Wall -Wextra -g -c src/protections/canary.c -o src/protections/canary.o
gcc -Wall -Wextra -g -c src/protections/fortify.c -o src/protections/fortify.o
gcc -Wall -Wextra -g -c src/protections/nx.c -o src/protections/nx.o
gcc -Wall -Wextra -g -c src/protections/pie.c -o src/protections/pie.o
gcc -Wall -Wextra -g -c src/protections/relro.c -o src/protections/relro.o
gcc -Wall -Wextra -g src/main.o src/core/elf_parser.o src/protections/arch.o src/protections/canary.o src/protections/fortify.o src/protections/nx.o src/protections/pie.o src/protections/relro.o -o elfsec

~/pa1va/elfsec
❯ ll
total 52K
-rwxrwxr-x 1 pa1va pa1va elfsec*
-rwxrwxr-x 1 pa1va pa1va hello*
-rwxrwxr-x 1 pa1va pa1va buffer*
-rw-rw-r-- 1 pa1va pa1va Makefile
drwxrwxr-x 4 pa1va pa1va src/
```

## Step 3

```bash
~/pa1va/elfsec
❯ ./elfsec hello

[*] Analyzing: hello
  Arch       : x86-64
  NX         : ENABLED
  PIE        : ENABLED
  RELRO      : FULL
  CANARY     : DISABLED
  FORTIFY    : DISABLED

~/pa1va/elfsec
❯ ./elfsec buffer 

[*] Analyzing: buffer
  Arch       : x86-64
  NX         : ENABLED
  PIE        : ENABLED
  RELRO      : FULL
  CANARY     : ENABLED
  FORTIFY    : ENABLED
```
