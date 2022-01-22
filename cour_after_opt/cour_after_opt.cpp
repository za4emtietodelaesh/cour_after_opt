//post-opt
#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include<conio.h>
#include<math.h>
#include "time.h"
#include<windows.h>
#include<stdint.h>
//TODO:
//-
// 
//Объявление переменных
char dict[100000][20] = { "\0" ,"\0" }; //переменная dict и dictNew нужны для реализации словаря в алгоритме LZ
char dictNew[100000][20] = { "\0" ,"\0" };
int sizes[100000];
int counters[100000];
int two = 0;
int NEW[100][100] = { 0 };
//
//
/// 


void qs(char* temp1, int* s_arr, int first, int last)
{
    if (first < last)
    {
        int left = first, right = last, middle = s_arr[(left + right) / 2];
        do
        {
            while (s_arr[left] > middle) left++;
            while (s_arr[right] < middle) right--;
            if (left <= right)
            {
                int tmp = s_arr[left];
                s_arr[left] = s_arr[right];
                s_arr[right] = tmp;
                char tmp1 = temp1[left];
                temp1[left] = temp1[right];
                temp1[right] = tmp1;
                left++;
                right--;
            }
        } while (left <= right);
        qs(temp1, s_arr, first, right);
        qs(temp1, s_arr, left, last);
    }
} //оптимизация путем смены "пузырьковой" сортировки на "быструю"


/// <summary>
/// вспомогательная функция для кодирования фано 
/// </summary>
/// <param name="codes">массив символов, используемый для кодирования Фано</param>
/// <param name="c">индекс смещения символов</param>
/// <param name="byte">размер символа</param>
/// <param name="bytecode">размер символа разпределенный в массив </param>
/// <param name="sizeofcode">размер файла в коде</param>
void Mcode(int* codes, int c, int byte, int* bytecode, int sizeofcode) {
    if (c > -1)
        bytecode[c] = byte;
    c++;
    int i = 0, l = 0, sum = 0;
    for (int i = 0; i < sizeofcode; i++) {
        sum += codes[i];
    }
    if (sizeofcode > 1)
        while (1) {
            if (l + codes[i] < sum / 2) {
                l += codes[i];
            }
            else {
                if ((l == 0) || (codes[i] - (sum - l) < sum / 8))l += codes[i];
                i++;
                break;

            }
            i++;
        }
    else {
        int* temp = NEW[two];
        for (int i = 0; i < c; i++)temp[i] = bytecode[i];
        NEW[two][c] = -1;
        two++;
        return;
    }
    int* temp = (int*)malloc(sizeof(int) * i);
    for (int* j = &temp[0], *h = &codes[0]; j < &temp[i]; j++, h++) {
        *j = *h;
    }
    Mcode(temp, c, 0, bytecode, i);
    temp = (int*)realloc(temp, sizeof(int) * (sizeofcode - i));
    if (temp == NULL)printf("Error not enough space");
    for (int j = i, k = 0; k < sizeofcode - i; j++, k++) {
        temp[k] = codes[j];
    }
    Mcode(temp, c, 1, bytecode, (sizeofcode - i));
}

void coding(char PATH[500], int* SIZE) //первичное кодирование алгоритмом Фано
{
    two = 0;
    FILE* open = fopen(PATH, "r");
    char temp[500] = { '\0' };
    int T = 0;
    char l = fgetc(open);
    for (int k = 0, s = 0; l != EOF; l = fgetc(open)) {
        for (int j = 0; temp[j] != '\0'; j++)if (temp[j] == l)k++;
        if (k == 0) {
            temp[s] = l;
            s++;
            T++;
        }
        else k = 0;
        (*SIZE)++;
    }
    rewind(open);
    l = fgetc(open);
    int* amount = (int*)calloc(T, sizeof(int));
    for (; l != EOF; l = fgetc(open)) {
        for (int j = 0; j < T; j++) {
            if (l == temp[j]) {
                amount[j]++;
            }
        }
    }
    qs(temp, amount, 0, T - 1);
    int* string = (int*)malloc(sizeof(int) * 100);
    if (T > 1)Mcode(amount, -1, 0, string, T);
    else {
        NEW[0][0] = 0;
        NEW[0][1] = -1;
    }
    FILE* out = fopen("C:\\Users\\f7kso\\Desktop\\key.txt", "w");
    for (int i = 0; i < T; i++) {// запись кодов
        fputc(temp[i], out);
        for (int j = 0; NEW[i][j] != -1; j++)fputc((NEW[i][j] + 48), out);
        fputc('\n', out);
    }
    rewind(open);
    fclose(out);
    out = fopen("C:\\Users\\f7kso\\Desktop\\codedF.txt", "wb+");
    l = fgetc(open);
    int NewSize = 0;
    while (l != EOF) {
        for (int i = 0; i < T; i++) {
            if (l == temp[i]) {
                for (int j = 0; NEW[i][j] != -1; j++) {
                    fputc((NEW[i][j] + 48), out);
                    NewSize++;
                }
                break;
            }
        }
        l = fgetc(open);
    }
    fclose(out);
    fclose(open);
}

