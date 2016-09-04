#include "eval.h"
#include "exception.h"

#define returnNormally(val) popStackMessage(); return val;

#define numberOfPrimitiveProc 20

Atom* env0;
int primitiveCount;
Symbol primitiveProcSymbol[numberOfPrimitiveProc];
Atom* (*primitiveProc[numberOfPrimitiveProc])(Atom* argl, Atom* env);

Atom* symbolLambda;
Atom* symbolEnvironment;

int pair_(Atom* exp) {
  if (exp->type == atomPair) {
    return 1;
  }
  else {
    return 0;
  }
}

int number_(Atom* exp) {
  if (exp->type == atomNumber) {
    return 1;
  }
  else {
    return 0;
  }
}

int symbol_(Atom* exp) {
  if (exp->type == atomSymbol) {
    return 1;
  }
  else {
    return 0;
  }
}

int null_(Atom* exp) {
  if (exp == nil) {
    return 1;
  }
  else {
    return 0;
  }
}

int eq_(Atom* x, Atom* y) {
  if (x->type == y->type) {
    if (x->type == atomSymbol) {
      if (symboleq_(x->data.symbol, y->data.symbol)) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if (x->type == atomNumber) {
      if (numbereq_(x->data.number, y->data.number)) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else {
      if (x == y) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else {
    return 0;
  }
}

int lambda_(Atom* proc) {
  if (pair_(proc) && eq_(car(proc), symbolLambda)) {
    return 1;
  }
  else {
    return 0;
  }
}

int environment_(Atom* proc) {
  if (pair_(proc) && eq_(car(proc), symbolEnvironment)) {
    return 1;
  }
  else {
    return 0;
  }
}

Atom* lookUp(Atom* symbol, Atom* env) {
  pushStackMessage("look up a variable");
  if (environment_(env)) {
    env = cdr(env);
    while (! null_(env)) {
      Atom* pair = car(env);
      if (symboleq_(car(pair)->data.symbol, symbol->data.symbol)) {
        returnNormally(cdr(pair));
      }
      env = cdr(env);
    }
    exception("can not find symbol");
  }
  else {
    exception("not an environment");
  }
}

Atom* newEnvironemnt() {
  return cons(symbolEnvironment, nil);
}

void appendEnvironment(Atom* env, Atom* name, Atom* value) {
  pushStackMessage("append environment");
  if (environment_(env)) {
    Atom* pair = cons(name, value);
    Atom* data = cons(pair, cdr(env));
    env->data.child[1] = data;
    popStackMessage();
  }
  else {
    exception("not an environment");
  }
}

Atom* extendEnvironment(Atom* env, Atom* name, Atom* argl) {
  pushStackMessage("extend environment");
  if (environment_(env)) {
    Atom* newEnv = newEnvironemnt();
    newEnv->data.child[1] = env->data.child[1];
    
    while (1) {
      if (null_(name) && null_(argl)) {
        returnNormally(newEnv)
      }
      else if (null_(name)) {
        exception("too many arguments");
      }
      else if (null_(argl)) {
        exception("too few arguments");
      }
      else {
        appendEnvironment(newEnv, car(name), car(argl));
      }
      name = cdr(name);
      argl = cdr(argl);
    }
  }
  else {
    exception("not an environment");
  }
}

Atom* evalSequence(Atom* exp, Atom* env) {
  if (null_(exp)) {
    return nil;
  }
  else {
    Atom* result = eval(car(exp), env);
    return cons(result, evalSequence(cdr(exp), env));
  }
}

Atom* getProcedureFromExp(Atom* exp) {
  return car(exp);
}

Atom* getArgumentsFromExp(Atom* exp) {
  return cdr(exp);
}

Atom* eval(Atom* exp, Atom* env) {
  pushStackMessage("eval");
  if (pair_(exp)) {
    Atom* proc = eval(getProcedureFromExp(exp), env);
    Atom* argl = getArgumentsFromExp(exp);
    returnNormally(apply(proc, argl, env));
  }
  else if (null_(exp)) {
    exception("evaluate an empty expression");
  }
  else if (symbol_(exp)) {
    returnNormally(lookUp(exp, env));
  }
  else if (number_(exp)) {
    returnNormally(exp);
  }
  else {
    exception("unknown operation");
  }
}

Atom* getEnvironmentFromProc(Atom* proc) {
  return cdr(cdr(cdr(proc)));
}

Atom* getArgumentsFromProc(Atom* proc) {
  return car(cdr(proc));
}

Atom* getBodyFromProc(Atom* proc) {
  return car(cdr(cdr(proc)));
}

Atom* primitiveProcApply(Symbol proc, Atom* argl, Atom* env) {
  pushStackMessage("primitiveProcApply");
  int i;
  for (i = 0; i < primitiveCount; i++) {
    if (symboleq_(proc, primitiveProcSymbol[i])) {
      returnNormally((*primitiveProc[i])(argl, env));
    }
  }
  exception("proc is not primitive or compounded");
}

Atom* apply(Atom* proc, Atom* argl, Atom* applyEnv) {
  pushStackMessage("apply");
  if (lambda_(proc)) {
    Atom* newEnv = 
      extendEnvironment(
        getEnvironmentFromProc(proc),
        getArgumentsFromProc(proc),
        evalSequence(argl, applyEnv)
      );
    returnNormally(eval(getBodyFromProc(proc), newEnv));
  }
  else if (symbol_(proc)) {
    returnNormally(primitiveProcApply(proc->data.symbol, argl, applyEnv));
  }
  else {
    exception("apply: number / string / .. etc can not be applied");
  }
}

void checkLength(Atom* list, int len) {
  if (null_(list)) {
    if (len != 0) {
      exception("check list length: list too short");
    }
    else {
      return;
    }
  }
  else if (len == 0) {
    if (! null_(list)) {
      exception("check list length: list too long");
    }
    else {
      return;
    }
  }
  else {
    checkLength(cdr(list), len - 1);
  }
}

void appendPrimitive(char* name, Atom* (*pri)(Atom* argl, Atom* env)) {
  pushStackMessage("appendPrimitive");
  if (primitiveCount >= numberOfPrimitiveProc) {
    exception("primitiveCount >= numberOfPrimitiveProc");
  }
  else {
    Symbol symbol = createSymbolFromStr(name);
    primitiveProcSymbol[primitiveCount] = symbol;
    primitiveProc[primitiveCount] = pri;
    primitiveCount++;
    
    Atom* tmp = createSymbol(symbol);
    appendEnvironment(env0, tmp, tmp);
    popStackMessage();
  }
}

Atom* priAdd(Atom* argl, Atom* env) {
  Atom* value = evalSequence(argl, env);
  
  if (null_(value)) {
    exception("primitive + (add): no arguments");
  }
  else {
    Atom* i;
    for (i = value; !null_(i); i = cdr(i)) {
      if (! number_(car(i))) {
        exception("primitive + (add): not a number");
      }
    }
  
    Number result = car(value)->data.number;
    for (i = cdr(value); !null_(i); i = cdr(i)) {
      result = numberadd(result, car(i)->data.number);
    }
    return createNumber(result);
  }
}

Atom* priEqual(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* value = evalSequence(argl, env);
  
  Atom* a = car(value);
  Atom* b = car(cdr(value));
  if (number_(a) && number_(b)) {
    if (numbereq_(a->data.number, b->data.number)) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
  else {
    exception("primitive = (equal): not a number");
  }
}

Atom* priLT(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* value = evalSequence(argl, env);
  
  Atom* a = car(value);
  Atom* b = car(cdr(value));
  if (number_(a) && number_(b)) {
    if (numberlt_(a->data.number, b->data.number)) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
  else {
    exception("primitive = (equal): not a number");
  }
}

Atom* priGT(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* value = evalSequence(argl, env);
  
  Atom* a = car(value);
  Atom* b = car(cdr(value));
  if (number_(a) && number_(b)) {
    if (numbergt_(a->data.number, b->data.number)) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
  else {
    exception("primitive = (equal): not a number");
  }
}

Atom* priLE(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* value = evalSequence(argl, env);
  
  Atom* a = car(value);
  Atom* b = car(cdr(value));
  if (number_(a) && number_(b)) {
    if (numberle_(a->data.number, b->data.number)) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
  else {
    exception("primitive = (equal): not a number");
  }
}

Atom* priGE(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* value = evalSequence(argl, env);
  
  Atom* a = car(value);
  Atom* b = car(cdr(value));
  if (number_(a) && number_(b)) {
    if (numberge_(a->data.number, b->data.number)) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
  else {
    exception("primitive = (equal): not a number");
  }
}

Atom* priDefine(Atom* argl, Atom* env) {
  pushStackMessage("define statement");
  checkLength(argl, 2);
  Atom* value = eval(car(cdr(argl)), env);
  
  if (symbol_(car(argl))) {
    appendEnvironment(env, car(argl), value);
    returnNormally(nil);
  }
  else {
    exception("primitive define: symbol expected");
  }
}

Atom* priLambda(Atom* argl, Atom* env) {
  pushStackMessage("lambda statement");
  checkLength(argl, 2);
  Atom* result = cons(car(cdr(argl)), env);
  result = cons(car(argl), result);
  result = cons(symbolLambda, result);
  returnNormally(result);
}

Atom* priIf(Atom* argl, Atom* env) {
  pushStackMessage("if statement");
  checkLength(argl, 3);
  Atom* condition = eval(car(argl), env);
  if (eq_(condition, tAtom)) {
    returnNormally(eval(car(cdr(argl)), env));
  }
  else if (eq_(condition, fAtom)) {
    returnNormally(eval(car(cdr(cdr(argl))), env));
  }
  else {
    exception("condition is neither #t nor #f");
  }
}

Atom* priCons(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  return cons(eval(car(argl), env), 
              eval(car(cdr(argl)), env));
}

Atom* priCar(Atom* argl, Atom* env) {
  checkLength(argl, 1);
  return car(eval(car(argl), env));
}

Atom* priCdr(Atom* argl, Atom* env) {
  checkLength(argl, 1);
  return cdr(eval(car(argl), env));
}

/* 
  (cons-stream <a> <b>) 
->(cons <a> (lambda () <b>))
 */

Atom* priConsStream(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* x = eval(car(argl), env);
  Atom* y = priLambda(cons(nil, cons(car(cdr(argl)), nil)), env);
  return cons(x, y);
}

Atom* priQuote(Atom* argl, Atom* env) {
  checkLength(argl, 1);
  return car(argl);
}

Atom* priEq_(Atom* argl, Atom* env) {
  checkLength(argl, 2);
  Atom* value = evalSequence(argl, env);
  
  Atom* a = car(value);
  Atom* b = car(cdr(value));
  if (number_(a) && number_(b)) {
    if (numbereq_(a->data.number, b->data.number)) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
  else if (null_(a) && null_(b)) {
    return tAtom;
  }
  else {
    if (a == b) {
      return tAtom;
    }
    else {
      return fAtom;
    }
  }
}

void printAtomList(FILE* file, Atom* atom, int depth) {
  fprintf(file, "(");
  while (1) {
    if (null_(cdr(atom))) {
      printAtom(file, car(atom), depth);
      fprintf(file, ")");
      return;
    }
    else if (pair_(cdr(atom))) {
      printAtom(file, car(atom), depth);
      fprintf(file, " ");
      atom = cdr(atom);
    }
    else {
      printAtom(file, car(atom), depth);
      fprintf(file, " . ");
      printAtom(file, cdr(atom), depth);
      fprintf(file, ")");
      return;
    }
  }
}

void printAtom(FILE* file, Atom* atom, int depth) {
  if (number_(atom)) {
    printNumber(file, atom->data.number);
  }
  else if (symbol_(atom)) {
    if (null_(atom)) {
      fprintf(file, "()");
    }
    else {
      printSymbol(file, atom->data.symbol);
    }
  }
  else if (pair_(atom)) {
    if (depth == 0) {
      fprintf(file, "(...)");
    }
    else {
      printAtomList(file, atom, depth - 1);
    }
  }
  else {
    fprintf(file, "not support now");
  }
}

void installEvalPackage() {
  symbolLambda = createReservedSymbol("lambda_tag");
  symbolEnvironment = createReservedSymbol("environment_tag");
  env0 = newEnvironemnt();
  
  primitiveCount = 0;
  appendPrimitive("+", priAdd);
  appendPrimitive("=", priEqual);
  appendPrimitive("<", priLT);
  appendPrimitive(">", priGT);
  appendPrimitive("<=", priLE);
  appendPrimitive(">=", priGE);
  appendPrimitive("define", priDefine);
  appendPrimitive("lambda", priLambda);
  appendPrimitive("if", priIf);
  appendPrimitive("cons", priCons);
  appendPrimitive("car", priCar);
  appendPrimitive("cdr", priCdr);
  appendPrimitive("cons-stream", priConsStream);
  appendPrimitive("quote", priQuote);
  appendPrimitive("eq?", priEq_);
}

#undef returnNormally