/*
 * os400module
 * 
 *--------------------------------------------------------------------
 * Copyright (c) 2001 by Per Gummedal.
 *
 * p.g@figu.no
 * 
 * By obtaining, using, and/or copying this software and/or its
 * associated documentation, you agree that you have read, understood,
 * and will comply with the following terms and conditions:
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * associated documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appears in all
 * copies, and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of FIGU DATA AS
 * or the author not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.
 * 
 * FIGU DATA AS AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS.  IN NO EVENT SHALL FIGU DATA AS OR THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *------------------------------------------------------------------------
 *
 *  !!!  Warning !!!
 * 
 * The number of parameters is undefined when called from C.
 * So do not use %parm, better to send all parameters and test for values.
 *
 * Return value can only be a pointer or string.
 * 
 * Pointer arguments in ILERPG should always be coded like this:
 *    D   xPtr                          *   value
 *
 *--------------------------------------------------------------------*/
#define ASTEC  /* special for ASTEC Helicopeter Services */

#include "Python.h"
#include <xxcvt.h>
#include <xxdtaa.h>
#include <signal.h>
#include <miptrnam.h>
#include <qusec.h>
#include <qwcrdtaa.h>
#include <qleawi.h>
#include <iconv.h>
#include "os400module.h"
#include "as400misc.h"

#ifdef ASTEC
#include <decimal.h>
/* array structure */
typedef struct {
    decimal(5,0) wElemSize;
    decimal(7,0) wCapacity;
    decimal(7,0) wElements;
    decimal(5,0) wKeySize;
    decimal(7,0) wCurPos;
} ArrayType;
/* not included in cobject.h, copied from cobject.c */
typedef struct {
    PyObject_HEAD
    void *cobject;
    void *desc;
    void (*destructor)(void *);
} PyCObject;
#endif

typedef void *(OS_func_0) (void);
typedef void *(OS_func_1) (void *p1);
typedef void *(OS_func_2) (void *p1, void *p2);
typedef void *(OS_func_3) (void *p1, void *p2, void *p3);
typedef void *(OS_func_4) (void *p1, void *p2, void *p3, void *p4);
typedef void *(OS_func_5) (void *p1, void *p2, void *p3, void *p4, void *p5);
typedef void *(OS_func_6) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6);
typedef void *(OS_func_7) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6, void *p7);
typedef void *(OS_func_8) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6, void *p7, void *p8);
typedef void *(OS_func_9) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6, void *p7, void *p8, void *p9);
typedef void *(OS_func_10) (void *p1, void *p2, void *p3, void *p4, void *p5,
                            void *p6, void *p7, void *p8, void *p9, void *p10);

typedef void (OS_pgm_0) (void);
typedef void (OS_pgm_1) (void *p1);
typedef void (OS_pgm_2) (void *p1, void *p2);
typedef void (OS_pgm_3) (void *p1, void *p2, void *p3);
typedef void (OS_pgm_4) (void *p1, void *p2, void *p3, void *p4);
typedef void (OS_pgm_5) (void *p1, void *p2, void *p3, void *p4, void *p5);
typedef void (OS_pgm_6) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6);
typedef void (OS_pgm_7) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6, void *p7);
typedef void (OS_pgm_8) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6, void *p7, void *p8);
typedef void (OS_pgm_9) (void *p1, void *p2, void *p3, void *p4, void *p5,
                           void *p6, void *p7, void *p8, void *p9);
typedef void (OS_pgm_10) (void *p1, void *p2, void *p3, void *p4, void *p5,
                            void *p6, void *p7, void *p8, void *p9, void *p10);

#pragma linkage(OS_pgm_0, OS)
#pragma linkage(OS_pgm_1, OS)
#pragma linkage(OS_pgm_2, OS)
#pragma linkage(OS_pgm_3, OS)
#pragma linkage(OS_pgm_4, OS)
#pragma linkage(OS_pgm_5, OS)
#pragma linkage(OS_pgm_6, OS)
#pragma linkage(OS_pgm_7, OS)
#pragma linkage(OS_pgm_8, OS)
#pragma linkage(OS_pgm_9, OS)
#pragma linkage(OS_pgm_10, OS)
/*#pragma argument(OS_func_0, OS)
#pragma argument(OS_func_1, OS)
#pragma argument(OS_func_2, OS)
#pragma argument(OS_func_3, OS)
#pragma argument(OS_func_4, OS)
#pragma argument(OS_func_5, OS)
#pragma argument(OS_func_6, OS)
#pragma argument(OS_func_7, OS)
#pragma argument(OS_func_8, OS)
#pragma argument(OS_func_9, OS)
#pragma argument(OS_func_10, OS)*/

static PyObject *os400Error;

/* get length of as400 string */    
static int
f_strlen(char *s, int len)
{
    len--;
    while (len >= 0 && isspace(Py_CHARMASK(s[len])))
        len--;
    len++;
    return len;    
}

static PyObject *
f_cvtToPy(unsigned char *p, char type, int len,
          int digits, int dec)
{
    short dsh;
    long dl;
    long long dll;
    float dfl;
    double ddbl;
    /* binary */
    if (type == 'i') {
        if (len == 2) {
            memcpy(&dsh, p, 2);
            return PyInt_FromLong(dsh);
        } else if (len == 4) {
            memcpy(&dl, p, 4);
            return PyInt_FromLong(dl);
        } else if (len == 8) {
            memcpy(&dll, p, 8);
            return PyLong_FromLong(dll);
        } else {
            PyErr_SetString(os400Error, "Data type error.");
            return NULL;
        }
    /* float */
    } else if (type == 'f') {
        if (len == 4) {
            memcpy(&dfl, p, 4);
            return PyFloat_FromDouble((double)dfl);
        }
        else if (len == 8) {
            memcpy(&ddbl, p, 8);
            return PyFloat_FromDouble((double)ddbl);
        } else {
            PyErr_SetString(os400Error, "Data type error.");
            return NULL;
        }
    /* zoned */
    } else if (type == 'z') {
        if (dec == 0 && digits < 10) 
            return PyInt_FromLong(QXXZTOI(p, digits, dec));
        else {
            char buf[100], *ss;
            zonedtostr((char *)buf, p, digits, dec, '.');  
            if (dec == 0)
                return PyLong_FromLong(strtoll((char *)buf, &ss, 10));
            else
                return PyFloat_FromDouble(strtod((char *)buf, &ss));
        }
    /* packed */
    } else if (type == 'd') {
        if (dec == 0 && digits < 10) 
            return PyInt_FromLong(QXXPTOI(p, digits, dec));
        else {
            char buf[100], *ss;
            packedtostr((char *)buf, p, digits, dec, '.');  
            if (dec == 0)
                return PyLong_FromLong(strtoll((char *)buf, &ss, 10));
            else
                return PyFloat_FromDouble(strtod((char *)buf, &ss));
        }
    /* char */
    } else if (type == 'c') {
        return PyString_FromStringAndSize(p, f_strlen(p, len));
    } else {
        PyErr_SetString(os400Error, "Data type not valid.");
        return NULL;
    }   
}

