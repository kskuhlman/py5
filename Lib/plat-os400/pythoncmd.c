/* Minimal main program -- everything is loaded from the library */

#include "Python.h"
#ifdef __ILEC400__
#include <locale.h>
#include <signal.h>
#include "as400misc.h"
#endif

extern DL_EXPORT(int) Py_Main(int, char **);

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
	short noOfP = 0;
	int proglen = 100;
	int parmlen = 256;
	char *s, path[256] = "";
	char *newarg[20];
	_HEAP_SIZE = 1048576; /* Set the fast heap to 1M */
	/*signal(SIGALL, &ile400SigHandler);*/
	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "C");
	/* first parameter is always program name */
	newarg[0] = argv[0];	
	/* program path */
	if (argc > 1) {
		newarg[1] = argv[1]; 
		s = newarg[1] + proglen - 1;
		while (s >= newarg[1] && isspace(Py_CHARMASK(*s)))
			s--;
		s++;
		*s = '\0';		
		/* set default program path */
		if (strncmp(newarg[1], "./", 2) == 0) {		
            Py_SetProgramName(argv[0]);
			s = strcat(path, getpythonhome());
			s = strcat(path, "/site-packages");
			s = strcat(s, newarg[1] + 1);
			newarg[1] = s;
		}
	}
	if (strlen(newarg[1]) == 0) {
		argc = 1;
	} else {
		/* parameters */
		if (argc > 2) {
			int i;
			memcpy(&noOfP, argv[2], 2);
			s = argv[2] + 2;
			for (i = 2; i < noOfP + 2; i++) {
				newarg[i] = s;
				s += parmlen - 1;
				while (s >= newarg[i] && isspace(Py_CHARMASK(*s)))
					s--;
				s++;
				*s = '\0';
				s = newarg[i] + parmlen;
			}
		}
		argc += noOfP - 1;
	}
	return Py_Main(argc, newarg);
#else
	return Py_Main(argc, argv);
#endif  
}


