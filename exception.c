#include "exception.h"

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>

static jmp_buf return_point;
static const char* error_stack[ERROR_STACK_SIZE];
static int stack_top;

void exception(const char *format,...) {
  fprintf(stderr, "Exception raised: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");

  longjmp(return_point, 1);
}

int resetErrorStack() {
  stack_top = 0;
  if (setjmp(return_point) == 0) {
    return 0;
  }
  else {
    while (stack_top > 0) {
      stack_top--;
      fprintf(stderr, "\t%s\n", error_stack[stack_top]);
    }
    return 1;
  }
}

void pushStackMessage(const char* s) {
  error_stack[stack_top] = s;
  stack_top++;
}

void popStackMessage() {
  stack_top--;
}