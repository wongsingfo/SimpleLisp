#ifndef ATOM_H
#define ATOM_H

#include "number.h"
#include "symbol.h"

typedef struct Atom Atom;

enum AtomType {
  atomPair,
  atomNumber,
  atomSymbol,
};

struct Atom {
  enum AtomType type;
  union {
    struct Atom* child[2]; 
    Number number;
    Symbol symbol;
  }data;
};

Atom* car(Atom* x);
Atom* cdr(Atom* x);
Atom* cons(Atom* x, Atom* y);

// treat nil, #t, #f as a symbol
extern Atom* nil; 
extern Atom* tAtom;
extern Atom* fAtom;
Atom* createReservedSymbol(char* symbol);

Atom* createSymbol(Symbol symbol);
Atom* createNumber(Number number);

void installAtomPackage();

#endif
