#include "eval.h"
#include "exception.h"
#include "primitive.h"

Atom* env0;
Atom* symbolLambda;

void installEvalPackage() {
  env0 = nil;
  symbolLambda = createReservedSymbol("lambda_tag");
}

int pair_(Atom* exp) {
  if (exp->type == atomPair) {
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

Atom* lookUp(Atom* symbol, Atom* env) {
  if (null_(env)) {
    exception("look up", "can not find symbol");
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

Atom* eval(Atom* exp, Atom* env) {
  if (pair_(exp)) {
    Atom* proc = eval(getProcedureFromExp(exp), env);
    Atom* argl = getArgumentsFromExp(exp);
    return apply(proc, argl, env);
  }
  else if (null_(exp)) {
    exception("eval", "evaluate an empty expression");
    return nil;
  }
  else {
    if (symbol_(exp)) {
      return lookUp(exp, env);
    }
    else {
      return exp;
    }
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

Atom* apply(Atom* proc, Atom* argl, Atom* applyEnv) {
  if (lambda_(proc)) {
    Atom* newEnv = 
      extendEnvironment(
        getEnvironmentFromProc(proc),
        getArgumentsFromProc(proc),
        evalSequence(argl, applyEnv)
      );
    return eval(getBodyFromProc(proc), newEnv);
  }
  else if (symbol_(proc)) {
    Atom* (*pri)(Atom* argl, Atom* env);
    if (primitive_(proc->data.symbol, pri)) {
      return (*pri)(argl, applyEnv);
    }
    else {
      exception("apply", "proc is not compounded or primitive");
      return nil;
    }
  }
  else {
    exception("apply", "number / string / .. etc can not be applied");
    return nil;
  }
}

