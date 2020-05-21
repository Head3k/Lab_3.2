#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include "comand.h"
#include "coder.h"

int encode(uint32_t code_point, CodeUnit *code_units)
{
int count = 0;
uint32_t c = code_point;
    while (c) //считаем кол-во битов в числе
    {
        c >>= 1; 
        count++;
    }
    if (count < 8)//если бит меньше 8, то кодируем как обычное число 
    {
        code_units->length = 1;
        code_units->code[0] = 0x7f & code_point;
    }
    else
    {
        code_units->length = (count + 3) % 5; //количество байт в закодированном числе
        if (code_units->length > 4)
        {
            return -1;
        }
        int mask = 8 - code_units->length; //кол-во бит, на которое нужно сдвинуть для первого байта
         
        code_units->code[0] = pow(2, code_units->length) - 1; //выделяем служебные единицы для первого байта
        code_units->code[0] <<= mask; //сдвигаем байты на место

        for (size_t i = code_units->length - 1; i > 0; i--)
        {
            code_units->code[i] = 0x80 | (code_point & 0x3f);//преобразовывается к виду(10ХХХХХХ)
            code_point >>= 6; //заполняем остальные байты
        }

        code_units->code[0] |= code_point; //оставшиеся биты соединяем с первым байтом
    }
return 0;
}

uint32_t decode(const CodeUnit *code_unit)
{
    uint32_t dec = 0;
    if (code_unit->length == 1) //если длина 1, то закодированное число = оригиналу
    {
        dec = code_unit->code[0];
    }
    else
    {
        if (code_unit->length == 2)//если длина 2 
        {
            dec = code_unit->code[0] & 0x1f; //убираем служебные единицы 
            dec <<= 6;                       //сдвигаем на 6 байтов влево 
            dec |= (0x3f & code_unit->code[1]); //декодируем следующий байт
        } 
        else
        {
            if (code_unit->length == 3)
            {
                dec = code_unit->code[0] & 0xf; //убираем служебные единицы
                for (size_t i = 1; i < code_unit->length; i++)
                {
                    dec <<= 6;                  //сдвигаем на 6 байтов влево 
                    dec |= (0x3f & code_unit->code[i]); //декодируем следующие байты
                }
            }
            else // если длина равна 4
            {
                dec = code_unit->code[0] & 0x7; //убираем служебные единицы
                for (size_t i = 1; i < code_unit->length; i++)
                {
                    dec <<= 6;                  //сдвигаем на 6 байтов влево 
                    dec |= (0x3f & code_unit->code[i]); //декодируем следующие байты 
                }
            }
        }
    }
return dec;
}


int write_code_unit(FILE *out, const CodeUnit *code_unit)
{
    for (size_t i = code_unit->length; i > 0; i--) //побайтово записываем в файл
    {
        size_t count = fwrite(&code_unit->code[code_unit->length - i], 1, sizeof(uint8_t), out);
        
        if (count != sizeof(uint8_t))
        {
            return -1;
        }
    }
return 0;
}

int read_next_code_unit(FILE* in, CodeUnit* code_units)
{
    uint8_t n[4];
    code_units->length = 0;
    size_t t = fread(&n[0], sizeof(uint8_t), 1, in); //считываем первый байт
    if (t != 1) 
    {
        return -1;
    }
    if (n[0] >> 7 == 0) //если служебное число - 0 
    { 
        code_units->length = 1;
        code_units->code[0] = n[0];
    }
    else 
        {
            for (int i = 5; i >= 3; i--) 
            {
                if (n[0] >> (8 - i) == (pow(2, i) - 2)) // если маска равна степени
                {                                       // 110=(2^3)-2 (6=6)
                    int count = 0;
                    uint8_t y = n[0] >> (8 - i + 1);
                    while (y) //считаем кол-во закодированных байт
                    { 
                        y >>= 1;
                        count++;
                    }
                    for (int j = 1; j < count; j++) 
                    {
                        size_t t = fread(&n[j], sizeof(uint8_t), 1, in);//считываем остальные байты
                        
                        if (!t) //проверка fread
                        {
                            return -1;
                        }
                        
                        if (n[j] >> 6 != 2) //если не 10......
                        {
                            return read_next_code_unit(in, code_units);
                        }
                    }
                    for (int j = 0; j < count; j++) 
                    {
                        code_units->code[j] = n[j]; //записываем в структуру
                    }
                    code_units->length = count;
                    break;
                }
            }
        }
        if (code_units->length == 0) //проверяем, зашли ли мы в if
        { 
            return read_next_code_unit(in, code_units);
        }
return 0;
}