#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#define MaxCodeLength 4

typedef struct
{
    uint8_t code[MaxCodeLength];
    size_t length;
} CodeUnit;

int encode(uint32_t code_point, CodeUnit *code_units);
uint32_t decode(const CodeUnit *code_unit);
int write_code_unit(FILE *out, const CodeUnit *code_unit);
int read_next_code_unit(FILE *in, CodeUnit *code_units);
int encode_file(const char *in_file_name, const char *out_file_name);
int decode_file(const char *in_file_name, const char *out_file_name);