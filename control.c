#include "parser.h"
#include "atom.h"
#include "eval.h"

void installPackage() {
  installNumberPackage();
  installSymbolPackage();
  installAtomPackage();
  installEvalPackage();
}

int main() {
  installPackage();
  while (1) {
    fprintf(stderr, "> ");
    Atom* exp = parseFromFile(stdin);
    Atom* value = eval(exp, env0);
    printAtom(stdout, value);
    printf("\n");
  }
  return 0;
}
