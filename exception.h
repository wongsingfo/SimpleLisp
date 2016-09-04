#ifndef EXCEPTION_H
#define EXCEPTION_H

#define ERROR_STACK_SIZE 256

void exception(const char *format,...);
int resetErrorStack();
void pushStackMessage(const char* s);
void popStackMessage();

#endif
