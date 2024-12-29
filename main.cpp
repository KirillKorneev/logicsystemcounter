#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char st[80]; ///Стек для операций
char all[256];///Массив для основной строки
int n = 80; ///Количество макс. операций
int kol = 0; ///Количество переменных
int counter = 0; ///Счетчик в ответ
int all_int=0;///Счетчик для главного массива
int c[256]={0};///Счетчик для подсчета количества переменных
FILE* input;///Файл ввода
FILE* output;///Файл вывода


char digit (char* out);

void f(char* out, char y) ///Функция таблицы истинности(готова)
{
    int i=0, size, j=0, f=0, t=1; ///Счетчики
    int a=0, h=0, g=0, v=0, c=0;
    char out1[200];///Буффер массива
    char symbol;///Символ
    output = fopen("data_out.txt", "w");
    if (output == NULL)
    {
        printf("Eror [cant open output file");
    }
    size = 1 << kol;///Узнаем количество строк в таблице истинности
    v=strlen(out);
    for(a = 0; a < size; a++)///Проходим каждую строку в таблице
    {
        j=0;
        for (f = 0; f < v; f++)///Кидаем основную строку в буффер
            out1[f] = out[f];
        out1[f]='\0';
        for (i = kol - 1; i >= 0; i--)///Проходим каждый разряд, исходя из количества переменных, для перебора возможных комбинаций
        {
            for(c = 0; c < strlen(out); c++)
            {
                if (((out1[j] >= 'a' && out1[j] <= 'z') || (out1[c] >= 'a' && out1[c] <= 'z')) || ((out1[j] >= 'A' && out1[j] <= 'Z') || (out1[c] >= 'A' && out1[c] <= 'Z')))
                {
                    if ((out1[c] >= 'a' && out1[c] <= 'z') || (out1[c] >= 'A' && out1[c] <= 'Z'))
                        j=c;
                    h=j+1;
                    while (out1[h]!='\0')///В этом цикле ищем похожие переменные
                    {
                        if (out1[h] == out1[j])///Если находим, то ставим на их значение константу
                        {
                            g = (a >> i) & 1;
                            symbol = g + '0';
                            out1[h] = symbol;
                            h++;
                        }
                        h++;
                    }
                    g = (a >> i) & 1;///Ставим константу в искомую переменную
                    symbol = g + '0';
                    out1[j] = symbol;
                    j++;
                    break;
                }
            }
        }
        if (digit(out1) == '1')///Если выражение истинно
        {
            counter++;///То повышаем значение счетчика
            fprintf(output, "%d", t);
            fprintf(output, ". ");
            for (i = kol - 1; i >=0; i--)
            {
                fprintf(output, "%d ", ((a>>i)&1) );///И печатаем этот ответ в файл
            }
            fprintf(output, "\n");
            t++;
        }
    }
}

int priority (char a) ///Функция выявления приоритета (готова)
{
    int n=0;
    switch (a)
    {
    case '!': ///Если это ^ или / , то наивысшее значение приоритета (5) (конъюнкция)
        { n = 5; break; }
    case '^': ///Если это ^ или / , то высшее значение приоритета (4) (конъюнкция)
        { n = 4; break; }
    case '+': ///Если это + , то среднее значение приоритета (3) (дизъюнкция)
        { n = 3; break; }
    case '-': ///Если это - или =, то низкое значенгие приоритета (2) (импликация, эквивалентность)
    case '=':
        { n = 2; break; }
    case '(': ///Если это (, то самое низкое значение
        { n = 1; break; } ///приоритета (1)
    }
    return n;
}

void operation_plus (int *top, int x) ///Функция для заноса операций в стек (готова)
{
    *top=*top+1; ///Собственно когда приходим с переменной, то повышаем значение top на 1
    st[*top]=x;  ///И присваиваем этому значению (макс.) значение новой операции
}

char operation_minus (int *top) ///Функция для выноса операций из стек (готова)
{
    char x; ///Выводной символ
    x=st[*top]; ///Он равен последнему значению в стек
    *top=*top-1; ///Теперь опускаемся ниже
    return x; ///Передаем операцию
}