/* convert from Python format */    
static int
f_cvtFromPy(unsigned char *p, char type, int len,
            int digits, int dec, PyObject *o)
{
    short dsh;
    long dlo;
    long long dll;
    float dfl;
    double ddbl;
    char *c;
    if (type == 'i') {
        /* binary */
        if (PyInt_Check(o)) {
            if (len == 2) {
                dsh = PyInt_AS_LONG(o);
                memcpy(p, &dsh, len);
                return 0;
            } else if (len == 4) {
                dlo = PyInt_AS_LONG(o);
                memcpy(p, &dlo, len);
                return 0;
            } else if (len == 8) {
                dll = PyInt_AS_LONG(o);
                memcpy(p, &dll, len);
                return 0;
            }
        } else if (PyLong_Check(o)) {
            if (len == 2) {
                dsh = PyLong_AsLong(o);
                memcpy(p, &dsh, len);
                return 0;
            } else if (len == 4) {
                dlo = PyLong_AsLong(o);
                memcpy(p, &dlo, len);
                return 0;
            } else if (len == 8) {
                dll = PyLong_AsLong(o);
                memcpy(p, &dll, len);
                return 0;
            }
        }
        PyErr_SetString(os400Error, "Data conversion error.");
        return -1;
    /* float */
    } else if (type == 'f') {
        if (len == 4) {
            if (PyInt_Check(o))
                dfl = PyInt_AS_LONG(o);
            else if (PyLong_Check(o))
                dfl = PyLong_AsLong(o);
            else if (PyFloat_Check(o))
                dfl = PyFloat_AS_DOUBLE(o);
            else {
                PyErr_SetString(os400Error, "Data conversion error.");
                return -1;
            }
            memcpy(p, &dfl, len);
            return 0;
        } else {
            if (PyInt_Check(o))
                ddbl = PyInt_AS_LONG(o);
            else if (PyLong_Check(o))
                ddbl = PyLong_AsLong(o);
            else if (PyFloat_Check(o))
                ddbl = PyFloat_AS_DOUBLE(o);
            else {
                PyErr_SetString(os400Error, "Data conversion error.");
                return -1;
            }
            memcpy(p, &ddbl, len);
            return 0;
        }
    /* zoned */
    } else if (type == 'z') {
        if (PyInt_Check(o)) {
            QXXITOZ(p, digits, dec, PyInt_AS_LONG(o));
            return 0;
        } else if (PyLong_Check(o)) {
            if ((digits - dec) < 10)
                QXXITOZ(p, digits, dec, PyLong_AsLong(o));
            else
                QXXDTOZ(p, digits, dec, PyLong_AsDouble(o));
            return 0;
        } else if (PyFloat_Check(o)) {
            QXXDTOZ(p, digits, dec, PyFloat_AS_DOUBLE(o));
            return 0;
        }
        PyErr_SetString(os400Error, "Data conversion error.");
        return -1;
    /* packed */
    } else if (type == 'd') {
        if (PyInt_Check(o)) {
            QXXITOP(p, digits, dec, PyInt_AS_LONG(o));
            return 0;
        } else if (PyLong_Check(o)) {
            if ((digits - dec) < 10)
                QXXITOP(p, digits, dec, PyLong_AsLong(o));
            else
                QXXDTOP(p, digits, dec, PyLong_AsDouble(o));
            return 0;
        } else if (PyFloat_Check(o)) {
            QXXDTOP(p, digits, dec, PyFloat_AS_DOUBLE(o));
            return 0;
        }
        PyErr_SetString(os400Error, "Data conversion error.");
        return -1;
    /* char */
    } else if (type == 'c') {
        if (PyString_Check(o)) { 
            int i = PyString_GET_SIZE(o);
            c = PyString_AS_STRING(o);
            if (i >= len)
                memcpy(p, c, len);
            else {
                memcpy(p, c, i);
                p += i;
                memset(p, ' ', len - i);
            }
            return 0;
        }
        PyErr_SetString(os400Error, "Data conversion error.");
        return -1;
    } else {
        PyErr_SetString(os400Error, "Data type not supported.");
        return -1;
    }   
}

/* check parameters */  
static int
f_checkparm(parmtype_t *ptype, PyObject *parm) 
{
    PyObject *sub;

    if (!PyTuple_Check(parm)) {
        PyErr_SetString(os400Error, "Each definition must be a tuple.");
        return -1;
    }
    sub = PyTuple_GET_ITEM(parm, 0);
    if (!PyString_Check(sub)) {
        PyErr_SetString(os400Error, "Not a valid type.");
        return -1;          
    }
    ptype->type = *(PyString_AsString(sub));
    if (PyTuple_Size(parm) > 1) {
        /* digits / length */
        sub = PyTuple_GET_ITEM((PyTupleObject *)parm, 1);
        if (!PyInt_Check(sub)) {
            PyErr_SetString(os400Error, "Not a valid type.");
            return -1;          
        }
        ptype->len = PyInt_AS_LONG(sub);
        ptype->digits = ptype->len;
        /* decimal positions */
        if (PyTuple_Size(parm) > 2) {
            sub = PyTuple_GET_ITEM(parm, 2);
            if (!PyInt_Check(sub)) {
                PyErr_SetString(os400Error, "Not a valid type.");
                return -1;          
            }
            ptype->dec = PyInt_AS_LONG(sub);
        } else
            ptype->dec = 0;
        /* if packed decimal set correct length */
        switch (ptype->type) {
        case 'd':
            ptype->len = ptype->len / 2 + 1;
            break;
        case 'i': case 'f': case 'z': 
        case 'c':
            break;
        default:
            PyErr_SetString(os400Error, "Not a valid type.");
            return -1;          
        }
    } else if (ptype->type == 'p') {
        ptype->len = 0;
    } else {
        PyErr_SetString(os400Error, "Not a valid type.");
        return -1;                  
    }
    return 0;
}

/* put value into parameter buffer */   
static int
f_setparm(unsigned char *buf, parmtype_t *ptype, PyObject *v)
{
    _OPENPTR o;
    if (ptype->type == 'p') {
        o = PyCObject_AsVoidPtr(v);
        ptype->ptr = o;
        /*ptype->ptr = &o;*/
        return 0;
    } else {
        return f_cvtFromPy(buf + ptype->offset, ptype->type, ptype->len, 
                           ptype->digits, ptype->dec, v);
    }
}

