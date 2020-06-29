#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include "comand.h"
#include "coder.h"
int encode(uint32_t code_point, CodeUnit* code_units)
{
	int count = 0;
	uint32_t c = code_point;
	while (c) //count bites in digit
	{
		c >>= 1;
		count++;
	}
	if (count < 8)//if bit<8 --> usually digit
	{
		code_units->length = 1;
		code_units->code[0] = 0x7f & code_point;
	}
	else
	{
		code_units->length = (count + 3) % 5; // count byte in encode digit
		if (code_units->length > 4)
		{
			return -1;
		}
		int mask = 8 - code_units->length; //count bite, must move for first byte

			code_units->code[0] = pow(2, code_units->length) - 1; //add work elements for first byte
			code_units->code[0] <<= mask; //back bytes to place
		for (size_t i = code_units->length - 1; i > 0; i--)
		{
			code_units->code[i] = 0x80 | (code_point &
				0x3f);// --> 10ХХХХХХ
			code_point >>= 6; // fill other element
		}
		code_units->code[0] |= code_point; //other bites add with 1 byte
	}
	return 0;
}
uint32_t decode(const CodeUnit* code_unit)
{
	uint32_t dec = 0;
	if (code_unit->length == 1) // if lens == 1 => digit original
	{
	dec = code_unit->code[0];
	}
	else
	{
		if (code_unit->length == 2)//if lens == 2
		{
			dec = code_unit->code[0] & 0x1f; //clean work element
			dec <<= 6; //move 6 bytes left
			dec |= (0x3f & code_unit->code[1]); //decode next byte
		}
		else
		{
			if (code_unit->length == 3)
			{
				dec = code_unit->code[0] & 0xf; //clean work element
				for (size_t i = 1; i < code_unit->length; i++)
				{
					dec <<= 6; //move 6 bytes left
					dec |= (0x3f & code_unit->code[i]); //decode other byte
				}
			}
			else // if lens = 4
			{
				dec = code_unit->code[0] & 0x7; //убираем служебные единицы
				for (size_t i = 1; i < code_unit->length; i++)
				{
					dec <<= 6; //move 6 bytes left
					dec |= (0x3f & code_unit->code[i]); //decode other byte
				}
			}
		}
	}
	return dec;
}
int write_code_unit(FILE* out, const CodeUnit* code_unit)
{
	for (size_t i = code_unit->length; i > 0; i--) //write every byte
	{
	size_t count = fwrite(&code_unit->code[code_unit->length - i], 1,
   sizeof(uint8_t), out);

	if (count != sizeof(uint8_t))
	{
	return -1;
	}
	}
	return 0;
}
int read_next_code_unit(FILE * in, CodeUnit * code_units)
{
	uint8_t n[4];
	code_units->length = 0;
	size_t t = fread(&n[0], sizeof(uint8_t), 1, in); // check first byte
	{
		return -1;
	}
	if (n[0] >> 7 == 0) //if == 0
	{
		code_units->length = 1;
		code_units->code[0] = n[0];
	}
	else {
		for (int i = 5; i >= 3; i--)
		{
			if (n[0] >> (8 - i) == (pow(2, i) - 2)) // if mask == pow
			int count = 0;
			uint8_t y = n[0] >> (8 - i + 1);
			while (y) //count encode bytes
			{
			y >>= 1;
			count++;
			}
		   for (int j = 1; j < count; j++)
			{
			size_t t = fread(&n[j], sizeof(uint8_t), 1,
		   in);// check other bytes

		   if (!t) // check fread
			{
			return -1;
			}
		   if (n[j] >> 6 != 2) 
			{
			return read_next_code_unit(in, code_units);
			}
			}
		   for (int j = 0; j < count; j++)
			{
			code_units->code[j] = n[j]; // write struct
			}
		   code_units->length = count;
			break;
			}
		}
	}
	if (code_units->length == 0) //check if
		return read_next_code_unit(in, code_units);
	}
	return 0;
}