#include "coder.c"
#include <stdio.h>

int encode(uint32_t code_point, CodeUnit *code_units);
uint32_t decode(const CodeUnit *code_unit);
int read_next_code_unit(FILE *in, CodeUnit *code_units);
int write_code_unit(FILE *out, const CodeUnit *code_unit);t);
