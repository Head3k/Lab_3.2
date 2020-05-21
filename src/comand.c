#include "comand.h"
#include "coder.h"
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

int encode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in = fopen(in_file_name, "r"); //открываем файл на чтение
    FILE *out = fopen(out_file_name, "wb"); //и бинарник на запись
    if (in==NULL || out==NULL) //проверка, что файлы были открыты
    {
        fclose(in);
        fclose(out);
        return -1;
    }

    CodeUnit *c = malloc(sizeof(CodeUnit));
    uint32_t n;
    while (!feof(in))    //пока не достигнут конец строки 
    {
        size_t k = fscanf(in, "%" SCNx32, &n); //считываем из файла число
        int z = encode(n, c);                  //кодируем его
        int y = write_code_unit(out, c);       //записываем
        if (z || y || k != 1)                  //если где-то ошибка
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

int decode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in = fopen(in_file_name, "rb"); //открываем бинарник на чтение
    FILE *out = fopen(out_file_name, "w"); //и файл на запись
    if (in==NULL || out==NULL) //проверка, что файлы были открыты
    {
        fclose(in);
        fclose(out);
        return -1;
    }

    CodeUnit *c = malloc(sizeof(CodeUnit));
    while (read_next_code_unit(in, c) != -1) //пока нет ошибки с функции
    {
        uint32_t dec = decode(c); //декодируем
        size_t t = fprintf(out, "\n%" PRIx32, dec); //записываем в файл
    
        if (!t) //если где-то ошибка
        {
            free(c);
            return -1;
        }
    }
    free(c);
return 0;
}