void encoded() { //реализованный алгоритм декодирования Фано
    FILE* open = fopen("C:\\Users\\f7kso\\Desktop\\Encoded.txt", "r");
    FILE* out = fopen("C:\\Users\\f7kso\\Desktop\\EncodedF.txt", "w");
    FILE* key = fopen("C:\\Users\\f7kso\\Desktop\\key.txt", "r");
    char t = fgetc(key);
    char letter[500];
    two = 0;
    int Pass[100][500];
    for (int l = 0; t != EOF; l++, t = fgetc(key), two++) {
        letter[l] = t;
        t = fgetc(key);
        for (int i = 0, *temp = Pass[two]; t != '\n'; i++, t = fgetc(key))
            temp[i] = t - 48;
    }
    fclose(key);
    int* counter = (int*)calloc(two, sizeof(int));
    int tester = 0;
    t = fgetc(open);
    while (t != EOF) {
        for (int* p = &counter[0]; p < &counter[two]; p++)
            *p = 0;
        for (int j = 0; t != EOF; j++, t = fgetc(open)) {
            for (int i = 0; i < two; i++) {
                if (Pass[i][j] == (int)t - 48) {
                    counter[i]++;
                }
                else counter[i] = -100;
            }
            for (int p = 0; p < two; p++) {
                if (counter[p] > 0)tester++;
            }
            if (tester == 1) {
                for (int q = 0; q < two; q++) {
                    if (counter[q] > 0) {
                        fputc(letter[q], out);
                        break;
                    }
                }
                break;
            }
            tester = 0;
        }
        t = fgetc(open);
    }
    fclose(open);
    fclose(out);
    remove("C:\\Users\\f7kso\\Desktop\\key.txt");
    remove("C:\\Users\\f7kso\\Desktop\\codedF.txt");
    remove("C:\\Users\\f7kso\\Desktop\\Encoded.txt");
    //printf("END");
}

void codingS() //вторичное кодирование алгоритмом LZ
{
    FILE* open = fopen("C:\\Users\\f7kso\\Desktop\\codedF.txt", "r");
    FILE* amount_of_num = fopen("C:\\Users\\f7kso\\Desktop\\amount.txt", "w+");
    char str;
    memset(counters, 0, sizeof(int) * 100000);
    memset(sizes, 0, sizeof(int) * 100000);
    memset(dict, 0, sizeof(char) * 100000);
    memset(dictNew, 0, sizeof(char) * 100000);
    int runner = 0, runnerNew = 0;
    int tempMax = 0, Temp = 0, check = 0;
    fseek(open, 0, SEEK_END);
    printf("Origianl size:%d bytes\n", ftell(open) - 1);
    rewind(open);
    for (int i = 0; !feof(open); i++) {
        str = fgetc(open);
        check = 0;
        for (int j = 0; j < runner; j++) {
            for (int k = 0; k < sizes[j]; k++) {
                if (str == EOF) {
                    for (int a = j, g = 0; a >= 0; a--) {
                        if (counters[a] > 0) {
                            strcpy(dictNew[runnerNew], dictNew[a]);
                            strcpy(dict[runner], dict[a]);
                            rewind(amount_of_num);
                            char gl = fgetc(amount_of_num);
                            while (g != a) {
                                g++;
                                gl = fgetc(amount_of_num);
                            }
                            fseek(amount_of_num, 0, SEEK_END);
                            fputc(gl, amount_of_num);
                            runnerNew++;
                            runner++;
                            break;
                        }
                    }
                    Temp = -1;
                    break;
                }
                else if (str == dict[j][k]) {
                    Temp++;
                    str = fgetc(open);
                }
                else {
                    fseek(open, (-Temp - 1), SEEK_CUR);
                    str = fgetc(open);
                    Temp = 0;
                    break;
                }
            }
            if (Temp == -1) {
                break;
            }
            counters[j] = Temp;
            if (str == EOF) {
                fseek(open, (-Temp), SEEK_END);
                str = fgetc(open);
            }
            else {
                fseek(open, (-Temp - 1), SEEK_CUR);
                str = fgetc(open);
            }
            Temp = 0;
        }
        for (int z = runner - 1; z >= 0; z--) {
            if (Temp == -1)break;
            if (counters[z] != 0) {
                fseek(open, counters[z] - 1, SEEK_CUR);
                str = fgetc(open);
                check = 1;
                if (str == EOF) {
                    if (z < 9) {
                        dictNew[runnerNew][0] = 48;
                        fputc(49, amount_of_num);
                        //fputc('\n', amount_of_num);
                        fseek(open, counters[z] - 2, SEEK_CUR);
                        str = fgetc(open);
                        dictNew[runnerNew][1] = str;
                    }
                    else {
                        int k = 0;
                        z++;
                        for (int i = 0, l = z; l > 0; i++) {
                            l /= 10;
                            k = i;
                        }
                        for (int i = k, l = z; i >= 0; i--) {
                            dictNew[runnerNew][i] = (l % 10) + 48;
                            l /= 10;
                        }
                        fputc(k + 49, amount_of_num);
                        fseek(open, counters[z - 1] - 2, SEEK_CUR);
                        str = fgetc(open);
                        dictNew[runnerNew][k + 1] = str;
                    }
                }
                else {
                    if (z < 9) {
                        dictNew[runnerNew][0] = z + 49;
                        fputc(49, amount_of_num);
                        dictNew[runnerNew][1] = str;
                    }
                    else {
                        int k = 0;
                        z++;
                        for (int i = 0, l = z; l > 0; i++) {
                            l /= 10;
                            k = i;
                        }
                        for (int i = k, l = z; i >= 0; i--) {
                            dictNew[runnerNew][i] = (l % 10) + 48;
                            l /= 10;
                        }
                        fputc(k + 49, amount_of_num);
                        dictNew[runnerNew][k + 1] = str;
                        z--;
                    }
                }
                runnerNew++;
                for (int l = 0; l < sizes[z]; l++) {
                    dict[runner][l] = dict[z][l];
                }
                dict[runner][sizes[z]] = str;
                sizes[runner] = sizes[z] + 1;
                runner++;
                break;
            }
        }
        if (check == 0 && Temp != -1) {
            dict[runner][0] = str;
            sizes[runner] = 1;
            dictNew[runnerNew][0] = 48;
            fputc(49, amount_of_num);
            dictNew[runnerNew][1] = str;
            runner++;
            runnerNew++;
        }
    }
    fclose(open);
    fclose(amount_of_num);
    FILE* out = fopen("C:\\Users\\f7kso\\Desktop\\coded.txt", "w");
    for (int i = 0; i < runnerNew; i++)fputs(dictNew[i], out);
    printf("New size:%d bytes\n", ftell(out) - 1);
    fclose(out);
    out = fopen("C:\\Users\\f7kso\\Desktop\\dictionary.txt", "w");
    for (int i = 0; i < runnerNew; i++) {
        fputs(dict[i], out);
        fputc('\n', out);
    }
    fclose(out);
    printf("CODED!");
}

