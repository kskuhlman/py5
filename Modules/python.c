/* Minimal main program -- everything is loaded from the library */

#include "Python.h"

#ifdef __ILEC400__
#include <locale.h>
#include <signal.h>
#include "as400misc.h"
#endif
#ifdef __FreeBSD__
#include <floatingpoint.h>
#endif

#ifdef __ILEC400__
void ile400SigHandler(int sig)
{
	_INTRPT_Hndlr_Parms_T excp_data;
	_GetExcData(&excp_data);
	PyErr_SetString(PyExc_RuntimeError, excp_data.Msg_Id);
}
#endif  
int
main(int argc, char **argv)
{
#ifdef __ILEC400__
	_HEAP_SIZE = 1048576; /* Set the fast heap to 1M */
	/*signal(SIGALL, &ile400SigHandler);*/
	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "C");
	/* program path */
	if (argc > 1) {
		int result;
		char *oldarg = argv[1];
		if (strncmp(argv[1], "./", 2) == 0) {		
			char path[256] = "";
            Py_SetProgramName(argv[0]);
			strcat(path, getpythonhome());
			strcat(path, "/site-packages");
			strcat(path, argv[1] + 1);
			argv[1] = path;
		}
		result = Py_Main(argc, argv);
		argv[1] = oldarg;
		return result;	   
	} else
#endif  
	/* 754 requires that FP exceptions run in "no stop" mode by default,
	 * and until C vendors implement C99's ways to control FP exceptions,
	 * Python requires non-stop mode.  Alas, some platforms enable FP
	 * exceptions by default.  Here we disable them.
	 */
#ifdef __FreeBSD__
	fp_except_t m;

	m = fpgetmask();
	fpsetmask(m & ~FP_X_OFL);
#endif
	return Py_Main(argc, argv);
}