char opz(char* a, char* out, int top, int k, int p) ///Функция преобразования в ОПЗ (готова)
{
    int i=0, j=0, r, v=0;
    int h;
    unsigned char sym;
    int c[256]={0};
    char digits[256];
    while ( a[k]!='\0' ) ///Пока строка не законччилась
    {
        if ( a[k]==')' ) ///Если встречаем ), то
        {
            while( st[top]!='(' ) ///Все значения в стеке до ( выполняем
            {
                out[p++]=operation_minus(&top);
            }
            operation_minus(&top); ///Чистим эту скобочку
        }

        if (( a[k] >='a' && a[k]<='z') || ( a[k] >='A' && a[k]<='Z')) ///Если значение символа - константа (буква), то
        {
            out[p++]=a[k]; ///Заносим в выводную строку стразу
        }

        if ( a[k] >='0' && a[k]<='9') ///Если значение символа - константа (буква), то
        {
            out[p++]=a[k]; ///Заносим в выводную строку стразу
        }


        if ( a[k] == '(' ) ///Если значение символа это (, то
        {
            operation_plus(&top, '('); ///Передаем в ф-ю количество операций в стеке и код (
        }

        if ( a[k]=='+' || a[k]=='-' || a[k]=='^' || a[k]=='=' || a[k]=='!' ) ///Если значение символа это знак, то
        {
            if ( top==0 ) ///Если в стеке ничего нет, то сразу туда и заносим наше значение
            {
                operation_plus(&top, a[k]);
            }

            else ///Если же нет, то проверяем приоритет
            {
                if ( priority(st[top]) < priority(a[k]) ) ///Если приоритет того, что есть, больше, того, что сейчас, то
                {
                    operation_plus(&top, a[k]); ///Ставим то, что есть сейчас, на топ стека
                }
                else ///Если приоритет того, что есть, меньше, того, что сейчас, то
                {
                    while ( priority(st[top]) >= priority(a[k]) ) ///Пока приоритет того, что в стеке, больше (равен) того, что сейчас
                    {
                        out[p++]=operation_minus(&top); ///Присваиваем значение операции к последнему в выводной строке
                                                        ///По сути делаем операци
                    }
                    operation_plus(&top, a[k]);
                }
            }
        }

        k++; ///Переходим на другой символ
    }

    while(top!=0 || top > 0)
    {
        out[p++]=operation_minus(&top);
    }
    out[p++] = 0;
    return *out;
}

char digit (char* out) ///Функция счета (готова)
{
    int t=0, j=0, k=0, v=0, l=0, v1=0; ///Разные счетчики и т.д.
    int x, y, rez=0;
    char symbol;
    v1=strlen(out);
    t=0;
    while (out[t]!='\0') ///Пока не закончена строка идем
    {
        while (out[t]!=';')
        {
        ///t++;
        if ((out[t] != '+') || (out[t] != '=') || (out[t] != '-') || (out[t] != '^') || (out[t] != '!')) ///Если константа - идем дальше
        {
            t++;
        }
        if ((out[t] == '+') || (out[t] == '=') || (out[t] == '-') || (out[t] == '^') || (out[t] == '!')) ///Если нет, идем считать
        {
            j=t-1; ///Берем две константы рядом с операцией
            k=t-2;
            if (out[t] == '+') ///Если это +
            {
                x=out[k]-'0'; ///Переводим два операнда в инт
                y=out[j]-'0';
                if ((x == 1) && (y == 1)) ///Считаем результат и переводим символ
                {
                    rez = 1;
                }
                else
                {
                    rez = x+y;
                }
                symbol=rez+'0';
                out[k]=symbol; ///Ставим символ в строку
                out[j]=out[t+1];
                j=j+1;
                for (long i = j; i < v1; ++i) ///Убираем ненужные два символа
                {
                  out[i] = out[i + 1];
                }
                for (long i = j; i < v1; ++i)
                {
                  out[i] = out[i + 1];
                }
                --v1;
                t=0;

            }
            if (out[t] == '=') ///Если это =
            {
                x=out[k]-'0'; ///Переводим два операнда в инт
                y=out[j]-'0';
                if (((x == 1) && (y == 1)) || ((x == 0) && (y == 0))) ///Считаем результат и переводим символ
                {
                    rez = 1;
                }
                else
                {
                    rez = 0;
                }
                symbol=rez+'0';
                out[k]=symbol; ///Ставим символ в строку
                for (long i = j; i < v1; ++i) ///Убираем ненужные два символа
                {
                  out[i] = out[i + 1];
                }
                --v1;
                for (long i = j; i < v1; ++i)
                {
                  out[i] = out[i + 1];
                }
                --v1;
                t=0;
            }
            if (out[t] == '-') ///Если это -
            {
                x=out[k]-'0'; ///Переводим два операнда в инт
                y=out[j]-'0';
                if ((x == 1) && (y == 0)) ///Считаем результат и переводим символ
                {
                    rez = 0;
                }
                else
                {
                    rez = 1;
                }
                symbol=rez+'0';
                out[k]=symbol; ///Ставим символ в строку
                for (long i = j; i < v1; ++i) ///Убираем ненужные два символа
                {
                  out[i] = out[i + 1];
                }
                --v1;
                for (long i = j; i < v1; ++i)
                {
                  out[i] = out[i + 1];
                }
                --v1;
                t=0;
            }
            if (out[t] == '^') ///Если это ^
            {
                x=out[k]-'0'; ///Переводим два операнда в инт
                y=out[j]-'0';
                rez = x * y; ///Считаем результат и переводим символ
                symbol=rez+'0';
                out[k]=symbol; ///Ставим символ в строку
                for (long i = j; i < v1; ++i) ///Убираем ненужные два символа
                {
                  out[i] = out[i + 1];
                }
                --v1;
                for (long i = j; i < v1; ++i)
                {
                  out[i] = out[i + 1];

                }
                --v1;
                t=0;
            }
            if (out[t] == '!') ///Если это !
            {
                y=out[j]-'0'; ///Переводим операнд в инт
                if (y == 1) ///Считаем результат и переводим символ
                {
                    rez = 0;
                }
                else
                {
                    rez = 1;
                }
                symbol=rez+'0'; ///Ставим символ в строку
                out[j]=symbol;
                for (long i = j+1; i < v1; ++i) ///Убираем ненужный символа
                {
                  out[i] = out[i + 1];
                }

                --v1;
                t=0;
            }
        }

        }
        t++;
    }
    v1=1;
    while (out[v]!='\0')
    {
        if (out[v] >= '0' && out[v] <= '1')
        {
            l=out[v]-'0';
            v1=v1*l;
        }
        v++;
    }
    symbol=v1+'0';
    return symbol;
}

