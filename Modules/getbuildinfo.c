#include "Python.h"

#ifndef DONT_HAVE_STDIO_H
#include <stdio.h>
#endif

#ifndef DATE
#ifdef __DATE__
#define DATE __DATE__
#else
#define DATE "xx/xx/xx"
#endif
#endif

#ifndef TIME
#ifdef __TIME__
#define TIME __TIME__
#else
#define TIME "xx:xx:xx"
#endif
#endif

#ifndef BUILD
#define BUILD 0
#endif

#ifdef __ILEC400__
#include "as400misc.h"
#endif


const char *
Py_GetBuildInfo(void)
{
	static char buildinfo[50];
#ifdef __ILEC400__  
    char *p = buildinfo + 1;
    strFromCp37("#", buildinfo);
    PyOS_snprintf(p, sizeof(buildinfo) - 1,
              "%d, %.20s, %.9s", BUILD, DATE, TIME);
#else
    PyOS_snprintf(buildinfo, sizeof(buildinfo),
              "#%d, %.20s, %.9s", BUILD, DATE, TIME);
#endif
    return buildinfo;
}
