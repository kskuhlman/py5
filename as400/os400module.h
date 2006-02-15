#ifndef OS400MODULE_H
#define OS400MODULE_H
#include <as400misc.h>

typedef struct {
    char type;
    int  len;
    int  digits;
    int  dec;
    int  offset;
    void *ptr;
} parmtype_t;

typedef struct {
    PyObject_HEAD
    char  name[11];
    char  lib[11];
    int   parmCount;
    int   parmLen;
    parmtype_t  types[10];
    char  *parmbuf;
    _SYSPTR ptr;
} OS400Program;

typedef struct {
    PyObject_HEAD
    char  name[11];
    char  lib[11];
    _SYSPTR ptr;
    int   actmark;
} OS400Srvpgm;

typedef struct {
    PyObject_HEAD
    PyStringObject  *name;
    int   parmCount;
    int   parmLen;
    parmtype_t  retval;
    parmtype_t  types[10];
    char  *parmbuf;
    OS400Srvpgm *srvpgm;
    _OPENPTR ptr;
} OS400Proc;

typedef struct {
    PyObject_HEAD
    char  name[15];
    char  ccsid[6];
    iconv_t  cdencode;
    iconv_t  cddecode;
} OS400Codec;

extern DL_IMPORT(PyTypeObject) OS400Program_Type;
extern DL_IMPORT(PyTypeObject) OS400Srvpgm_Type;
extern DL_IMPORT(PyTypeObject) OS400Proc_Type;
extern DL_IMPORT(PyTypeObject) OS400Codec_Type;

#define OS400Program_Check(v)   ((v)->ob_type == &OS400Program_Type)
#define OS400Srvpgm_Check(v)    ((v)->ob_type == &OS400Srvpgm_Type)
#define OS400Proc_Check(v)  ((v)->ob_type == &OS400Proc_Type)
#define OS400Codec_Check(v)  ((v)->ob_type == &OS400Codec_Type)

#endif






