#include "comand.h"
#include "coder.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
     if (argc != 4) //если некорректное кол-во аргументов командной строки
    {
        printf("Неверные аргументы командной строки. Программа завершается...\n");
        return 0;
    }
    else
    {
        if (!strcmp(argv[1], "decode")) //если decode
        {
            decode_file(argv[2], argv[3]); //декодируем
        }
        else
        {
            if (!strcmp(argv[1], "encode")) //если encode
            {
                encode_file(argv[2], argv[3]); //кодируем
            }
            else //если неправильная команда
            {
            printf("Неверные аргументы командной строки. Программа завершается...\n");
            return 0;
            }
        }
    }
    
    return 0;
}