/* get value from parameter buffer */   
static PyObject *
f_getparm(unsigned char *buf, parmtype_t *ptype) 
{
    if (ptype->type == 'p') {
        return PyCObject_FromVoidPtr(ptype->ptr, NULL);
    } else {
        return f_cvtToPy(buf + ptype->offset, ptype->type, ptype->len, 
                         ptype->digits, ptype->dec);
    }
}

static void
f_srvpgm_activate(OS400Srvpgm *self)
{
    if (self->ptr == NULL) {
        self->ptr = rslvsp(WLI_SRVPGM, self->name, self->lib, _AUTH_OBJ_MGMT);
        self->actmark = QleActBndPgm(&(self->ptr), NULL, NULL, NULL, NULL);
    }
}

/* convert form one ccsid to another */
static PyObject *
f_convert(iconv_t cd, char *in, int size)
{
    size_t insize, outsize, newsize;
    int pos;
    char *out;
    PyObject *outobj;
    insize = outsize = newsize = size;
    outobj = PyString_FromStringAndSize(NULL, newsize);
    if (outobj == NULL) return NULL;
    out = PyString_AsString(outobj);
    iconv(cd, &in, &insize, &out, &outsize);
    while (insize > 0) {
        pos = newsize - outsize;
        newsize += 10 + outsize * 2;
        _PyString_Resize(&outobj, newsize);
        if (outobj == NULL) return NULL;
        out = PyString_AsString(outobj) + pos;
        outsize = newsize - pos;
        iconv(cd, &in, &insize, &out, &outsize);
        /* if outsize still is same size there is a bug */
        if (outsize == newsize - pos) {
            PyErr_SetString(os400Error, "Error in code conversion.");
            return NULL;
        }
    }
    if (outsize > 0)
        _PyString_Resize(&outobj, newsize - outsize);
    return outobj;
}

/* ---------------------------------------------------------------------*/
/* OS400Program methods  */

static void
OS400Program_dealloc(OS400Program *self)
{
    if (self->parmbuf) 
        PyMem_Free(self->parmbuf);
    PyObject_Del(self);
}

