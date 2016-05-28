#ifndef SYMBOL_H
#define SYMBOL_H

typedef int Symbol;

Symbol createSymbolFromStr(char* str);
void deleteSymbol(Symbol symbol);

int symboleq_(Symbol x, Symbol y);

void installSymbolPackage();

#endif
