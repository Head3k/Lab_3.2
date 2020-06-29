#include "comand.h"
#include "coder.h"
#include <string.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
	if (argc != 4) //if error cmd enter
	{
	printf("Error arguments cmd\n");
	return 0;
		}
	else
	{
		if (!strcmp(argv[1], "decode")) //if decode
		{
			decode_file(argv[2], argv[3]); //decoding
		}
		else
		{
			if (!strcmp(argv[1], "encode")) //encode
			{
				encode_file(argv[2], argv[3]); //encoding
			}
			else //if wrong command
			{
				printf("Error arguments cmd\n");
					return 0;
			}
		}
	}

	return 0;
}