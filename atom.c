#include "atom.h"
#include "exception.h"

#include <malloc.h>

Atom* nil;
Atom* tAtom;
Atom* fAtom;

Atom* car(Atom* x) {
  if (x->type == atomPair) {
    return x->data.child[0];
  }
  else {
    exception("car", "Not a pair");
    return nil;
  }
}

Atom* cdr(Atom* x) {
  if (x->type == atomPair) {
    return x->data.child[1];
  }
  else {
    exception("cdr", "Not a pair");
    return nil;
  }
}

Atom* cons(Atom* x, Atom* y) {
  Atom* result = (Atom*) malloc(sizeof(Atom));
  result->type = atomPair;
  result->data.child[0] = x;
  result->data.child[1] = y;
  return result;
}

Atom* createReservedSymbol(char* symbol) {
  Atom* result = (Atom*) malloc(sizeof(Atom));
  result->type = atomSymbol;
  result->data.symbol = createSymbol(symbol);
}

void installAtomPackage() {
  nil = createReservedSymbol("nil");
  tAtom = createReservedSymbol("#t");
  fAtom = createReservedSymbol("#f");
}