int amount (char* digits) ///Функция плодсчета переменных (готова)
{
    int h=0, v=0;
    unsigned char sym;
    while (digits[v]!='\0')
    {
        sym=digits[v];
        h=sym;
        c[h]++;
        v++;
    }

}

void every (char* a1) ///Функция сцепления в строку (готова)
{
    int i=0, j=0;
    for (i=0; i<strlen(a1); i++)
    {
        all[all_int]=a1[i];///Сцепляем строку в одну большую
        all_int++;
    }
    ///all_int++;
    all[all_int]=';';///И разделяем их точкой с запятой
    all_int++;
}

int check(char* a1) ///Функция проверки синтаксиса (готова)
{
    int i=0, j=0;
    for (i=0; i<strlen(a1); i++)///Пока строка не закончится, проверяем
    {
        if (a1[i]  == '(')///Если мы нашли (, то
        {
            j=i+1;
            if (a1[j] == ')')///Если за ней сразу идет ), то ошибка
            {
                printf("Syntax Eror [there is a ) right after the (]!");
                return 0;
            }
            else
            {
                while (a1[j]!='\0')
                {
                    if (a1[j] == ')')///Если мы находим ), то все правильно, иначе - ошибка
                        goto metka;
                    j++;
                }
                printf("Syntax Eror [there is no a ) after the (]!");
                return 0;
                metka:;
                j++;
                ///printf(" ");
            }
        }
        j=i+1;
        if ((a1[i]>='a' && a1[i]<='z') || (a1[i]>='A' && a1[i]<='Z'))///Если два операнда идут подряд, то -ошибка
        {
            if ((a1[j]>='a' && a1[j]<='z') || (a1[j]>='A' && a1[j]<='Z'))
            {
                printf("Syntax Eror[there is a letter right after the another one]!");
                return 0;
            }
        }
        if (a1[i]>='0' && a1[i]<='9')///Если две константы идут подряд, то -ошибка
        {
            if (a1[j]>='0' && a1[j]<='9')
            {
                printf("Syntax Eror[there is a digit right after the another one]!");
                return 0;
            }
        }
        if (a1[i]=='^' || a1[i]=='+' || a1[i]=='-' || a1[i]=='=' || a1[i]=='!')///Если две операции идут подряд , то - ошибка (доделать вариант ошибки -!x1)
        {
            if (a1[j]=='^' || a1[j]=='+' || a1[j]=='-' || a1[j]=='=')
            {
                printf("Syntax Eror[there is a operation right after the another one]!");
                return 0;
            }
        }
    }
}

