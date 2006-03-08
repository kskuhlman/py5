
/* Support for dynamic loading of extension modules for OS390 / ZOS IBM platforms */

/* undef or def this to unset / set debug tracing mode */
#define DLL_DEBUG

#include <errno.h >

#if defined(__MVS__)
#include <dll.h>
#endif

#include "Python.h"
#include "importdl.h"


#define FUNCNAME_PATTERN "init%s"

/*
   use '.dll' suffix naming rules for dll naming on MVS
 */
const struct filedescr _PyImport_DynLoadFiletab[] = {
	{".dll", "rb", C_EXTENSION},
	{"module.dll", "rb", C_EXTENSION},
	{0, 0}
};

dl_funcptr _PyImport_GetDynLoadFunc(const char *fqname, const char *shortname,
				    const char *pathname, FILE *fp)
{
dl_funcptr p = NULL ;
char FuncName[258] ;
int SaveFlag = Py_VerboseFlag ; /* backup */


#if defined(__MVS__)

dllhandle * pLib    ;

#ifdef DLL_DEBUG
  Py_VerboseFlag = 1 ;
#endif

  if ( Py_VerboseFlag )
    printf("dllload %s \n" , pathname ) ;

  pLib = dllload( pathname ) ;

  if ( pLib == NULL )
  {
  char errMsg[256] ;
    PyOS_snprintf( errMsg , sizeof(errMsg) , " fail to load %s errno = %i " , pathname , errno ) ;
    	
    if ( Py_VerboseFlag )
      perror(errMsg) ;
    Py_VerboseFlag = SaveFlag ; /* restore */
    return NULL ;  	
  }

  PyOS_snprintf( FuncName , sizeof(FuncName) , FUNCNAME_PATTERN , shortname ) ;

  if ( Py_VerboseFlag )
    printf( "dllqueryfn %s\n" , FuncName ) ;

  p = dllqueryfn( pLib , FuncName ) ;

  if ( p == NULL )
  {
  char errMsg[256] ;
    PyOS_snprintf( errMsg , sizeof(errMsg) , " fail to get function %s errno = %i " , FuncName , errno ) ;
    dllfree( pLib ) ;
    	
    if ( Py_VerboseFlag )
      perror(errMsg) ;
    Py_VerboseFlag = SaveFlag ; /* restore */
    return NULL ;  	
  }

  Py_VerboseFlag = SaveFlag ; /* restore */
#endif

 return p  ;
}
