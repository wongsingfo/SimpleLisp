/* If possible, use bignum instead of long long. */  

#ifndef NUMBER_H
#define NUMBER_H

typedef long long Number;

Number createNumber(char* str);
void deleteNumber(Number number);

int numbereq_(Number x, Number y);
Number numberadd(Number x, Number y);

void installNumberPackage();

#endif
