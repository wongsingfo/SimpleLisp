#include "eval.h"
#include "exception.h"

Atom* env0;
Atom* symbolLambda;

#define numberOfPrimitiveProc 10

int primitiveCount;
Symbol primitiveProcSymbol[numberOfPrimitiveProc];
Atom* (*primitiveProc[numberOfPrimitiveProc])(Atom* argl, Atom** env);

void appendPrimitive(Symbol symbol, Atom* (*pri)(Atom* argl, Atom** env)) {
  if (primitiveCount >= numberOfPrimitiveProc) {
    exception("??", "primitiveCount >= numberOfPrimitiveProc");
    return;
  }
  else {
    primitiveProcSymbol[primitiveCount] = symbol;
    primitiveProc[primitiveCount] = pri;
    primitiveCount++;
    
    Atom* tmp = createSymbol(symbol);
    env0 = cons(cons(tmp, tmp), env0);
  }
}

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

Atom* evalSequence(Atom* exp, Atom* env) {
  if (null_(exp)) {
    return nil;
  }
  else {
    Atom* result = eval(car(exp), &env);
    return cons(result, evalSequence(cdr(exp), env));
  }
}

Atom* getProcedureFromExp(Atom* exp) {
  return car(exp);
}

Atom* getArgumentsFromExp(Atom* exp) {
  return cdr(exp);
}

Atom* lookUp(Atom* symbol, Atom* env) {
  if (null_(env)) {
    exception("look up", "can not find symbol");
    return nil;
  }
  else {
    Atom* pair = car(env);
    if (symboleq_(car(pair)->data.symbol, symbol->data.symbol)) {
      return cdr(pair);
    }
    else {
      return lookUp(symbol, cdr(env));
    }
  }
}

Atom* eval(Atom* exp, Atom** env) {
  if (pair_(exp)) {
    Atom* proc = eval(getProcedureFromExp(exp), env);
    Atom* argl = getArgumentsFromExp(exp);
    return apply(proc, argl, env);
  }
  else if (null_(exp)) {
    exception("eval", "evaluate an empty expression");
    return nil;
  }
  else if (symbol_(exp)) {
    return lookUp(exp, *env);
  }
  else if (number_(exp)) {
    return exp;
  }
  else {
    exception("??eval", "unknown operation");
    return nil;
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

Atom* getEnvironmentFromProc(Atom* proc) {
  return car(cdr(cdr(proc)));
}

Atom* getArgumentsFromProc(Atom* proc) {
  return car(cdr(proc));
}

Atom* getBodyFromProc(Atom* proc) {
  return cdr(cdr(cdr(proc)));
}

Atom* extendEnvironment(Atom* env, Atom* name, Atom* argl) {
  if (null_(name) && null_(argl)) {
    return env;
  }
  else if (null_(name)) {
    exception("extend environment", "too many arguments");
    return env;
  }
  else if (null_(argl)) {
    exception("extend environment", "too few arguments");
    return env;
  }
  else {
    return extendEnvironment(
      cons(cons(car(name), car(argl)), env),
      cdr(name),
      cdr(argl)
    );
  }
}

Atom* primitiveProcApply(Symbol proc, Atom* argl, Atom** env) {
  int i;
  for (i = 0; i < primitiveCount; i++) {
    if (symboleq_(proc, primitiveProcSymbol[i])) {
      return (*primitiveProc[i])(argl, env);
    }
  }
  exception("primitive apply", "proc is not primitive or compounded");
  return nil;
}

Atom* apply(Atom* proc, Atom* argl, Atom** applyEnv) {
  if (lambda_(proc)) {
    Atom* newEnv = 
      extendEnvironment(
        getEnvironmentFromProc(proc),
        getArgumentsFromProc(proc),
        evalSequence(argl, *applyEnv)
      );
    return eval(getBodyFromProc(proc), &newEnv);
  }
  else if (symbol_(proc)) {
    return primitiveProcApply(proc->data.symbol, argl, applyEnv);
  }
  else {
    exception("apply", "number / string / .. etc can not be applied");
    return nil;
  }
}

void checkLength(Atom* list, int len) {
  if (null_(list)) {
    if (len != 0) {
      exception("check list length", "list too short");
    }
    else {
      return;
    }
  }
  else if (len == 0) {
    if (! null_(list)) {
      exception("check list length", "list too long");
    }
    else {
      return;
    }
  }
  else {
    checkLength(cdr(list), len - 1);
  }
}

Atom* priAdd(Atom* argl, Atom** env) {
  Atom* value = evalSequence(argl, *env);
  
  if (null_(value)) {
    exception("primitive + (add)", "no arguments");
    return nil;
  }
  else {
    Atom* i;
    for (i = value; !null_(i); i = cdr(i)) {
      if (! number_(car(i))) {
        exception("primitive + (add)", "not a number");
        return nil;
      }
    }
  
    Number result = car(value)->data.number;
    for (i = cdr(value); !null_(i); i = cdr(i)) {
      result = numberadd(result, car(i)->data.number);
    }
    return createNumber(result);
  }
}

Atom* priDefine(Atom* argl, Atom** env) {
  checkLength(argl, 2);
  Atom* value = eval(car(cdr(argl)), env);
  
  if (symbol_(car(argl))) {
    Atom* def = cons(car(argl), value);
    *env = cons(def, *env);
    return nil;
  }
  else {
    exception("primitive define", "symbol expected");
    return nil;
  }
}

Atom* priLambda(Atom* argl, Atom** env) {
  checkLength(argl, 2);
  Atom* result = cons(*env, car(cdr(argl)));
  result = cons(car(argl), result);
  result = cons(symbolLambda, result);
  return result;
}

void printAtom(FILE* file, Atom* atom) {
  if (number_(atom)) {
    printNumber(file, atom->data.number);
  }
  else if (symbol_(atom)) {
    printSymbol(file, atom->data.symbol);
  }
  else {
    fprintf(file, "not support now");
  }
}

void installEvalPackage() {
  env0 = nil;
  symbolLambda = createReservedSymbol("lambda_tag");
  
  primitiveCount = 0;
  appendPrimitive(createSymbolFromStr("+"), priAdd);
  appendPrimitive(createSymbolFromStr("define"), priDefine);
  appendPrimitive(createSymbolFromStr("lambda"), priLambda);
}
