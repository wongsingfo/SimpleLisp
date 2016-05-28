/* If possible, use bignum instead of long long. */  

#ifndef NUMBER_H
#define NUMBER_H

#include <stdio.h>

typedef long long Number;

Number createNumberFromStr(char* str);
void deleteNumber(Number number);

int numbereq_(Number x, Number y);
Number numberadd(Number x, Number y);

void printNumber(FILE* file, Number x);

void installNumberPackage();

#endif
