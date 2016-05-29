#ifndef EVALAPPLY_H
#define EVALAPPLY_H

#include "atom.h"

Atom* eval(Atom* exp, Atom** env);
// '(lambda args env . body)
Atom* apply(Atom* proc, Atom* argl, Atom** applyEnv);

extern Atom* env0;

void printAtom(FILE* file, Atom* atom);

void installEvalPackage();

#endif