char convert(char* a1) ///Функция перевода переменных (готова, до делать до 12 переменной)
{
    int i=0, j=0, h=0, k=0, v1=0;
    ///v1=strlen(a1);
    while (a1[i]!='\0') ///Тут заменяем двухсимвольные переменные на односимвольные
    {
        if (a1[i] == 'x')
        {
            j=i+1;
            k=i+2;
            if (a1[j] == '1')
                a1[i] = 'a';
            if (a1[j] == '2')
                a1[i] = 'b';
            if (a1[j] == '3')
                a1[i] = 'c';
            if (a1[j] == '4')
                a1[i] = 'd';
            if (a1[j] == '5')
                a1[i] = 'e';
            if (a1[j] == '6')
                a1[i] = 'f';
            if (a1[j] == '7')
                a1[i] = 'g';
            if (a1[j] == '8')
                a1[i] = 'h';
            if (a1[j] == '9')
                a1[i] = 'i';
            if (a1[j] == '1' && a1[k] == '0')
                a1[i] = 'B';
            if (a1[j] == '1' && a1[k] == '1')
                a1[i] = 'C';
            if (a1[j] == '1' && a1[k] == '2')
                a1[i] = 'D';
            if (a1[j] == '1' && a1[k] == '3')
                a1[i] = 'K';
            if ((a1[j] >= '0' && a1[j] <= '9') && (a1[k] >= '0' && a1[k] <= '9'))
            {
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
            }
            else
            {
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
            }
        }
        if (a1[i] == 'y')
        {
            j=i+1;
            if (a1[j] == '1')
                a1[i] = 'j';
            if (a1[j] == '2')
                a1[i] = 'k';
            if (a1[j] == '3')
                a1[i] = 'l';
            if (a1[j] == '4')
                a1[i] = 'm';
            if (a1[j] == '5')
                a1[i] = 'n';
            if (a1[j] == '6')
                a1[i] = 'o';
            if (a1[j] == '7')
                a1[i] = 'p';
            if (a1[j] == '8')
                a1[i] = 'q';
            if (a1[j] == '9')
                a1[i] = 'r';
            if (a1[j] == '1' && a1[k] == '0')
                a1[i] = 'E';
            if (a1[j] == '1' && a1[k] == '1')
                a1[i] = 'F';
            if (a1[j] == '1' && a1[k] == '2')
                a1[i] = 'G';
            if (a1[j] == '1' && a1[k] == '3')
                a1[i] = 'L';
            if ((a1[j] >= '0' && a1[j] <= '9') && (a1[k] >= '0' && a1[k] <= '9'))
            {
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
            }
            else
            {
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
            }
        }
        if (a1[i] == 'z')
        {
            j=i+1;
            if (a1[j] == '1')
                a1[i] = 's';
            if (a1[j] == '2')
                a1[i] = 't';
            if (a1[j] == '3')
                a1[i] = 'u';
            if (a1[j] == '4')
                a1[i] = 'v';
            if (a1[j] == '5')
                a1[i] = 'w';
            if (a1[j] == '6')
                a1[i] = 'x';
            if (a1[j] == '7')
                a1[i] = 'y';
            if (a1[j] == '8')
                a1[i] = 'z';
            if (a1[j] == '9')
                a1[i] = 'A';
            if (a1[j] == '1' && a1[k] == '0')
                a1[i] = 'H';
            if (a1[j] == '1' && a1[k] == '1')
                a1[i] = 'I';
            if (a1[j] == '1' && a1[k] == '2')
                a1[i] = 'J';
            if (a1[j] == '1' && a1[k] == '3')
                a1[i] = 'M';
            if ((a1[j] >= '0' && a1[j] <= '9') && (a1[k] >= '0' && a1[k] <= '9'))
            {
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
            }
            else
            {
                for ( h = j; h < strlen(a1); ++h) ///Убираем ненужный символ
                {
                  a1[h] = a1[h + 1];
                }
            }
        }
        i++;
    }
}

int main()
{
    int top=0; ///Изначальное значение стек = 0;
    char a[80],   ///Исходная строка
         symbol,
         out[256]; ///Выводная строка
    char *a1;
    int k=0, ///Счетчики
        i=0,
        p=0;
    counter =0;
    input = fopen("data.txt", "r");
    if (input == NULL)
    {
        printf("Eror");
        return 0;
    }
    while (1)
    {
        a1 = fgets (a, sizeof(a), input);
        if (a1 == NULL)
        {
            if(feof(input)!=0)
            {
                break;
            }
            else
            {
                printf("Eror");
                break;
            }
        }
        if(a1[strlen(a1)-1]=='\n')
        {
            a1[strlen(a1)-1]='\0';
        }
        convert(a1);
        if (check(a1) == 0)
        {
            return 0;
        }
        amount(a1);
        opz(a1, out, top, k, p);
        every(out);
    }
    for (i=0; i< 256; i++)
        if ((i>=65 && i<=90) || (i>=97 && i<=122))
            if(c[i]>0)
                kol++;
    f(all, symbol);
    printf("Answer: "); printf("%d\n", counter);
    printf("Number of letters: "); printf("%d\n", kol);
    printf("Developer: Kirill Korneev (telegram - @Korneev_Kirill; mail - kirill.corneew2017@yandex.ru) ");
    printf("\n");printf("\n");printf("\n");
    printf("*Press Enter to stop the programm*");
    return 0;
}



