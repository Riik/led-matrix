#ifndef PABORT_H
#define PARBORT_H

#include <stdnoreturn.h>

#define pabort(s) pabort_complete(__FILE__, __LINE__, s, errno)

noreturn void pabort_complete(const char *filename, const size_t linenum, const char *msg, const int err);

#endif //PABORT_H
