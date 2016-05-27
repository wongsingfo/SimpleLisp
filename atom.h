#ifndef ATOM_H
#define ATOM_H

#include "symbol.h"

enum AtomType{
  atomPair,
  atomNumber,
  atomSymbol,
};

struct Atom {
  AtomType type;
  union Data {
    Atom* child[2]; 
    Number number;
    Symbol symbol;
  }data;
};

Atom* car(Atom* x);
Atom* cdr(Atom* x);
Atom* cons(Atom* x, Atom* y);
Atom* eq_(Atom* x, Atom* y);

// treat nil, #t, #f as a symbol
extern Atom* nil; 
extern Atom* tAtom;
extern Atom* fAtom;

void installAtomPackage();

#endif
