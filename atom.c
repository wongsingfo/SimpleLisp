#include "atom.h"
#include "exception.h"

Atom* car(Atom* x) {
  if (x->type == objectPair) {
    return x->child[0];
  }
  else {
    exception("car", "Not a pair");
    return nil;
  }
}

Atom* cdr(Atom* x) {
  if (x->type == objectPair) {
    return x->child[1];
  }
  else {
    exception("cdr", "Not a pair");
    return nil;
  }
}

