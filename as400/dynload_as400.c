/* This module provides the necessary stubs for when dynamic loading is
   not present. */

#include "Python.h"
#include "importdl.h"
#include "as400misc.h"
#include <qusec.h>


const struct filedescr _PyImport_DynLoadFiletab[] = {
	{0, 0}
};


dl_funcptr _PyImport_GetDynLoadFunc(const char *fqname, const char *shortname,
				    const char *pathname, FILE *fp)
{
	char funcname[258];
	int actmark;
	_OPENPTR func;

	sprintf(funcname, "init%.200s", shortname);
	actmark = getSrvpgm((char*)shortname, Py_GetProgramFullPath());
	if (actmark > 0) {
		func = getFunction(actmark, funcname);
		return func;
	}
	return NULL;
}
