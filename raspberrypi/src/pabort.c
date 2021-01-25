#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pabort.h"

noreturn void pabort_complete(const char *filename, const size_t linenum, const char* msg, const int err)
{
    fprintf(stderr, "%s:%zu %s. Errno: %d (%s)\n", filename, linenum, msg, err, strerror(err));
    abort();
}
