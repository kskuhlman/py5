
/* Return the compiler identification, if possible. */

#include "Python.h"

#ifndef COMPILER

#ifdef __GNUC__
#define COMPILER "\n[GCC " __VERSION__ "]"
#endif

#endif /* !COMPILER */

#ifndef COMPILER

#ifdef __cplusplus
#define COMPILER "[C++]"
#else
#define COMPILER "[C]"
#endif

#endif /* !COMPILER */

#ifdef __ILEC400__
#include "as400misc.h"
static char compiler[7] = "";
#endif

const char *
Py_GetCompiler(void)
{
#ifdef __ILEC400__
    if (strlen(compiler) == 0) {
        strFromCp37("[ILEC]", compiler);
    }
    return compiler;
#else
    return COMPILER;
#endif
}
