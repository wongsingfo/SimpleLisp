#ifndef ATOM_H
#define ATOM_H

#include "number.h"
#include "symbol.h"
#include "exception.h"

typedef struct Atom Atom;

enum AtomTag {
  PAIR,
  NUMBER,
  SYMBOL,
  PRIMITIVE,
  COMPOUND,
  BOOLEAN,
};

struct Atom {
  enum AtomTag tag;
  int ref;  // used for gargage collection
  union {
    Atom* child[2];
    Number number;
    Symbol symbol;
    Atom* (*primitive)(Atom*);
    struct {
      Atom* body;
      Atom* params;
      Atom* env;
    }compound;
    int boolean;
  }data;
};

Atom* nil = NULL;
Atom* symbol_ok;
Atom* symbol_error;
Atom* boolean_true;
Atom* boolean_false;

Atom* car(Atom* x);
Atom* cdr(Atom* x);
Atom* cons(Atom* x, Atom* y);

Atom* createAtomFromSymbol(Symbol symbol);
Atom* createAtomFromNumber(Number number);

void installAtomPackage();

#endif