void encodedS(char coders[]) { //декодирование после второго сжатия алгоритмом LZ
    FILE* d = fopen("C:\\Users\\f7kso\\Desktop\\dictionary.txt", "r");
    FILE* open = fopen(coders, "r");
    FILE* out = fopen("C:\\Users\\f7kso\\Desktop\\Encoded.txt", "w");
    FILE* amount_of_num = fopen("C:\\Users\\f7kso\\Desktop\\amount.txt", "r");
    int counter, amount, counter1, l;
    char temp[10000], str = fgetc(open), runner, AON;
    while (!feof(open)) {
        AON = fgetc(amount_of_num);
        amount = 0;
        counter = 0;
        counter1 = 0;
        for (int i = 0; i < (AON - 48); i++) {
            amount *= 10;
            amount += (str - 48);
            temp[counter] = str;
            counter++;
            counter1++;
            str = fgetc(open);
        }
        temp[counter] = str;
        counter++;
        str = fgetc(open);
        while (1) {
            if (temp[0] == '0') {
                fputc(temp[1], out);
                break;
            }
            else {
                runner = '0';
                for (int i = 0; i < (amount - 1);) {
                    if (runner == '\n')i++;
                    runner = fgetc(d);
                }
                if (amount == 1)runner = fgetc(d);
                while (runner != '\n' && runner != EOF) {
                    fputc(runner, out);
                    runner = fgetc(d);
                }
                fseek(d, 0, SEEK_SET);
                fputc(temp[counter1], out);
                break;
            }
        }
    }
    fclose(open);
    fclose(out);
    fclose(d);
    fclose(amount_of_num);
    remove("C:\\Users\\f7kso\\Desktop\\dictionary.txt");
    remove("C:\\Users\\f7kso\\Desktop\\coded.txt");
    remove("C:\\Users\\f7kso\\Desktop\\amount.txt");
}

void coder(char text[], int* size) { //функция кодировани двумя алгоритмами: фано и Lz
    coding(text, size);
    codingS();
}

void encoder(char code[]) { //функция декодирования в обратном порядке
    encodedS(code);
    encoded();
}

int main() { //основное тело программмы
    int i = 0;
    int size = 0;
    while (i != 3) {
        printf("what to do?\n1)Code\n2)encode\n3)exit\n");
        scanf("%d", &i);
        if (i == 1) {
            printf("press path to the text:\n");
            char t[500] = "C:\\Users\\f7kso\\Desktop\\test.txt";
            // scanf("%s", t);
            double start = clock();
            coder(t, &size);
            //system("cls");
            printf("it takes:%.4lf\n", (clock() - start) / CLOCKS_PER_SEC);
        }
        else if (i == 2) {
            printf("press path to the codes:\n");
            char t[500] = "C:\\Users\\f7kso\\Desktop\\coded.txt";
            // scanf("%s", t);
            // scanf("%s", k);
            double start = clock();
            encoder(t);
            //system("cls");
            printf("it takes:%.4lf\n", (clock() - start) / CLOCKS_PER_SEC);
        }
    }
    printf("END");
}
