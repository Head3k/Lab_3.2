#include "coder.c"
#include <inttypes.h>
#include <stdlib.h>

int encode_file(const char *in_file_name, const char *out_file_name)
{
FILE *in = fopen(in_file_name, "r"); //открываем файл на чтение
FILE *out = fopen(out_file_name, "wb"); //и бинарник на запись
if (!in)
{
fclose(out);
return -1;
}
if (!out)
{
fclose(in);
return -1;
}
CodeUnit *c = malloc(sizeof(CodeUnit));
uint32_t n;
while (!feof(in))
{
size_t k = fscanf(in, "%" SCNx32, &n); //считываем из файла число
int t = encode(n, c); //кодируем его
int y = write_code_unit(out, c); //и записываем в бинарник
if (t || y || k != 1) //если где-то ошибка
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
if (!in)
{
fclose(out);
return -1;
}
if (!out)
{
fclose(in);
return -1;
}
CodeUnit *c = malloc(sizeof(CodeUnit)); //выделяем память под структуру
while (read_next_code_unit(in, c) != -1) //пока нет ошибки с функции
{
uint32_t dec = decode(c); //декодируем
size_t t = fprintf(out, "\n%" SCNx32, dec); //записываем в файл
if (!t) //если где-то ошибка
{
free(c);
return -1;
}
}
free(c);
return 0;
}
