#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>

typedef int Symbol;

Symbol createSymbolFromStr(char* str);
void deleteSymbol(Symbol symbol);

int symboleq_(Symbol x, Symbol y);

void printSymbol(FILE* file, Symbol x);

void installSymbolPackage();

#endif
