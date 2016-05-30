#ifndef EVALAPPLY_H
#define EVALAPPLY_H

#include "atom.h"

extern Atom* env0;
Atom* eval(Atom* exp, Atom* env);
Atom* apply(Atom* proc, Atom* argl, Atom* applyEnv);

void printAtom(FILE* file, Atom* atom);

void installEvalPackage();

#endif
