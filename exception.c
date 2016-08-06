#include "exception.h"

#include <stdio.h>
#include <stdlib.h>

void exception(char* type, char* message) {
  fprintf(stderr, "Exception: %s. detail: %s\n", type, message);
}