static PyObject *
OS400Program_call(OS400Program *self, PyObject *args, PyObject *keyws)
{
    PyObject *parms = Py_None, *k, *v;
    int i, parmCount = self->parmCount;
    volatile _INTRPT_Hndlr_Parms_T  excpData;

#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
/*  if (!PyArg_ParseTuple(args, "|O:call", &parms))
        return NULL;    */
    /* resolve program object */
    if (self->ptr == NULL)
        self->ptr = rslvsp(_Program, self->name, self->lib, _AUTH_OBJ_MGMT);
    /* convert values */
/*  if (parms != Py_None) {
        if (!PyTuple_Check(parms) && !PyList_Check(parms)) {
            PyErr_SetString(os400Error, "Key must be a tuple or list with values.");
            return NULL;
        } */
    parmCount = PyTuple_Size(args);
    if (parmCount > self->parmCount) {
        PyErr_SetString(os400Error, "Too many parameters.");
        return NULL;
    }
    for (i = 0; i < parmCount; i++) {
        v = PyTuple_GetItem(args, i);
        if (f_setparm(self->parmbuf, &(self->types[i]), v))
            return NULL;
    }       
    /* call the program */
    switch (parmCount) {
    case 0:     
        ((OS_pgm_0 *)self->ptr)();
        break;
    case 1:     
        ((OS_pgm_1 *)self->ptr)(self->types[0].ptr);
        break;
    case 2:     
        ((OS_pgm_2 *)self->ptr)(self->types[0].ptr, self->types[1].ptr);
        break;
    case 3:     
        ((OS_pgm_3 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr);
        break;
    case 4:     
        ((OS_pgm_4 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr, self->types[3].ptr);
        break;
    case 5:     
        ((OS_pgm_5 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr, self->types[3].ptr,
                                 self->types[4].ptr);
        break;
    case 6:     
        ((OS_pgm_6 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr, self->types[3].ptr,
                                 self->types[4].ptr, self->types[5].ptr);
        break;
    case 7:     
        ((OS_pgm_7 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr, self->types[3].ptr,
                                 self->types[4].ptr, self->types[5].ptr,
                                 self->types[6].ptr);
        break;
    case 8:     
        ((OS_pgm_8 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr, self->types[3].ptr,
                                 self->types[4].ptr, self->types[5].ptr,
                                 self->types[6].ptr, self->types[7].ptr);
        break;
    case 9:     
        ((OS_pgm_9 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                 self->types[2].ptr, self->types[3].ptr,
                                 self->types[4].ptr, self->types[5].ptr,
                                 self->types[6].ptr, self->types[7].ptr,
                                 self->types[8].ptr);
        break;
    case 10:        
        ((OS_pgm_10 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                  self->types[2].ptr, self->types[3].ptr,
                                  self->types[4].ptr, self->types[5].ptr,
                                  self->types[6].ptr, self->types[7].ptr,
                                  self->types[8].ptr, self->types[9].ptr);
        break;
    }
    Py_INCREF(Py_None);
    return Py_None;
EXCP1:
    PyErr_SetString(os400Error, "Error calling program.");
    return NULL;    
}

static char program_resolve_doc[] =
"pgm.resolve() -> None.\n\
\n\
Do a new resolve.";

static PyObject *
OS400Program_resolve(OS400Program *self, PyObject *args)
{
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    if (!PyArg_ParseTuple(args, ":resolve"))
        return NULL;    
    self->ptr = rslvsp(_Program, self->name, self->lib, _AUTH_OBJ_MGMT);
EXCP1:
    PyErr_SetString(os400Error, "Error trying to resolve to the program.");
    return NULL;    
}

static PyMethodDef OS400Program_methods[] = {
    {"resolve", (PyCFunction)OS400Program_resolve, METH_VARARGS , program_resolve_doc},
    {NULL,      NULL}       /* sentinel */
};

static int
OS400Program_length(OS400Program *self)
{
    return self->parmCount;
}

static int
OS400Program_ass_subscript(OS400Program *self, PyObject *v, PyObject *w)
{
    int pos;
    if (w == NULL) {
        PyErr_SetString(os400Error, "NULL value not supported.");
        return -1;
    }
    if (!PyInt_Check(v)) {
        PyErr_SetString(os400Error, "Argument must be a number.");
        return -1;
    }
    pos = PyInt_AS_LONG(v);
    if (pos >= self->parmCount) {
        PyErr_SetString(os400Error, "Not so many parameters.");
        return -1;
    }
    return f_setparm(self->parmbuf, &(self->types[pos]), w);
}

static PyObject *
OS400Program_subscript(OS400Program *self, PyObject *v)
{
    int pos;
    if (!PyInt_Check(v)) {
        PyErr_SetString(os400Error, "Argument must be a number.");
        return NULL;
    }
    pos = PyInt_AS_LONG(v);
    if (pos >= self->parmCount) {
        PyErr_SetString(os400Error, "Not so many parameters.");
        return NULL;
    }
    return f_getparm(self->parmbuf, &(self->types[pos]));
}

static PyMappingMethods OS400Program_as_mapping = {
    (inquiry)OS400Program_length, /*mp_length*/
    (binaryfunc)OS400Program_subscript, /*mp_subscript*/
    (objobjargproc)OS400Program_ass_subscript, /*mp_ass_subscript*/
};

static PyObject *
OS400Program_getattr(OS400Program *self, char *name)
{
    return Py_FindMethod(OS400Program_methods, (PyObject *)self, name);
}

PyTypeObject OS400Program_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyObject_HEAD_INIT(NULL)
    0,          /*ob_size*/
    "OS400Program",         /*tp_name*/
    sizeof(OS400Program),   /*tp_basicsize*/
    0,          /*tp_itemsize*/
    /* methods */
    (destructor)OS400Program_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    (getattrfunc)OS400Program_getattr, /*tp_getattr*/
    0,          /*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    &OS400Program_as_mapping,   /*tp_as_mapping*/
    0,          /*tp_hash*/
   (ternaryfunc)OS400Program_call, /*tp_call*/
    0,          /*tp_str*/
};

/* --------------------------------------------------------------------- */
/* OS400Srvpgm methods  */

static void
OS400Srvpgm_dealloc(OS400Srvpgm *self)
{
    PyObject_Del(self);
}

static char srvpgm_proc_doc[] =
"s.proc(name, return value, parameters) -> Procedure object.\n\
\n\
Returns a procedure object(function).\n\
Name should be the name of the service program (uppercase).\n\
Return value should be a tuple that describes the return value.\n\ 
Parmeters should be a tuple of parameter descriptions.\n\
First item in the parameter tuple is the return value.\n\
There is a limit of 10 parameters not including the return value.\n\
Each parameter description shold be a tuple with the following fields:\n\
  Type(string), length(int), decimals(int, optional)\n\
  The following Types are valid:\n\
    'c' String. ('c',20)\n\
    'i' Binary(Integer). ('i', 4) Length is here number of bytes (4/8).\n\
    'f' Float. ('f', 4) Length is here number of bytes (4/8).\n\
    'd' packed decimal ('d',5,0)\n\
    'z' Zoned decimal ('z',5,0)\n\
    'p' Pointer ('p',) (no length and decimals)\n\
The Procedure object returned should be used as a normal function:\n\
But You can also set the parameters by indexing as shown here,\n\
 (proc1[0] = 'TEST')\n\
To see changed parameter values after the call use indexing. \n\
 (print proc1[0])\n\
Here is an example:\n\
  s1 = os400.Srvpgm('ITEM','*LIBL')\n\
  proc1 = s1.proc('getDesc',('c',30),(('c',10)))\n\
  desc = proc1('PART_A')";

static PyObject *
OS400Srvpgm_proc(OS400Srvpgm *self, PyObject *args)
{
    int i;
    PyObject *name, *retval = Py_None, *parm = Py_None, *item, *sub;
    OS400Proc *proc;

    if (!PyArg_ParseTuple(args, "SOO:Program", &name, &retval, &parm))
        return NULL;
    if (parm != Py_None && !PyTuple_Check(parm)) {
        PyErr_SetString(os400Error, "Parmaters must be a tuple.");
        return NULL;
    }
    proc = PyObject_New(OS400Proc, &OS400Proc_Type);
    if (proc == NULL)
        return NULL;
    Py_INCREF(name);
    proc->name = (PyStringObject *)name;
    Py_INCREF(self);
    proc->srvpgm = self;
    proc->ptr = NULL;
    proc->parmbuf = NULL;       
    proc->parmLen = 0;
    proc->parmCount = 0;
    Py_INCREF(parm);
    Py_INCREF(retval);
    /* return value */
    if (retval != Py_None) {
        proc->retval.offset = 0;
        if (f_checkparm(&(proc->retval), retval)) { 
            Py_DECREF(proc);
            return NULL;
        }       
    } else {
        proc->retval.type = ' ';
        proc->retval.len = 0;
    }
    /* parameters */
    if (parm != Py_None) {
        proc->parmCount = PyTuple_Size(parm);
        if (proc->parmCount > 10) {
            PyErr_SetString(os400Error, "Maximum number of parameters is 10.");
            Py_DECREF(proc);
            return NULL;
        }   
        /* check parameters and get size */
        for (i = 0; i < proc->parmCount; i++) {
            /* get parameter type */
            if (f_checkparm(&(proc->types[i]), 
                                 PyTuple_GET_ITEM((PyTupleObject *)parm, i))) {
                Py_DECREF(proc);
                return NULL;
            } else {
                proc->types[i].offset = proc->parmLen;
                proc->parmLen += proc->types[i].len;
            }
        }
    }
    /* allcoate parameter area and set pointers */
    if (proc->parmLen > 0) {
        proc->parmbuf = PyMem_Malloc(proc->parmLen + 100);
        for (i = 0; i < proc->parmCount; i++) {
            if (proc->types[i].type != 'p')
                proc->types[i].ptr = proc->parmbuf + proc->types[i].offset;
        }
    }
    Py_DECREF(parm);
    Py_DECREF(retval);
    return (PyObject *) proc;
}

static char srvpgm_resolve_doc[] =
"srvpgm.resolve() -> None.\n\
\n\
Resolve and activate the service program.";

static PyObject *
OS400Srvpgm_resolve(OS400Srvpgm *self, PyObject *args)
{
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    if (!PyArg_ParseTuple(args, ":resolve"))
        return NULL;    
    f_srvpgm_activate(self);
EXCP1:
    PyErr_SetString(os400Error, "Error trying to activate the service program.");
    return NULL;    
}

static PyMethodDef OS400Srvpgm_methods[] = {
    {"proc",    (PyCFunction)OS400Srvpgm_proc,  METH_VARARGS, srvpgm_proc_doc},
    {"resolve", (PyCFunction)OS400Srvpgm_resolve, METH_VARARGS , srvpgm_resolve_doc},
    {NULL,      NULL}       /* sentinel */
};

static PyObject *
OS400Srvpgm_getattr(OS400Srvpgm *self, char *name)
{
    return Py_FindMethod(OS400Srvpgm_methods, (PyObject *)self, name);
}

PyTypeObject OS400Srvpgm_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyObject_HEAD_INIT(NULL)
    0,          /*ob_size*/
    "OS400Srvpgm",          /*tp_name*/
    sizeof(OS400Srvpgm),    /*tp_basicsize*/
    0,          /*tp_itemsize*/
    /* methods */
    (destructor)OS400Srvpgm_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    (getattrfunc)OS400Srvpgm_getattr, /*tp_getattr*/
    0,          /*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    0,          /*tp_as_mapping*/
    0,          /*tp_hash*/
};

/* --------------------------------------------------------------------- */
/* OS400Proc methods  */

static void
OS400Proc_dealloc(OS400Proc *self)
{
    if (self->parmbuf) 
        PyMem_Free(self->parmbuf);
    Py_XDECREF(self->name);
    Py_XDECREF(self->srvpgm);
    PyObject_Del(self);
}

static PyObject *
OS400Proc_call(OS400Proc *self, PyObject *args, PyObject *keyws)
{
    PyObject *parms = Py_None, *v;
    int i, expres, parmCount = self->parmCount;
    char *xx = NULL;
    void *retval;
    char **ptr;
    volatile _INTRPT_Hndlr_Parms_T  excpData;

#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
/*  if (!PyArg_ParseTuple(args, "|O:call", &parms))
        return NULL; */ 
    /* get function object */
    if (self->ptr == NULL) {
        if (self->srvpgm->ptr == NULL)
            f_srvpgm_activate(self->srvpgm);
        QleGetExp(&(self->srvpgm->actmark), 0, 0, 
                  PyString_AsString((PyObject *)self->name), 
                  &(self->ptr), &expres, NULL);
        if (expres != 1) {
            PyErr_SetString(os400Error, "Procedure not found.");
            return NULL;
        }
    }
    /* convert values */
/*  if (parms != Py_None) { 
        if (!PyTuple_Check(parms) && !PyList_Check(parms)) {
            PyErr_SetString(os400Error, "Key must be a tuple or list with values.");
            return NULL;
        }  */
    parmCount = PyTuple_Size(args);
    if (parmCount > self->parmCount) {
        PyErr_SetString(os400Error, "Too many parameters.");
        return NULL;
    }
    for (i = 0; i < parmCount; i++) {
        v = PyTuple_GetItem(args, i);
        if (f_setparm(self->parmbuf, &(self->types[i]), v))
            return NULL;
    }       
    /* have tried almost everything. */
    /* not possible to get all return types.
    /* have to code all return types. */
    /* the solution is probably to use MI */
    /* this hack gets both a pointer and a rpg string */ 
    /* have to allocate memory for the rpg string */    
    /*if (self->retval.type == 'c')*/
        xx = malloc(sizeof(self->retval.len) + 100);    
    /* call the program */
    switch (parmCount) {
    case 0:     
        retval = ((OS_func_0 *)self->ptr)();
        break;
    case 1:     
        retval = ((OS_func_1 *)self->ptr)(self->types[0].ptr);
        break;
    case 2:     
        /* the diff. in parameterlist is just a test (works alike) */
        retval = ((OS_func_2 *)self->ptr)(self->types[0].ptr, self->types[1].ptr);
        /*retval = ((OS_func_2 *)self->ptr)(self->parmbuf + self->types[0].offset,
                                          self->parmbuf + self->types[1].offset);*/
        break;
    case 3:     
        retval = ((OS_func_3 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr);
        break;
    case 4:     
        retval = ((OS_func_4 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr);
        break;
    case 5:     
        retval = ((OS_func_5 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr,
                                          self->types[4].ptr);
        break;
    case 6:     
        retval = ((OS_func_6 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr,
                                          self->types[4].ptr, self->types[5].ptr);
        break;
    case 7:     
        retval = ((OS_func_7 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr,
                                          self->types[4].ptr, self->types[5].ptr,
                                          self->types[6].ptr);
        break;
    case 8:     
        retval = ((OS_func_8 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr,
                                          self->types[4].ptr, self->types[5].ptr,
                                          self->types[6].ptr, self->types[7].ptr);
        break;
    case 9:     
        retval = ((OS_func_9 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr,
                                          self->types[4].ptr, self->types[5].ptr,
                                          self->types[6].ptr, self->types[7].ptr,
                                          self->types[8].ptr);
        break;
    case 10:        
        retval = ((OS_func_10 *)self->ptr)(self->types[0].ptr, self->types[1].ptr,
                                          self->types[2].ptr, self->types[3].ptr,
                                          self->types[4].ptr, self->types[5].ptr,
                                          self->types[6].ptr, self->types[7].ptr,
                                          self->types[8].ptr, self->types[9].ptr);
        break;
    }
    if (self->retval.type != ' ') {
        if (self->retval.type == 'p')
            v = PyCObject_FromVoidPtr(retval, NULL);
        else if (self->retval.type == 'c')
            v = PyString_FromStringAndSize(retval, f_strlen(retval, self->retval.len));
        else {
        /*  ptr = retval;
            v = f_cvtToPy(ptr[0] + self->retval.offset, self->retval.type, 
                          self->retval.len, self->retval.digits,
                          self->retval.dec);*/
        }
        free(xx);
        return v;
    } else {
        free(xx);
        Py_INCREF(Py_None);
        return Py_None;
    }
EXCP1:
    PyErr_SetString(os400Error, "Error calling procedure.");
    return NULL;    
}

static int
OS400Proc_length(OS400Proc *self)
{
    return self->parmCount;
}

static int
OS400Proc_ass_subscript(OS400Proc *self, PyObject *v, PyObject *w)
{
    int pos;
    if (w == NULL) {
        PyErr_SetString(os400Error, "NULL value not supported.");
        return -1;
    }
    if (!PyInt_Check(v)) {
        PyErr_SetString(os400Error, "Argument must be a number.");
        return -1;
    }
    pos = PyInt_AS_LONG(v);
    if (pos >= self->parmCount) {
        PyErr_SetString(os400Error, "Not so many parameters.");
        return -1;
    }
    return f_setparm(self->parmbuf, &(self->types[pos]), w);
}

static PyObject *
OS400Proc_subscript(OS400Proc *self, PyObject *v)
{
    int pos;
    if (!PyInt_Check(v)) {
        PyErr_SetString(os400Error, "Argument must be a number.");
        return NULL;
    }
    pos = PyInt_AS_LONG(v);
    if (pos >= self->parmCount) {
        PyErr_SetString(os400Error, "Not so many parameters.");
        return NULL;
    }
    return f_getparm(self->parmbuf, &(self->types[pos]));
}

static PyMappingMethods OS400Proc_as_mapping = {
    (inquiry)OS400Proc_length, /*mp_length*/
    (binaryfunc)OS400Proc_subscript, /*mp_subscript*/
    (objobjargproc)OS400Proc_ass_subscript, /*mp_ass_subscript*/
};

static PyMethodDef OS400Proc_methods[] = {
    {NULL,      NULL}       /* sentinel */
};

static PyObject *
OS400Proc_getattr(OS400Proc *self, char *name)
{
    return Py_FindMethod(OS400Proc_methods, (PyObject *)self, name);
}

PyTypeObject OS400Proc_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyObject_HEAD_INIT(NULL)
    0,          /*ob_size*/
    "OS400Proc",            /*tp_name*/
    sizeof(OS400Proc),  /*tp_basicsize*/
    0,          /*tp_itemsize*/
    /* methods */
    (destructor)OS400Proc_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    (getattrfunc)OS400Proc_getattr, /*tp_getattr*/
    0,          /*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    &OS400Proc_as_mapping,  /*tp_as_mapping*/
    0,          /*tp_hash*/
    (ternaryfunc)OS400Proc_call, /*tp_call*/
    0,          /*tp_str*/
};

/* --------------------------------------------------------------------- */
/* OS400Codec methods  */

static void
OS400Codec_dealloc(OS400Codec *self)
{
    iconv_close(self->cdencode);
    iconv_close(self->cddecode);
    PyObject_Del(self);
}

static char codec_encode_doc[] =
"codec.encode(input) -> string.\n\
\n\
Encodes the object input and returns a coded string.";

static PyObject *
OS400Codec_encode(OS400Codec *self, PyObject *args)
{
    int size;
    char *in;
    if (!PyArg_ParseTuple(args, "s#:encode", &in, &size))
        return NULL;    
    return f_convert(self->cdencode, in, size);
}

static char codec_decode_doc[] =
"codec.decode(input) -> string.\n\
\n\
Decodes the object input and returns a decoded string.";

static PyObject *
OS400Codec_decode(OS400Codec *self, PyObject *args)
{
    int size;
    char *in;
    if (!PyArg_ParseTuple(args, "s#:decode", &in, &size))
        return NULL;    
    return f_convert(self->cddecode, in, size);
}

static PyMethodDef OS400Codec_methods[] = {
    {"encode", (PyCFunction)OS400Codec_encode,  METH_VARARGS, codec_encode_doc},
    {"decode", (PyCFunction)OS400Codec_decode, METH_VARARGS , codec_decode_doc},
    {NULL,      NULL}       /* sentinel */
};

static PyObject *
OS400Codec_getattr(OS400Codec *self, char *name)
{
    return Py_FindMethod(OS400Codec_methods, (PyObject *)self, name);
}

PyTypeObject OS400Codec_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyObject_HEAD_INIT(NULL)
    0,          /*ob_size*/
    "OS400Codec",          /*tp_name*/
    sizeof(OS400Codec),    /*tp_basicsize*/
    0,          /*tp_itemsize*/
    /* methods */
    (destructor)OS400Codec_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    (getattrfunc)OS400Codec_getattr, /*tp_getattr*/
    0,          /*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    0,          /*tp_as_mapping*/
    0,          /*tp_hash*/
};
/* --------------------------------------------------------------------- */

static char program_doc[] =
"os400.Program(name, lib, parameters) -> Program object.\n\
\n\
Returns a Program object.\n\
Name should be the name of the program (uppercase).\n\
Lib is the library, the special value *LIBL is supported.\n\
Parmeters should be a tuple of parameter descriptions.\n\
There is a limit of 10 parameters.\n\
Each parameter description shold be a tuple with the following fields:\n\
Type(string), length(int), decimals(int, optional).\n\
  The following types are valid:\n\
    'c' String. ('c',20)\n\
    'i' Binary(Integer). ('i', 4) Length is here number of bytes (4/8).\n\
    'f' Float. ('f', 4) Length is here number of bytes (4/8).\n\
    'd' packed decimal ('d',5,0)\n\
    'z' Zoned decimal ('z',5,0)\n\
The Program object has the following methodes:\n\
  resolve - Do a new resolve.\n\
To set the parameters use either a tuple in the call methode,\n\
or use indexing. (mypgm1[0] = 'TEST')\n\
To see values after the call use indexing. (print mypgm1[0])\n\
Here is an example:\n\
  getprice = os400.Program('GETPRICE','*LIBL',(('c',10),('d',9,0)))\n\
  mypgm1('PART_A', 0)\n\
  price = mypgm1[1]";

static PyObject *
os400_OS400Program(PyObject *self, PyObject *args)
{
    int i;
    char *name, *lib;
    PyObject *parm = Py_None, *item, *sub;
    OS400Program *pgm;

    if (!PyArg_ParseTuple(args, "ssO:Program", &name, &lib, &parm))
        return NULL;
    if (!PyTuple_Check(parm)) {
        PyErr_SetString(os400Error, "Third argument must be a tuple.");
        return NULL;
    }
    pgm = PyObject_New(OS400Program, &OS400Program_Type);
    if (pgm == NULL)
        return NULL;
    strtostr400(pgm->name, name, 10);
    strtostr400(pgm->lib, lib, 10);
    pgm->ptr = NULL;
    pgm->parmbuf = NULL;        
    pgm->parmLen = 0;
    pgm->parmCount = 0;
    /* parameters */
    if (parm != Py_None) {
        pgm->parmCount = PyTuple_Size(parm);
        if (pgm->parmCount > 10) {
            PyErr_SetString(os400Error, "Maximum number of parameters is 10.");
            Py_DECREF(pgm);
            return NULL;
        }   
        /* check parameters and get size */
        for (i = 0; i < pgm->parmCount; i++) {
            /* get parameter type */
            if (f_checkparm(&(pgm->types[i]), 
                                 PyTuple_GET_ITEM((PyTupleObject *)parm, i))) {
                Py_DECREF(pgm);
                return NULL;
            } else {
                pgm->types[i].offset = pgm->parmLen;
                pgm->parmLen += pgm->types[i].len;
            }
        }
    }
    /* allcoate parameter area and set pointers */
    if (pgm->parmLen > 0) {
        pgm->parmbuf = PyMem_Malloc(pgm->parmLen);
        for (i = 0; i < pgm->parmCount; i++) {
            if (pgm->types[i].type != 'p')
                pgm->types[i].ptr = pgm->parmbuf + pgm->types[i].offset;
        }
    }
    return (PyObject *) pgm;
}

static char srvpgm_doc[] =
"os400.Srvpgm(name, lib) -> Srvpgm object.\n\
\n\
Returns a Srvpgm object.\n\
Name should be the name of the service program (uppercase).\n\
Lib is the library, the special value *LIBL is supported.\n\
The Srvpgm object has the following methods:\n\
  resolve - Loads the service program.\n\ 
  proc    - Defines and returns a Procedure object.";

static PyObject *
os400_OS400Srvpgm(PyObject *self, PyObject *args)
{
    char *name, *lib;
    OS400Srvpgm *pgm;

    if (!PyArg_ParseTuple(args, "ss:Srvpgm", &name, &lib))
        return NULL;
    pgm = PyObject_New(OS400Srvpgm, &OS400Srvpgm_Type);
    if (pgm == NULL)
        return NULL;
    strtostr400(pgm->name, name, 10);
    strtostr400(pgm->lib, lib, 10);
    pgm->ptr = NULL;
    pgm->actmark = 0;
    return (PyObject *) pgm;
}

static char rtvdtaara_doc[] =
"os400.rtvdtaara(name, lib, length) -> String.\n\
\n\
Returns a String with the value of the data area.";

static PyObject *
os400_rtvdtaara(PyObject *self, PyObject *args)
{
    Qus_EC_t error;
    char dtaara[21], *name, *lib;
    char buffer[2100], *pbuf;
    int len;
    Qwc_Rdtaa_Data_Returned_t *retData;
    PyObject *v = NULL;
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)

    if (!PyArg_ParseTuple(args, "ssi:Program", &name, &lib, &len))
        return NULL;
    if (strlen(name) > 10 || strlen(lib) > 10) {
        PyErr_SetString(os400Error, "Error when retreiving data area.");
        return NULL;
    }
    pbuf = (char *) buffer;
    strtostr400(dtaara, name, 10);
    strtostr400((dtaara + 10), lib, 10);
    error.Bytes_Provided = sizeof(error);
    QWCRDTAA(pbuf, 2100, dtaara, 1, len, &error);
    if (error.Bytes_Available > 0) {
        PyErr_SetString(os400Error, "Error when retreiving data area.");
        return NULL;
    }
    retData = (Qwc_Rdtaa_Data_Returned_t *)pbuf;
    if (!strncmp(retData->Type_Value_Returned, "*CHAR     ", 10))
        return PyString_FromStringAndSize(pbuf + sizeof(*retData),
                                          retData->Length_Value_Returned);
    else if (!strncmp(retData->Type_Value_Returned, "*DEC      ", 10)) {
        if (retData->Number_Decimal_Positions > 0 || retData->Length_Value_Returned > 4)
            return PyInt_FromLong(QXXPTOI(pbuf + sizeof(*retData),
                                          retData->Length_Value_Returned,
                                          retData->Number_Decimal_Positions));
        else
            return PyLong_FromDouble(QXXPTOD(pbuf + sizeof(*retData),
                                             retData->Length_Value_Returned,
                                             retData->Number_Decimal_Positions));
    }
 EXCP1:
    PyErr_SetString(os400Error, "Error when retreiving data area.");
    return NULL;                    
}

static char codec_doc[] =
"os400.Codec(ccsid) -> Codec object.\n\
\n\
Returns a Codec object.\n\
ccsid should be the ccsid to convert from/to.";

static PyObject *
os400_OS400Codec(PyObject *self, PyObject *args)
{
    char *name;
    OS400Codec *co;

    if (!PyArg_ParseTuple(args, "s:Codec", &name))
        return NULL;
    co = PyObject_New(OS400Codec, &OS400Codec_Type);
    if (co == NULL) return NULL;
    if (strcmp(name, "latin1") == 0)
        strcpy(co->ccsid, "00819");
    else if(strcmp(name, "utf8") == 0)
        strcpy(co->ccsid, "01208");
    else if(strcmp(name, "ucs2") == 0)
        strcpy(co->ccsid, "01200");
    else if(strlen(name) > 5) {
        PyErr_SetString(os400Error, "Ccsid not valid.");
        Py_DECREF(co);
        return NULL;
    } else
        strcpy(co->ccsid, name);
    strcpy(co->name, name);
    initConversion(co->ccsid, &co->cddecode, &co->cdencode);
    if (co->cddecode.return_value == -1 || co->cdencode.return_value == -1) {
        PyErr_SetString(os400Error, "Error while creating codec.");
        Py_DECREF(co);
        return NULL;
    }
    return (PyObject *) co;
}

static char chgdtaara_doc[] =
"os400.chgdtaara(name, lib, value) -> None.\n\
\n\
Change data area.";

static PyObject *
os400_chgdtaara(PyObject *self, PyObject *args)
{
    int retval;
    char *name, *lib, *p;
    _DTAA_NAME_T  dtaara;
    char buf[200];
    PyObject *v;
    volatile _INTRPT_Hndlr_Parms_T  excpData;

#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    if (!PyArg_ParseTuple(args, "ssO:Program", &name, &lib, &v))
        return NULL;
    if (PyString_Check(v)) {
        p = (char *) &dtaara;
        memset(p, ' ', sizeof(dtaara));
        memcpy(p, name, strlen(name));
        memcpy(p + 10, lib, strlen(lib));
        QXXCHGDA(dtaara, 1, PyString_Size(v), PyString_AsString(v));       
    } else if (PyNumber_Check(v)) {
        strcpy(buf, "chgdtaara ");
        strcat(buf, lib);
        strcat(buf, "/");
        strcat(buf, name);
        strcat(buf, " value(");
        strcat(buf, PyString_AsString(PyObject_Str(v)));
        strcat(buf, ")");
        retval = system(buf);
        if (retval != 0) {
            PyErr_SetString(os400Error, "Error when changing data area.");
            return NULL;
        }
    }
    Py_INCREF(Py_None);
    return Py_None;
 EXCP1:
    PyErr_SetString(os400Error, "Error when changing data area.");
    return NULL;
    
}

static char commit_doc[] =
"os400.commit([String]) -> None\n\
\n\
Commit all changes.";

static PyObject *
os400_commit(PyObject *self, PyObject *args)
{
    char *s = NULL;
    if (!PyArg_ParseTuple(args, "|s:commit", &s))
        return NULL;
    if (s)
        _Rcommit(s);
    else
        _Rcommit();
    Py_INCREF(Py_None);
    return Py_None;
}

static char rollback_doc[] =
"os400.rollback() -> None\n\
\n\
Commit all changes.";

static PyObject *
os400_rollback(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":rollback"))
        return NULL;
    _Rrollbck();
    Py_INCREF(Py_None);
    return Py_None;
}

#ifdef ASTEC

static void
array_dealloc(void *array)
{
    free(array); 
}

static char arrayToList_doc[] =
"os400.arrayToList(array, [keys],[fields]) -> List.\n\
\n\
Convert a rpg array to a list.\n\
The key will be the first field(s) in the list.\n\
At the same time adds a destructor to PyCObject.";
 
static PyObject *
os400_arrayToList(PyObject *self, PyObject *args)
{
    parmtype_t keytypes[10], types[50];
    int i, itemno, offset, keysLen, keysCount, fieldsLen, fieldsCount;
    int keysize, datasize;
    PyObject *array, *keys = Py_None, *fields = Py_None, *list, *item, *v;
    ArrayType *arr;
    char *carr, *buf;
    volatile _INTRPT_Hndlr_Parms_T  excpData;

#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    if (!PyArg_ParseTuple(args, "O|OO:arrayToList", &array, &keys, &fields))
        return NULL;
    if (!PyCObject_Check(array)) {
        PyErr_SetString(os400Error, "Array not correct type.");
        return NULL;
    }
    /* get array from pointer */
    arr = (ArrayType *)PyCObject_AsVoidPtr(array);
    carr = (char *)arr;
    keysize = arr->wKeySize;
    datasize = arr->wElemSize - arr->wKeySize;
    /* keys */
    if (keys == Py_None) {
        if (keysize > 0)
            keysCount = 1;
        else
            keysCount = 0;
    } else {
        if (!PyTuple_Check(keys)) {
            PyErr_SetString(os400Error, "Field definitions must be a tuple of tuples.");
            return NULL;
        } else if (PyTuple_Size(keys) > 10) {
            PyErr_SetString(os400Error, "Too many fields.");
            return NULL;
        }
        keysLen = 0;
        keysCount = PyTuple_Size(keys);
        /* check fields and get size */
        for (i = 0; i < keysCount; i++) {
            /* get field type */
            if (f_checkparm(&(keytypes[i]), PyTuple_GetItem(keys, i))) {
                PyErr_SetString(os400Error, "Key field definition not valid.");
                return NULL;
            } else {
                keytypes[i].offset = keysLen;
                keysLen += keytypes[i].len;
            }
        }
        /* check total length */
        if (keysLen != keysize) {
            PyErr_SetString(os400Error, "Key fields definition total length not valid.");
            return NULL;
        }
    }
    /* fields */
    if (fields == Py_None) {
        if (datasize > 0)
            fieldsCount = 1;
        else
            fieldsCount = 0;            
    } else {
        if (!PyTuple_Check(fields)) {
            PyErr_SetString(os400Error, "Field definitions must be a tuple of tuples.");
            return NULL;
        } else if (PyTuple_Size(fields) > 50) {
            PyErr_SetString(os400Error, "Too many fields.");
            return NULL;
        }
        fieldsLen = 0;
        fieldsCount = PyTuple_Size(fields);
        /* check fields and get size */
        for (i = 0; i < fieldsCount; i++) {
            /* get field type */
            if (f_checkparm(&(types[i]), PyTuple_GetItem(fields, i))) {
                PyErr_SetString(os400Error, "Field definition not valid.");
                return NULL;
            } else {
                types[i].offset = fieldsLen;
                fieldsLen += types[i].len;
            }
        }
        /* check total length */
        if (fieldsLen != datasize) {
            PyErr_SetString(os400Error, "Fields definition total length not valid.");
            return NULL;
        }
    }
    /* read all items */ 
    list = PyList_New(0);
    for (itemno = 0; itemno < arr->wElements; itemno++) {  
        /* use internal representation */
        if (keysCount + fieldsCount > 1)
            item = PyTuple_New(keysCount + fieldsCount);
        else
            item = NULL;
        if (keysCount > 0) {
            buf = carr + 20 + (itemno * (keysize + datasize)) + datasize;
            if (keys == Py_None) {
                v = PyString_FromStringAndSize(buf, f_strlen(buf, keysize));
                if (item) PyTuple_SetItem(item, 0, v);
                else item = v;
            } else {
                for (i = 0; i < keysCount; i++) {
                    v = f_cvtToPy(buf + keytypes[i].offset, keytypes[i].type, 
                                  keytypes[i].len, keytypes[i].digits,
                                  keytypes[i].dec);
                    if (item) PyTuple_SetItem(item, i, v);
                    else item = v;
                }       
            }
        }
        if (fieldsCount > 0) {
            buf = carr + 20 + (itemno * (keysize + datasize));
            if (fields == Py_None) {
                v = PyString_FromStringAndSize(buf, f_strlen(buf, datasize));
                if (item) PyTuple_SetItem(item, keysCount, v);
                else item = v;
            } else {
                for (i = 0; i < fieldsCount; i++) {
                    v = f_cvtToPy(buf + types[i].offset, types[i].type, 
                                  types[i].len, types[i].digits, types[i].dec);
                    if (item) PyTuple_SetItem(item, keysCount + i, v);
                    else item = v;
                }       
            }
        }
        PyList_Append(list, item);
        Py_DECREF(item);            
    }   
    /* set the destructor */
    ((PyCObject *)array)->destructor = array_dealloc;
    return list;
EXCP1:
    PyErr_SetString(os400Error, "Error occured while converting to list.");
return NULL;
}

static char array_doc[] =
"os400.array(array) -> None.\n\
\n\
Adds a array destructor to PyCObject.";
 
static PyObject *
os400_array(PyObject *self, PyObject *args)
{
    PyObject *array;
    PyCObject *carr;
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    if (!PyArg_ParseTuple(args, "O:array", &array))
        return NULL;
    if (!PyCObject_Check(array)) {
        PyErr_SetString(os400Error, "Not correct type.");
        return NULL;
    }
    carr = (PyCObject *)array;
    carr->destructor = array_dealloc;
    Py_INCREF(array);
    return array;
EXCP1:
    PyErr_SetString(os400Error, "Error occured when deleting array.");
    return NULL;    
}
#endif

/* List of functions defined in the module */

static PyMethodDef os400_methods[] = {
    {"Program", os400_OS400Program, METH_VARARGS, program_doc},
    {"Srvpgm",  os400_OS400Srvpgm,  METH_VARARGS, srvpgm_doc},
    {"rtvdtaara",   os400_rtvdtaara, METH_VARARGS, rtvdtaara_doc},
    {"chgdtaara",   os400_chgdtaara, METH_VARARGS, chgdtaara_doc},
    {"commit",   os400_commit, METH_VARARGS, commit_doc},
    {"rollback",   os400_rollback, METH_VARARGS, rollback_doc},
    {"Codec",   os400_OS400Codec, METH_VARARGS, codec_doc},
#ifdef ASTEC
    {"array",   os400_array, METH_VARARGS, array_doc},
    {"arrayToList", os400_arrayToList, METH_VARARGS, arrayToList_doc},
#endif
    {NULL,      NULL}       /* sentinel */
};

DL_EXPORT(void)
initos400(void)
{
    PyObject *m, *d;

    /* Initialize the type of the new type object here; doing it here
     * is required for portability to Windows without requiring C++. */
    OS400Program_Type.ob_type = &PyType_Type;

    /* Create the module and add the functions */
    m = Py_InitModule("os400", os400_methods);

    /* Add some symbolic constants to the module */
    d = PyModule_GetDict(m);
    os400Error = PyErr_NewException("os400.error", NULL, NULL);
    PyDict_SetItemString(d, "error", os400Error);
}










