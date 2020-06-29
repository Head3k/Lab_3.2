#include "comand.h"
#include "coder.h"
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
int encode_file(const char* in_file_name, const char* out_file_name)
{
	FILE* in = fopen(in_file_name, "r"); //open file for read
	FILE* out = fopen(out_file_name, "wb"); //open dat or create for write
	if (in == NULL || out == NULL) // check open files
	{
		fclose(in);
		fclose(out);
		return -1;
	}
	CodeUnit* c = malloc(sizeof(CodeUnit));
	uint32_t n;
	while (!feof(in)) //while !E0F
	{
		size_t k = fscanf(in, "%" SCNx32, &n); //scan digit
		int z = encode(n, c); //encode digit
		int y = write_code_unit(out, c); //write digit
		if (z || y || k != 1) //if error
		{
			free(c);
			fclose(in);
			fclose(out);
			return -1;
		}
	}
	fclose(in);
	fclose(out);
	free(c);
	return 0;
}
int decode_file(const char* in_file_name, const char* out_file_name)
{
	FILE* in = fopen(in_file_name, "rb"); //open dat for read
	FILE* out = fopen(out_file_name, "w"); //open or create file for write
	if (in == NULL || out == NULL) //check open files
	{
		fclose(in);
		fclose(out);
		return -1;
	}
	CodeUnit* c = malloc(sizeof(CodeUnit));
	while (read_next_code_unit(in, c) != -1) //while non-error
	{
		uint32_t dec = decode(c); //decode
		size_t t = fprintf(out, "\n%" PRIx32, dec); //write in file

		if (!t) //if error
		{
			free(c);
			return -1;
		}
	}
	free(c);
	return 0;
}