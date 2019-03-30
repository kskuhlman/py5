/* Fudge unix isatty for OS400 */
/* Per had a more interesting version coded into "pythonrun.c" */
/* I've included it here for future reference, should my simplistic method */
/*  not work out. */

#include "isatty.h"

int isatty(int fn)
{ return (fn==fileno(stdin));
}

//#ifdef __ILEC400__
//#include <qusrjobi.h>
// #endif

#ifdef __ILEC400__
//int
//isatty(int fd)
//{
//    char buffer[100];
//    QUSRJOBI(buffer, 100, "JOBI0100", "*                         ",
//              "                ");
//    return (buffer[60] == 'I' && fd == fileno(stdin));
//}
#endif
