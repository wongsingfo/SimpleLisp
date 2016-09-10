#include "atom.h"
#include <stdlib.h>

static Atom* memory_pool = NULL;
static Atom* memory_pool_tail = NULL;
static Atom* cur_memory = NULL;
static Atom** queue = NULL;

void installAtomPackage() {
  symbol_ok = createSymbolFromStr("ok");
  symbol_ok->ref = 1;
  symbol_error = createSymbolFromStr("error");
  symbol_error->ref = 1;
  boolean_true = createBollean(1);
  boolean_true->ref = 1;
  boolean_false = createBollean(0);
  boolean_false->ref = 1;
}

static void GC(Atom* x) {
  #ifndef gcForward
  #define gcForward(x) tmp = x; \
    if (tmp->ref == -2) {\
      tmp->ref = 0;\
      *rear = tmp;\
      rear++;\
    }
  #else 
    #error gcForward() has been defined.
  #endif

  Atom** front = queue;
  Atom** rear = queue;
  Atom* tmp;

  *rear = x;
  rear++;

  while (front < rear) {
    x = *front;
    front++;
    if (x->tag == PAIR) {
      gcForward(x->data.child[0]);
      gcForward(x->data.child[1]);
    }
    else if (x->tag == COMPOUND) {
      gcForward(x->data.compound.body);
      gcForward(x->data.compound.params);
      gcForward(x->data.compound.env);
    }
  }
  #undef gcForward
}

static void gcFree(Atom* x) {
  x->ref = -1;
  if (x->tag == NUMBER) {
    deleteNumber(x->data.number);
  }
}

static void garbageCollection() {
  for (Atom* i = memory_pool; i < memory_pool_tail; i++) {
    if (i->ref == 0) {
      i->ref = -2;
    }
  }

  for (Atom* i = memory_pool; i < memory_pool_tail; i++) {
    if (i->ref > 0) {
      GC(i);
    }
  }

  for (Atom* i = memory_pool; i < memory_pool_tail; i++) {
    if (i->ref == -2) {
      gcFree(i);
    }
  }

  cur_memory = memory_pool;
}

static void extendMemoryTo(int size) {
  int offset_tail  = memory_pool_tail - memory_pool;
  int offset_cur   = cur_memory       - memory_pool;
  memory_pool = (Atom*) realloc(memory_pool, size * (sizeof (Atom)));
  if (memory_pool == NULL) {
    exception("extend memory: realloc return NULL");
  }
  queue = (Atom**) realloc(queue, size * (sizeof (Atom*)));
  memory_pool_tail = memory_pool      + offset_tail;
  cur_memory       = memory_pool      + offset_cur;
}

static Atom* newAtom() {
  if (cur_memory >= memory_pool_tail) {
    if (memory_pool == NULL) {
      extendMemoryTo(32);  // initial size 
    }
    else {
      garbageCollection();
      int memory_size = memory_pool_tail - memory_pool;
      if (cur_memory >= memory_pool + (memory_size / 2)) {
        extendMemoryTo(memory_size * 2);
      }
    }
  }

  while (cur_memory->ref > -1) {
    cur_memory++;
  }
  return cur_memory - 1;
}

Atom* car(Atom* x) {
  if (x->tag == PAIR) {
    return x->data.child[0];
  }
  else {
    exception("car: not a pair");
    return symbol_error;
  }
}

Atom* cdr(Atom* x) {
  if (x->tag == PAIR) {
    return x->data.child[1];
  }
  else {
    exception("cdr: not a pair");
    return symbol_error;
  }
}

Atom* cons(Atom* x, Atom* y) {
  Atom* result = newAtom();
  result->tag = PAIR;
  result->data.child[0] = x;
  result->data.child[1] = y;
  return result;
}
