/*
 * file400  AS/400 Database(file) access 
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
 */

#include "Python.h"
#include <signal.h>
#include <xxfdbk.h>
#include <xxcvt.h>
#include <qusec.h>
#include <qdbrtvfd.h>
#include <quscrtus.h>
#include <qusdltus.h>
#include <qusptrus.h>
#include <qusgen.h>
#include <quslmbr.h>
#include <decimal.h>
#include <float.h>
#include "file400module.h"
#include "as400misc.h"

/* Open modes */
#define F4_OREAD 10
#define F4_OUPDATE 12
#define F4_OWRITE 14
/* output */
#define LIST 0
#define OBJ 1
#define DICT 2

static PyObject *file400Error;
static PyObject *dummyClassName;
static PyObject *dummyClass;

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

/* convert field to Python format */    
static PyObject *
f_cvtToPy(unsigned char *p, unsigned short type, int len,
          int digits, int dec)
{
    short dsh, varlen;
    long dl;
    long long dll;
    float dfl;
    double ddbl;
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)

    switch (type) {
    /* binary */
    case 0:
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
            PyErr_SetString(file400Error, "Data type error.");
            return NULL;
        }
    /* float */
    case 1:
        if (len == 4) {
            memcpy(&dfl, p, 4);
            return PyFloat_FromDouble((double)dfl);
        }
        else if (len == 8) {
            memcpy(&ddbl, p, 8);
            return PyFloat_FromDouble((double)ddbl);
        } else {
            PyErr_SetString(file400Error, "Data type error.");
            return NULL;
        }
    /* zoned */
    case 2:
        if (dec == 0 && digits < 10) 
            return PyInt_FromLong(QXXZTOI(p, digits, dec));
        else {
            char buf[100], *ss;
            zonedtostr((char *)buf, p, digits, dec, '.');  
            if (dec == 0)
                /*return PyLong_FromDouble(strtod((char *)buf, &ss));*/
                return PyLong_FromLongLong(strtoll((char *)buf, &ss, 10));
            else
                return PyFloat_FromDouble(strtod((char *)buf, &ss));
        }
    /* packed */
    case 3:
        if (dec == 0 && digits < 10) 
            return PyInt_FromLong(QXXPTOI(p, digits, dec));
        else {
            char buf[100], *ss;
            packedtostr((char *)buf, p, digits, dec, '.');  
            if (dec == 0)
                /*return PyLong_FromDouble(strtod((char *)buf, &ss));*/
                return PyLong_FromLongLong(strtoll((char *)buf, &ss, 10));
            else
                return PyFloat_FromDouble(strtod((char *)buf, &ss));
        }
    /* char */
    case 4:
        return PyString_FromStringAndSize(p, f_strlen(p, len));
    /* dbcs (unicode) */
    case 6:
        return PyUnicode_FromWideChar((wchar_t *)p, len / 2);
    /* date, time, timestamp */
    case 11: case 12: case 13:
        return PyString_FromStringAndSize(p, len);
    /* varchar */
    case 0x8004:
        memcpy(&varlen, p, sizeof(short));
        p += sizeof(short);
        return PyString_FromStringAndSize(p, f_strlen(p, varlen));
    /* vardbcs (unicode) */
    case 0x8006:
        memcpy(&varlen, p, sizeof(short));
        p += sizeof(short);
        return PyUnicode_FromWideChar((wchar_t *)p, varlen / sizeof(short));
    default:
        PyErr_SetString(file400Error, "Data type not supported.");
        return NULL;
    }   
 EXCP1:
    PyErr_SetString(file400Error, "Data conversion error.");
    return NULL;
}

/* change decimal seperator */
#pragma inline(f_chgDecSep)
static
f_chgDecSep(char *buf, char decsep, int len)
{
    char *c1 = memchr(buf, '.', len);
    if (c1 != NULL)
        c1[0] = decsep;
}

/* convert field to string */   
static int
f_addToString(char *buf, unsigned char *p, unsigned short type, 
              int len, int digits, int dec, char decsep)
{
    short varlen;
    float dfl;
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)

    switch (type) {
    /* binary */
    case 0:
        if (len == 2) {
            return sprintf(buf, "%hd", *p);
        } else if (len == 4) {
            return sprintf(buf, "%ld", *p);
        } else if (len == 8) {
            return sprintf(buf, "%lld", *p);
        } else {
            PyErr_SetString(file400Error, "Data type error.");
            return -1;
        }
    /* float */
    case 1:
        if (len == 4) {
            memcpy(&dfl, p, 4);         
            varlen = sprintf(buf, "%f", (double)dfl);
        }
        else if (len == 8) {
            varlen = sprintf(buf, "%f", *p);
        } else {
            PyErr_SetString(file400Error, "Data type error.");
            return -1;
        }
        if (decsep != '.')
            f_chgDecSep(buf, decsep, varlen);
        return varlen;
    /* zoned */
    case 2:
        return strlen(zonedtostr(buf, p, digits, dec, decsep));
    /* packed */
    case 3:
        return strlen(packedtostr(buf, p, digits, dec, decsep));
    /* char */
    case 4:
        len = f_strlen(p, len);
        memcpy(buf, p, len);
        return len;
    /* dbcs (unicode) */
    case 6:
        len = wcstombs(buf, (wchar_t *)p, len);
        return f_strlen(buf, len);
    /* date, time, timestamp */
    case 11: case 12: case 13:
        len = f_strlen(p, len);
        memcpy(buf, p, len);
        return len;
    /* varchar */
    case 0x8004:
        memcpy(&varlen, p, sizeof(short));
        p += sizeof(short);
        varlen = f_strlen(p, varlen);
        memcpy(buf, p, varlen);
        return varlen;
    /* vardbcs (unicode) */
    case 0x8006:
        memcpy(&varlen, p, sizeof(short));
        p += sizeof(short);
        varlen = wcstombs(buf, (wchar_t *)p, varlen);
        return f_strlen(buf, varlen);
    default:
        PyErr_SetString(file400Error, "Data type not supported.");
        return -1;
    }   
 EXCP1:
    PyErr_SetString(file400Error, "Data conversion error.");
    return -1;
}

static int
f_xmlescape(char *buf, char *s, int len) 
{
    char *p, *q, *c = buf;
    /* remove ending blanks */
    len--;
    while (len >= 0 && isspace(Py_CHARMASK(s[len])))
        len--;
    q = s + len + 1;
    for (p = s; p < q; p++) {
        if (*p == '<' || *p == '>' || *p == '&') {
            if (p > s) {
                memcpy(c, s, p - s);
                c += p - s;
            }
            if (*p == '<') c += strlen(strcpy(c, "&lt;"));
            else if (*p == '>') strlen(strcpy(c, "&gt;"));
            else if (*p == '&') strlen(strcpy(c, "&amp;"));
            s = p + 1;
        }
    }
    if (p > s) {
        memcpy(c, s, p - s);
        c += p - s;
    }
    return c - buf;
}


static int
f_addToXml(char *buf, char * name, unsigned char *p, unsigned short type,
           int len, int digits, int dec, char decsep, int output)
{
    char *c = buf;
    short varlen;
    float dfl;
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)

    if (output == OBJ) {
        c += strlen(strcpy(c, "<"));
        c += strlen(strcpy(c, name));
    } else if (output == DICT) {
        c += strlen(strcpy(c, "<k>"));
        c += strlen(strcpy(c, name));
        c += strlen(strcpy(c, "</k><v"));
    } else {
        c += strlen(strcpy(c, "<v"));
    }
    if (type == 0) {
        /* binary */
        if (len == 2) {
            c += strlen(strcpy(c, " t=\"i\">"));
            c += sprintf(c, "%hd", *p);
        } else if (len == 4) {
            c += strlen(strcpy(c, " t=\"i\">"));
            c += sprintf(c, "%ld", *p);
        } else if (len == 8) {
            c += strlen(strcpy(c, " t=\"l\">"));
            c += sprintf(c, "%lld", *p);
        } else {
            PyErr_SetString(file400Error, "Data type error.");
            return -1;
        }
    } else if (type == 1) {
        /* float */
        c += strlen(strcpy(c, " t=\"f\">"));
        if (len == 4) {
            memcpy(&dfl, p, 4);         
            varlen = sprintf(c, "%f", (double)dfl);
        }
        else if (len == 8) {
            varlen = sprintf(c, "%f", *p);
        } else {
            PyErr_SetString(file400Error, "Data type error.");
            return -1;
        }
        if (decsep != '.')
            f_chgDecSep(c, decsep, varlen);
        c += varlen;
    } else if (type == 2) {
        /* zoned */
        if (dec == 0)
            if (digits < 10)
                c += strlen(strcpy(c, " t=\"i\">"));
            else
                c += strlen(strcpy(c, " t=\"l\">"));
        else
            c += strlen(strcpy(c, " t=\"f\">"));
        c += strlen(zonedtostr(c, p, digits, dec, decsep));
    } else if (type == 3) {
        /* packed */
        if (dec == 0)
            if (digits < 10)
                c += strlen(strcpy(c, " t=\"i\">"));
            else
                c += strlen(strcpy(c, " t=\"l\">"));
        else
            c += strlen(strcpy(c, " t=\"f\">"));
        c += strlen(packedtostr(c, p, digits, dec, decsep));
    } else if (type == 4) {
        /* char */
        c += strlen(strcpy(c, ">"));
        c += f_xmlescape(c, p, len);
    } else if (type == 4) {
        /* dbcs (unicode) */
        char buf[32000];
        int len = wcstombs(buf, (wchar_t *)p, len);     
        c += strlen(strcpy(c, ">"));
        c += f_xmlescape(c, buf, len);
    } else if (type >= 11 && type <= 13) {
        /* date, time, timestamp */
        c += strlen(strcpy(c, ">"));
        memcpy(c, p, len);
        c += len;
    } else if (type == 0x8004) {
        /* varchar */
        c += strlen(strcpy(c, ">"));
        memcpy(&varlen, p, sizeof(short));
        p += sizeof(short);
        c += f_xmlescape(c, p, varlen);
    } else if (type == 0x8006) {
        /* vardbcs (unicode) */
        char buf[32000];
        c += strlen(strcpy(c, ">"));
        memcpy(&varlen, p, sizeof(short));
        p += sizeof(short);
        wcstombs(buf, (wchar_t *)p, varlen);        
        c += f_xmlescape(c, buf, varlen);
    } else {
        PyErr_SetString(file400Error, "Data type not supported.");
        return -1;
    }   
    if (output == OBJ) {
        c += strlen(strcpy(c, "</"));
        c += strlen(strcpy(c, name));
        c += strlen(strcpy(c, ">"));
    } else {
        c += strlen(strcpy(c, "</v>"));
    }
    return c - buf;

 EXCP1:
    PyErr_SetString(file400Error, "Data conversion error.");
    return -1;
}

/* convert field from Python format */  
static int
f_cvtFromPy(unsigned char *p, unsigned short type, int len,
            int digits, int dec, PyObject *o)
{
    short dsh;
    long dlo;
    long long dll;
    float dfl;
    double ddbl;
    char *c;
    int i;
    short ulen;
    volatile _INTRPT_Hndlr_Parms_T  excpData;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)

    switch (type) {
    /* binary */
    case 0:
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
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* float */
    case 1:
        if (len == 4) {
            if (PyInt_Check(o))
                dfl = PyInt_AS_LONG(o);
            else if (PyLong_Check(o))
                dfl = PyLong_AsLong(o);
            else if (PyFloat_Check(o))
                dfl = PyFloat_AS_DOUBLE(o);
            else {
                PyErr_SetString(file400Error, "Data conversion error.");
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
                PyErr_SetString(file400Error, "Data conversion error.");
                return -1;
            }
            memcpy(p, &ddbl, len);
            return 0;
        }
    /* zoned */
    case 2:
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
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* packed */
    case 3:
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
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* char */
    case 4:
        if (PyString_Check(o)) { 
            c = PyString_AS_STRING(o);
            i = PyString_GET_SIZE(o);
            if (i >= len)
                memcpy(p, c, len);
            else {
                memcpy(p, c, i);
                p += i;
                memset(p, ' ', len - i);
            }
            return 0;
        }
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* DBCS (unicode) */
    case 6:
        if (PyUnicode_Check(o)) { 
            ulen = len / sizeof(wchar_t);
            PyUnicode_AsWideChar((PyUnicodeObject *)o, (wchar_t *)p, ulen);
            if (PyUnicode_GET_SIZE(o) < ulen) {
                p += len;
                wmemset((wchar_t *)p, L' ', ulen - PyUnicode_GET_SIZE(o));
            }
            return 0;
        }
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* date, time, timestamp */
    case 11: case 12: case 13:
        if (PyString_Check(o)) { 
            c = PyString_AS_STRING(o);
            i = PyString_GET_SIZE(o);
            if (i >= len)
                memcpy(p, c, len);
            else {
                memcpy(p, c, i);
                p += i;
                memset(p, ' ', len - i);
            }
            return 0;
        }
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* varchar */
    case 0x8004:
        if (PyString_Check(o)) { 
            c = PyString_AS_STRING(o);
            i = PyString_GET_SIZE(o);
            if (i > (len - 2))
                dsh = (len - 2);
            else
                dsh = i;
            memcpy(p, &dsh, 2);
            p += 2;
            memcpy(p, c, dsh);
            return 0;
        }
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    /* vardbc (unicode) */
    case 0x8006:
        if (PyUnicode_Check(o)) { 
            ulen = len / sizeof(wchar_t);           
            dsh = PyUnicode_GET_SIZE(o) * sizeof(wchar_t);
            if (dsh > len)
                dsh = len;
            memcpy(p, &dsh, sizeof(short));
            p += sizeof(short);
            PyUnicode_AsWideChar((PyUnicodeObject *)o, (wchar_t *)p, ulen);
            return 0;
        }
        PyErr_SetString(file400Error, "Data conversion error.");
        return -1;
    default:
        PyErr_SetString(file400Error, "Data type not supported.");
        return -1;
    }   
 EXCP1:
    PyErr_SetString(file400Error, "Data conversion error.");
    return -1;
}

/* clear the record */  
static int
f_clear(File400Object *file, unsigned char *record)
{
    int i;
    unsigned char *p;
    memset(record, ' ', file->recLen);
    /* set all fields that don't hve blanks as default */ 
    for (i = 0; i < file->fieldCount; i++) {
        p = record + file->fieldArr[i].offset;
        switch(file->fieldArr[i].type) {
        /* binary, float*/
        case 0: case 1:
            memset(p, '\0', file->fieldArr[i].len);
            break;
        /* zoned */
        case 2:
            memset(p, 0xf0, file->fieldArr[i].len);
            break;
        /* packed */
        case 3:
            memset(p, 0x00, file->fieldArr[i].len);
            p += file->fieldArr[i].len - 1;
            memset(p, 0x0f, 1);
            break;
        /* char */
        case 4:
            break;
        /* DBCS (unicode) */
        case 6:
            wmemset((wchar_t *)p, L' ', (size_t)file->fieldArr[i].len / 2);
            break;
        /* date, time, timestamp */
        case 11: case 12: case 13:
            memset(p, '\0', file->fieldArr[i].len);
            break;
        /* varchar */
        case 0x8004: case 0x8006:
            memset(p, '\0', sizeof(short));
            break;
        default:
            memset(p, '\0', file->fieldArr[i].len);
        }
    }
}

/* internal routine to get field value from record */   
static PyObject *
f_getFieldValue(File400Object *self, int pos, unsigned char *p)
{
    return f_cvtToPy(p + self->fieldArr[pos].offset,
                     self->fieldArr[pos].type, self->fieldArr[pos].len,
                     self->fieldArr[pos].digits, self->fieldArr[pos].dec);
}

/* internal routine to get field value as string from record */ 
static PyObject *
f_getFieldString(File400Object *self, int pos, char decsep, unsigned char *p)
{
    PyObject *o;
    o = f_cvtToPy(p + self->fieldArr[pos].offset,
                     self->fieldArr[pos].type, self->fieldArr[pos].len,
                     self->fieldArr[pos].digits, self->fieldArr[pos].dec);
    if (PyString_Check(o))
        return o;
    else {      
        PyObject *s1 = PyObject_Str(o);
        if (decsep != '.' && PyFloat_Check(o)) {
            char *c2 = strchr(PyString_AsString(s1), '.');
        if (c2 != NULL)
            c2[0] = decsep;
        }
        Py_DECREF(o);
        return s1;
    }
}

/* internal routine to get key value */ 
static PyObject *
f_getKeyValue(File400Object *self, int pos)
{
    unsigned char *p;
    p = self->fp->riofb.key;
    p += self->keyArr[pos].offset;
    return f_cvtToPy(p, self->keyArr[pos].type, self->keyArr[pos].len,
                     self->keyArr[pos].digits, self->keyArr[pos].dec);
}

/* internal routine to set field value */   
static int
f_setFieldValue(File400Object *self, int pos, PyObject *o, unsigned char *p)
{
    if (self->omode == F4_OWRITE)
        return f_cvtFromPy(p + self->fieldArr[pos].offset,
                       self->fieldArr[pos].type, self->fieldArr[pos].len,
                       self->fieldArr[pos].digits, self->fieldArr[pos].dec, o);
    else    
        return f_cvtFromPy(p + self->fieldArr[pos].offset,
                       self->fieldArr[pos].type, self->fieldArr[pos].len,
                       self->fieldArr[pos].digits, self->fieldArr[pos].dec, o);
}

/* internal routine to set key value */ 
static int
f_setKeyValue(File400Object *self, int pos, PyObject *o)
{
    unsigned char *p;
    p = self->fp->riofb.key;
    p += self->keyArr[pos].offset;
    return f_cvtFromPy(p, self->keyArr[pos].type, self->keyArr[pos].len,
                       self->keyArr[pos].digits, self->keyArr[pos].dec, o);
}

/* internal routine to convert string to upper */
static PyObject *
f_PyStringToUpper(PyObject *so) {
    PyObject *new;
    char *s, *snew = NULL;
    int i, c;
    s = PyString_AS_STRING(so);
    for (i = 0; i < 10; i++) {
        if (*s == '\0')
            break;
        c = Py_CHARMASK(*s);
        if (islower(c)) {          
            if (snew == NULL) {
                new = PyString_FromStringAndSize(NULL, PyString_GET_SIZE(so));
                if (new == NULL) {
                    Py_INCREF(Py_None);
                    return Py_None;
                }
                snew = PyString_AS_STRING(new);
                if (i > 0){ 
                    s -= i;
                    memcpy(snew, s, i);
                    s += i;
                    snew += i;
                }
            }
            *snew++ = toupper(c);              
        } else if (snew != NULL)
            *snew++ = c;                                
        *s++;
    }
    if (snew) {
        *snew = '\0';
        return new;
    } else {
        Py_INCREF(so);
        return so;
    }
}

/* run command */
static int
f_system(char *cmd)
{
    int sts;
    Py_BEGIN_ALLOW_THREADS
    sts = system(cmd);
    Py_END_ALLOW_THREADS
    return sts;
}

/* internal routine to get field position */    
static int
f_getFieldPos(File400Object *self, PyObject *field)
{
    PyObject *posO;
    int i;
    if (PyInt_Check(field)) {
        i = PyInt_AS_LONG(field);
        if (i >= self->fieldCount)
            i = -1;
        return i;
    } else if (PyString_Check(field)) {
        /* convert to upper, first check */
        field = f_PyStringToUpper(field);
        posO = PyDict_GetItem(self->fieldDict, field);
        Py_DECREF(field);
        if (posO == NULL)
            return -1;
        else {
            return PyInt_AS_LONG(posO);
        }
    }
    return -1;
}

/* internal routine to get key position */  
static int
f_getKeyPos(File400Object *self, PyObject *field)
{
    PyObject *posO;
    int i;
    if (PyInt_Check(field)) {
        i = PyInt_AS_LONG(field);
        if (i >= self->keyCount)
            i = -1;
        return i;
    } else if (PyString_Check(field)) {
        /* convert to upper, first check */
        field = f_PyStringToUpper(field);
        posO = PyDict_GetItem(self->keyDict, field);
        Py_DECREF(field);
        if (posO == NULL)
            return -1;
        else
            return PyInt_AS_LONG(posO);
    }
    return -1;
}

/* check if file is open */ 
#pragma inline(f_isOpen)
static int
f_isOpen(File400Object *self)
{
    if (!self->fp) {
        PyErr_SetString(file400Error, "File not open.");
        return 0;
    }
    return 1;
}

/* get key length and update key buffer */
static int
f_keylen(File400Object *self, PyObject *key, char *keyval)
{
    int i, keyLen, keyCnt;
    /* The key can be None (all keys) */
    /* or an integer with number of keys to search */
    if (PyInt_Check(key)) {
        /* find key length */
        keyCnt = PyInt_AS_LONG(key);
        if (keyCnt > self->keyCount) {
            PyErr_SetString(file400Error, "Too many key fields.");
            return -1;
        }
        keyLen = 0;
        for (i = 0; i < keyCnt; i++)
            keyLen += self->keyArr[i].len;
    } else if (keyval != NULL && PySequence_Check(key)) {
        PyObject *ko;
        /* check number of key fields */
        keyCnt = PySequence_Length(key);
        if (keyCnt > self->keyCount) {
            PyErr_SetString(file400Error, "Too many key fields.");
            return -1;
        }
        /* put key values  */
        keyLen = 0;
        for (i = 0; i < keyCnt; i++) {
            ko = PySequence_GetItem(key, i);
            if (f_setKeyValue(self, i, ko))
                return -1;          
            Py_DECREF(ko);
            keyLen += self->keyArr[i].len;
        }
        memcpy(keyval, self->fp->riofb.key, self->keyLen);
        self->keyset = 1;
    } else {
        PyErr_SetString(file400Error, "Key not valid.");
        return -1;
    }
    return keyLen;  
}

/* creates object from class or type */
static PyObject *
f_createObject(PyObject *cls)
{
    if (PyClass_Check(cls)) {
        return PyInstance_NewRaw(cls, NULL);
    } else {
        PyObject *obj, *args, *kwds;
        args = PyTuple_New(0);
        kwds = PyDict_New();
        obj = ((PyTypeObject *)cls)->tp_new((PyTypeObject *)cls, args, kwds);
        Py_DECREF(args);
        Py_DECREF(kwds);
        return obj;
    }
}

/* File400 methods */

static void
File400_dealloc(File400Object *self)
{
    if (self->fp)
        _Rclose(self->fp);
    if (self->qry) {
        char buf[80];
        sprintf(buf, "clof %s", self->file);
        f_system(buf);
        sprintf(buf, "dltovr %s", self->file);
        f_system(buf);
    }
    if (self->recbuf) PyMem_Free(self->recbuf);
    if (self->keybuf) PyMem_Free(self->keybuf);
    if (self->fieldArr) PyMem_Free(self->fieldArr);
    if (self->keyArr) PyMem_Free(self->keyArr);
    Py_XDECREF(self->fieldDict);
    Py_XDECREF(self->keyDict);
    PyObject_Del(self);
}

static char open_doc[] =
"f.open([mode, options, mbr]) -> None\n\
\n\
Opens the file.  The mode can be 'r' for reading (default),\n\
'a' for appending and 'r+' for read, update and append.\n\
The options shold be a string with this format: 'arrseq=Y, secure=Y'.\n\
Options:\n\ 
arrseq  - Arrival sequence. (Y, N(default))\n\
blkrcd  - Preforms record blocking. (Y(default), N)\n\
ccsid   - Specifies the CCSID. Default is 0 (the job CCSID is used.)\n\
commit  - Open under commitment control. (Y, N(default)).\n\
secure  - Secure from overrides. (Y, N(default)).\n\
You can sepcify a member to open."; 

static PyObject*
File400_open(File400Object *self, PyObject *args, PyObject *keywds)
{
    char openKeyw[100], *smode;
    char fullName[35];
    char *mbr = NULL;
    static char *kwlist[] = {"mode", "options", "mbr", NULL};
    PyObject *mode = Py_None, *keyw = Py_None;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OOs:File400", kwlist, 
                                      &mode, &keyw, &mbr))
        return NULL;
    if (self->fp) {
        PyErr_SetString(file400Error, "File already opened.");
        return NULL;
    }
    /* mode */
    if (mode == Py_None) {
        self->omode = F4_OREAD;
        strcpy(openKeyw, "rr");
    } else {
        if (!PyString_Check(mode)) {
            PyErr_SetString(file400Error, "Open mode not a valid type.");
            return NULL;
        }
        smode = PyString_AS_STRING(mode);
        if (!strcmp(smode, "r")) {
            strcpy(openKeyw, "rr");
            self->omode = F4_OREAD;
        } else if (!strcmp(smode, "a")) {
            strcpy(openKeyw, "ar");
            self->omode = F4_OWRITE;
        } else if (!strcmp(smode, "r+")) {
            strcpy(openKeyw, "rr+");
            self->omode = F4_OUPDATE;
        } else {
            PyErr_SetString(file400Error, "Open mode not valid.");
            return NULL;
        }
    }
    /* more options */
    if (keyw != Py_None) {
        if (!PyString_Check(keyw)) {
            PyErr_SetString(file400Error, "Keyword parm not valid type.");
            return NULL;
        }
        strcat(openKeyw, ",");
        strcat(openKeyw, PyString_AS_STRING(keyw));
    }
    /* block is default with read */
    if (self->omode == F4_OREAD && !strstr(openKeyw, "blkrcd"))
        strcat(openKeyw, ", blkrcd=Y");
    /* member */
    if (mbr)
        strcpy(self->mbr, mbr);
    /* build file name to open */
    sprintf(fullName, "%s/%s(%s)", self->lib, self->file, self->mbr);
    /* open */
    self->fp = _Ropen(fullName, openKeyw);
    if (!self->fp) {
        PyErr_SetString(file400Error, "Open failed.");
        return NULL;
    }
    /* check record length */
    if (self->fp->buf_length != self->recLen) {
        PyErr_SetString(file400Error, "Record size check failed.");
        return NULL;
    }
    /* clear the record */
    if (self->omode == F4_OWRITE)
        f_clear(self, (char *)(*(self->fp->out_buf)));
    else {
        /* position to before first will also initialize iofb */
        _Rlocate(self->fp, NULL, 0, __START);
        f_clear(self, (char *)(*(self->fp->in_buf)));
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static char close_doc[] =
"f.close() -> None.\n\
\n\
Close the file.";

static PyObject *
File400_close(File400Object *self, PyObject *args)
{
    char buf[80];
    if (!PyArg_ParseTuple(args, ":close"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (_Rclose(self->fp)) {
        PyErr_SetString(file400Error, "Error closing file.");
        return NULL;
    }
    if (self->qry) {
        sprintf(buf, "clof %s", self->file);
        f_system(buf);
        sprintf(buf, "dltovr %s", self->file);
        f_system(buf);
    }
    self->fp = NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static char posb_doc[] =
"f.posb(key) -> returns 1 - if key found otherwise 0.\n\
\n\
Positions before the first record that has a key equal to the specified key.\n\
The key value can be a number that says number of keyfields\n\
to use from the key buffer, or it can be a sequence of key values.";

static PyObject *
File400_posb(File400Object *self, PyObject *args)
{
    int  keyLen, found = 0;
    PyObject *key;

    if (!PyArg_ParseTuple(args, "O:posb", &key))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    keyLen = f_keylen(self, key, self->keybuf);
    if (keyLen < 0)
        return NULL;
    errno = 0;
    _Rlocate(self->fp, self->fp->riofb.key, keyLen, 
                     __KEY_EQ|__PRIOR|__NO_LOCK);   
    if (self->fp->riofb.num_bytes == 0) {
        _Rlocate(self->fp, self->fp->riofb.key, keyLen, 
                     __KEY_GT|__PRIOR|__NO_LOCK);   
        if (self->fp->riofb.num_bytes == 0)
            _Rlocate(self->fp, NULL, 0, __END); 
    } else
        found = 1;
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->fp->riofb.blk_count = 0;
    return PyInt_FromLong(found);
}

static char posa_doc[] =
"f.posa(key) -> None.\n\
\n\
Positions after the last record that has a key equal to the specified key.\n\
The key value can be a number that says number of keyfields\n\
to use from the key buffer, or it can be a sequence of key values.";

static PyObject *
File400_posa(File400Object *self, PyObject *args)
{
    int  keyLen;
    PyObject *key;
    
    if (!PyArg_ParseTuple(args, "O:posa", &key))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    keyLen = f_keylen(self, key ,self->keybuf);
    if (keyLen < 0)
        return NULL;
    errno = 0;
    _Rlocate(self->fp, self->fp->riofb.key, keyLen, 
                     __KEY_GT|__PRIOR|__NO_LOCK);   
    if (self->fp->riofb.num_bytes == 0)
        _Rlocate(self->fp, NULL, 0, __END); 
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->fp->riofb.blk_count = 0;
    Py_INCREF(Py_None);
    return Py_None;
}

static char posf_doc[] =
"f.posf() -> None.\n\
\n\
Positions before the first record.";

static PyObject *
File400_posf(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":posf"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    errno = 0;
    _Rlocate(self->fp, NULL, 0, __START);   
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->fp->riofb.blk_count = 0;
    Py_INCREF(Py_None);
    return Py_None;
}

static char posl_doc[] =
"f.posl() -> None.\n\
\n\
Positions after the last record.";

static PyObject *
File400_posl(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":posl"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    errno = 0;
    _Rlocate(self->fp, NULL, 0, __END); 
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->fp->riofb.blk_count = 0;
    Py_INCREF(Py_None);
    return Py_None;
}

static char undelete_doc[] =
"f.undelete(record number) -> returns 0 if ok, otherwise 1.\n\
\n\
Undelete record (does not work as intended).";

static PyObject *
File400_undelete(File400Object *self, PyObject *args)
{
    int  rrn, lock = 1, lockOpt = __DFT;

    if (!PyArg_ParseTuple(args, "i:undelete", &rrn))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (self->omode != F4_OUPDATE) {
        PyErr_SetString(file400Error, "File not opened for update.");
        return NULL;
    }
    _Rlocate(self->fp, NULL, rrn, __RRN_EQ);    
    if (self->fp->riofb.num_bytes == 1) {
        _Rupdate(self->fp, NULL, 0);
        return PyInt_FromLong(0);
    }
    return PyInt_FromLong(1);
}

static char readrrn_doc[] =
"f.readrrn(rrn[lock]) -> 0 (found), 1(not found).\n\
\n\
Read by relative record number.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readrrn(File400Object *self, PyObject *args)
{
    int rrn, keyOpt, lock = 1, lockOpt = __DFT;
    PyObject *key = Py_None;

    if (!PyArg_ParseTuple(args, "i|i:readrrn", &rrn, &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    _Rreadd(self->fp, NULL, 0, lockOpt, rrn);
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readeq_doc[] =
"f.readeq(key[lock]) -> 0 (found), 1(not found).\n\
\n\
Read first record equal with the specified key.\n\
The key value can be a number that says number of keyfields\n\
to use from the key buffer, or it can be a sequence of key values.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readeq(File400Object *self, PyObject *args)
{
    int keyLen, keyOpt, lock = 1, lockOpt = __DFT;
    PyObject *key = Py_None;

    if (!PyArg_ParseTuple(args, "O|i:readeq", &key, &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    keyLen = f_keylen(self, key ,self->keybuf);
    if (keyLen < 0)
        return NULL;
    keyOpt = (lockOpt == __DFT) ? __KEY_EQ : __KEY_EQ | lockOpt;  
    _Rreadk(self->fp, NULL, 0,
            keyOpt, self->fp->riofb.key, keyLen);   
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readn_doc[] =
"f.readn([lock]) -> 0 (found), 1(not found).\n\
\n\
Read next record into the buffer.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readn(File400Object *self, PyObject *args)
{
    int lock = 1, lockOpt = __DFT;

    if (!PyArg_ParseTuple(args, "|i:readn", &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    /* locate if block count and last read was previous */
    if (self->fp->riofb.blk_filled_by == __READ_PREV &&
        self->fp->riofb.blk_count > 0) {
        PyErr_SetString(file400Error, "Not valid to reverse the read sequence without new position operation, when in block mode.");
        return NULL;
        /*_Rupfb(self->fp);
        _Rlocate(self->fp, NULL, self->fp->riofb.rrn,
                 __RRN_EQ|__NO_LOCK);   */
    }
    _Rreadn(self->fp, NULL, 0, lockOpt);
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readne_doc[] =
"f.readne(key[lock]) -> 0 (found), 1(not found).\n\
\n\
Read next record if equal with the specified key.\n\
The key should normally be a number that says number of keyfields\n\
to use from the key buffer, or it can be a sequence of key values.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readne(File400Object *self, PyObject *args)
{
    int keyLen, lock = 1, lockOpt = __DFT;
    PyObject *key = Py_None;

    if (!PyArg_ParseTuple(args, "O|i:readne", &key, &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    keyLen = f_keylen(self, key ,self->keybuf);
    if (keyLen < 0)
        return NULL;
    /* read next and check key */
    if (!self->keyset)
        memcpy(self->keybuf, self->fp->riofb.key, self->keyLen);
    memcpy(self->recbuf, (char *)(*(self->fp->in_buf)), self->recLen);
    /* locate if block count and last read was previous */
    if (self->fp->riofb.blk_filled_by == __READ_PREV &&
        self->fp->riofb.blk_count > 0) {
        PyErr_SetString(file400Error, "Not valid to reverse read sequence without new position when in block mode.");
        return NULL;
    }
    _Rreadn(self->fp, NULL, 0, lockOpt);
    if (memcmp(self->keybuf, self->fp->riofb.key, keyLen)) {
        memcpy((char *)(*(self->fp->in_buf)), self->recbuf, self->recLen);
        self->fp->riofb.num_bytes = 0;
        if (self->omode == F4_OUPDATE && lockOpt == __DFT)
            _Rrlslck(self->fp);
    }
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readpe_doc[] =
"f.readpe(key[lock]) -> 0 (found), 1(not found).\n\
\n\
Read previous record if equal with the specified key.\n\
The key should normally be a number that says number of keyfields\n\
to use from the key buffer, or it can be a sequence of key values.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readpe(File400Object *self, PyObject *args)
{
    int keyLen, lock = 1, lockOpt = __DFT;
    PyObject *key = Py_None;

    if (!PyArg_ParseTuple(args, "|Oi:readpe", &key, &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    keyLen = f_keylen(self, key ,self->keybuf);
    if (keyLen < 0)
        return NULL;
    /* read previous and check key */
    if (!self->keyset)
        memcpy(self->keybuf, self->fp->riofb.key, self->keyLen);
    memcpy(self->recbuf, (char *)(*(self->fp->in_buf)), self->recLen);
    /* locate if block count and last read was previous */
    if (self->fp->riofb.blk_filled_by == __READ_NEXT &&
        self->fp->riofb.blk_count > 0) {
        PyErr_SetString(file400Error, "Not valid to reverse read sequence without new position when in block mode.");
        return NULL;
    }
    _Rreadp(self->fp, NULL, 0, lockOpt);
    if (memcmp(self->keybuf, self->fp->riofb.key, keyLen)) {
        memcpy((char *)(*(self->fp->in_buf)), self->recbuf, self->recLen);
        self->fp->riofb.num_bytes = 0;
        if (self->omode == F4_OUPDATE && lockOpt == __DFT)
            _Rrlslck(self->fp);
    }
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readp_doc[] =
"f.readp([lock]) -> 0 (found), 1(not found).\n\
\n\
Read previous record into the buffer.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readp(File400Object *self, PyObject *args)
{
    int lock = 1, lockOpt = __DFT;
    if (!PyArg_ParseTuple(args, "|i:readp", &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    /* locate if block count and last read was previous */
    if (self->fp->riofb.blk_filled_by == __READ_NEXT &&
        self->fp->riofb.blk_count > 0) {
        PyErr_SetString(file400Error, "Not valid to reverse the read sequence without new position operation, when in block mode.");
        return NULL;
    }
    _Rreadp(self->fp, NULL, 0, lockOpt);
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readf_doc[] =
"f.readf([lock]) -> 0 (found), 1(not found).\n\
\n\
Read first record into the buffer.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readf(File400Object *self, PyObject *args)
{
    int lock = 1, lockOpt = __DFT;
    if (!PyArg_ParseTuple(args, "|i:readf", &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    _Rreadf(self->fp, NULL, 0, lockOpt);
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char readl_doc[] =
"f.readl([lock]) -> 0 (found), 1(not found).\n\
\n\
Read last record into the buffer.\n\
Lock (for mode 'r+'). 1 - lock(default) 0 - no lock.";

static PyObject *
File400_readl(File400Object *self, PyObject *args)
{
    int lock = 1, lockOpt = __DFT;
    if (!PyArg_ParseTuple(args, "|i:readl", &lock))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (lock == 0) {
        lockOpt = __NO_LOCK;
    }
    errno = 0;
    _Rreadl(self->fp, NULL, 0, lockOpt);
    if (errno != 0 && errno != EIORECERR) {     
        PyErr_SetString(file400Error, strerror(errno));
        return NULL;
    }
    self->keyset = 0;
    return PyInt_FromLong((self->fp->riofb.num_bytes < self->recLen));
}

static char write_doc[] =
"f.write() -> None.\n\
\n\
Appends the buffer to the file.";

static PyObject *
File400_write(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":write"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (self->omode == F4_OREAD) {
        PyErr_SetString(file400Error, "File not opened for write.");
        return NULL;
    }
    /* save the key and restore it after write */
    if (self->keyLen > 0)
        memcpy(self->keybuf, self->fp->riofb.key, self->keyLen);
    /* copy from input buffer if not opened for append */
    if (self->omode != F4_OWRITE)
        memcpy((char *)(*(self->fp->out_buf)), (char *)(*(self->fp->in_buf)), self->recLen);
    _Rwrite(self->fp, NULL, self->recLen);
    if (self->fp->riofb.num_bytes < self->recLen) {
        PyErr_SetString(file400Error, "Write failed.");
        return NULL;
    }
    if (self->keyLen > 0)
        memcpy(self->fp->riofb.key, self->keybuf, self->keyLen);
    Py_INCREF(Py_None);
    return Py_None;
}

static char delete_doc[] =
"f.delete() -> None.\n\
\n\
Deletes the currently locked record with the content of the buffer.";

static PyObject *
File400_delete(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":delete"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (self->omode != F4_OUPDATE) {
        PyErr_SetString(file400Error, "File not opened for update.");
        return NULL;
    }
    /* save the key and restore it after delete */
    if (self->keyLen > 0)
        memcpy(self->keybuf, self->fp->riofb.key, self->keyLen);
    _Rdelete(self->fp);
    if (self->fp->riofb.num_bytes == 0) {
        PyErr_SetString(file400Error, "Delete failed.");
        return NULL;
    }
    if (self->keyLen > 0)
        memcpy(self->fp->riofb.key, self->keybuf, self->keyLen);
    Py_INCREF(Py_None);
    return Py_None;
}

static char update_doc[] =
"f.update() -> None.\n\
\n\
Updates the currently locked record with the content of the buffer.";

static PyObject *
File400_update(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":update"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (self->omode != F4_OUPDATE) {
        PyErr_SetString(file400Error, "File not opened for update.");
        return NULL;
    }
    /* save the key and restore it after update */
    if (self->keyLen > 0)
        memcpy(self->keybuf, self->fp->riofb.key, self->keyLen);
    _Rupdate(self->fp, NULL, self->recLen);
    if (self->fp->riofb.num_bytes < self->recLen) {
        PyErr_SetString(file400Error, "Update failed.");
        return NULL;
    }
    if (self->keyLen > 0)
        memcpy(self->fp->riofb.key, self->keybuf, self->keyLen);
    Py_INCREF(Py_None);
    return Py_None;
}

static char rlsLock_doc[] =
"f.rlsLock() -> None.\n\
\n\
Releases the lock on the currently locked record.";

static PyObject *
File400_rlsLock(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":rlsLock"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    _Rrlslck(self->fp);
    Py_INCREF(Py_None);
    return Py_None;
}

static char get_doc[] =
"f.get([fields][output][cls][labels]) -> List or Value.\n\
\n\
Get information from the buffer.\n\
f.get() - Get list of all values.\n\
f.get('cusno') - Value of the field cusno. Same as f['cusno']\n\
f.get(0) - Value of the first field. Same as f[0]\n\
f.get(('cusno','name')) - Returns list of values.\n\  
f.get((0,1)) - Returns list of values.\n\
output can have the following values:\n\
 0 - output as a list(default)\n\
 1 - output as a object.\n\
 2 - output as a dictionary.\n\
cls is class of object if output object is selected.\n\
labels could be a tuple of labels to use insted of fieldnames\n\
if output is dictionary.";


static PyObject *
File400_get(File400Object *self, PyObject *args, PyObject *keywds)
{
    PyObject *o = Py_None, *labels = Py_None, *cls = Py_None,
        *fo, *va, *obj, *dict;
    int i, pos, len, output = LIST;
    static char *kwlist[] = {"fields","output","cls","labels", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OiOO:get", kwlist,
                                     &o, &output, &cls, &labels))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (!cls || cls == Py_None)
        cls = dummyClass;
    else if(!PyClass_Check(cls) && !PyType_Check(cls)) {
        PyErr_SetString(file400Error, "cls not a valid type.");
        return NULL;
    }
    /* if none, return all fields*/ 
    if (o == NULL || o == Py_None) {
        if (output == LIST) {
            obj = PyList_New(self->fieldCount);
            for (i = 0; i < self->fieldCount; i++) {
                PyList_SetItem(obj, i, f_getFieldValue(self, i, 
                   (unsigned char *)(*(self->fp->in_buf)))); 
            }
        } else {
            char *label;
            if (output == OBJ) {
                obj = f_createObject(cls);
                if(obj == NULL) {
                    PyErr_SetString(file400Error, "Could not create object.");
                    return NULL;
                }
                dict = PyObject_GetAttrString(obj, "__dict__");
                if(dict == NULL) {
                    PyErr_SetString(file400Error, "Can not get objects __dict__.");
                    return NULL;
                }
                Py_DECREF(dict);
            } else
                obj = dict = PyDict_New();
            for (i = 0; i < self->fieldCount; i++) {                
                if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                    label = PyString_AsString(PyTuple_GetItem(labels, i));
                } else
                    label = self->fieldArr[i].name;
                PyDict_SetItemString(dict, label, f_getFieldValue(self, 
                    i,(unsigned char *)(*(self->fp->in_buf)))); 
            }
        }
        return obj;
    }
    /* one value */
    else if (PyInt_Check(o) || PyString_Check(o)) {
        pos = f_getFieldPos(self, o);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else
            return f_getFieldValue(self, pos, (unsigned char *)(*(self->fp->in_buf)));
    }
    /* if tuple get specified fields */
    else if (PySequence_Check(o)) {
        char *label;
        len = PySequence_Length(o);
        if (output == LIST)
            obj = PyList_New(len);
        else {
            if (output == OBJ) {
                obj = f_createObject(cls);
                if(obj == NULL) {
                    PyErr_SetString(file400Error, "Could not create object.");
                    return NULL;
                }
                dict = PyObject_GetAttrString(obj, "__dict__");
                if(dict == NULL) {
                    PyErr_SetString(file400Error, "Object has no __dict__.");
                    return NULL;
                }
                Py_DECREF(dict);
            } else
                obj = dict = PyDict_New();
        }
        for (i = 0; i < len; i++) {
            fo = PySequence_GetItem(o, i);
            pos = f_getFieldPos(self, fo);
            Py_DECREF(fo);
            if (pos < 0 ) {
                PyErr_SetString(file400Error, "Field not valid.");
                Py_DECREF(obj);
                return NULL;        
            } else {
                if (output == LIST)
                    PyList_SetItem(obj, i, f_getFieldValue(self, pos, (unsigned char *)(*(self->fp->in_buf)))); 
                else {
                    if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                        label = PyString_AsString(PyTuple_GetItem(labels, i));
                    } else 
                        label = self->fieldArr[pos].name;
                    PyDict_SetItemString(dict, label,
                       f_getFieldValue(self, pos, (unsigned char *)(*(self->fp->in_buf)))); 
                }
            }
        }
        return obj;
    } 
}

static char clear_doc[] =
"f.clear() -> None.\n\
\n\
Clear the record buffer.\n\
Numeric fields are set to 0 and chars to blank.";

static PyObject *
File400_clear(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":clear"))
        return NULL;
    if (!f_isOpen(self))
        return NULL;
    /* fill the record with blanks */
    if (self->omode == F4_OWRITE)
        f_clear(self, (char *)(*(self->fp->out_buf)));
    else
        f_clear(self, (char *)(*(self->fp->in_buf)));
    Py_INCREF(Py_None);
    return Py_None;
}

static char getKey_doc[] =
"f.getKey([sequence/value]) -> List or Value.\n\
\n\
Get information from the key buffer.\n\
f.getKey() - Get list of key values.\n\
f.getKey('cusno') - Value of the key field cusno.\n\
f.getKey(0) - Value of the first key field.\n\
f.getKey(('cusno','name')) - Returns list of values.\n\  
f.getKey((0,1)) - Returns list of values.";

static PyObject *
File400_getKey(File400Object *self, PyObject *args)
{
    PyObject *o = Py_None, *fo, *va, *list;
    int i, pos, len;

    if (!PyArg_ParseTuple(args, "|O:get", &o))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (self->keyCount <= 0) {
        PyErr_SetString(file400Error, "No key exists.");
        return NULL;
    }
    /* if none, return all key values*/ 
    if (o == Py_None) {
        list = PyList_New(self->keyCount);
        for (i = 0; i < self->keyCount; i++) {
            PyList_SetItem(list, i, f_getKeyValue(self, i)); 
        }
        return list;
    }
    /* one value */
    else if (PyInt_Check(o) || PyString_Check(o)) {
        pos = f_getKeyPos(self, o);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else
            return f_getKeyValue(self, pos);
    } 
    /* if sequence get specified key values */
    else if (PySequence_Check(o)) {
        len = PySequence_Length(o);
        list = PyList_New(len);
        for (i = 0; i < len; i++) {
            fo = PySequence_GetItem(o, i);
            pos = f_getKeyPos(self, fo);
            Py_DECREF(fo);
            if (pos < 0 ) {
                PyErr_SetString(file400Error, "Key not valid.");
                Py_DECREF(list);
                return NULL;        
            } else {
                PyList_SetItem(list, i, f_getKeyValue(self, pos)); 
            }
        }
        return list;
    }
}

static char getRrn_doc[] =
"f.getRrn() -> Long.\n\
\n\
Get the current record number.";

static PyObject *
File400_getRrn(File400Object *self, PyObject *args)
{
    PyObject o;
    if (!PyArg_ParseTuple(args, ":getRrn"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    return PyInt_FromLong(self->fp->riofb.rrn);
}

static char getString_doc[] =
"f.getString([fields][fieldsep][decsep]) -> String.\n\
\n\
Get the field values as string seperated by tab.\n\
f.getString() - All fields.\n\
f.getString('cusno') - One field.\n\
f.getString(0) - One field by psoition in record.\n\
f.getString(('cusno','name')) - List of fields.\n\  
f.getString((0,1)) - List of fields.\n\
Default value for fieldsep(field seperator) is tab(\t).\n\
Default value for decsep(decimal seperator) is .(periode).";

static PyObject *
File400_getString(File400Object *self, PyObject *args, PyObject *keywds)
{
    PyObject *fields = Py_None;
    char decsep = '.';
    char fieldsep = '\t';
    static char *kwlist[] = {"fields","fieldsep","decsep", NULL};
    char *fb = (unsigned char *)(*(self->fp->in_buf));

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Occ:getString", kwlist,
                                     &fields, &fieldsep, &decsep))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    /* one value */
    if (PyInt_Check(fields) || PyString_Check(fields)) {
        int pos = f_getFieldPos(self, fields);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else
            return f_getFieldString(self, pos, decsep, fb);
    } else {
        char *buf;
        int i, len = 0, size = 4000;
        PyObject *newstr, *fo;
        newstr = PyString_FromStringAndSize(NULL, size);
        buf = PyString_AsString(newstr);
        
        /* if none, return all fields*/ 
        if (fields == NULL || fields == Py_None) {
            for (i = 0; i < self->fieldCount; i++) {
                if (self->fieldArr[i].len > size - len - 50) {
                    size += self->fieldArr[i].len;
                    _PyString_Resize(&newstr, size);
                }
                len += f_addToString(buf + len, fb + self->fieldArr[i].offset,
                                     self->fieldArr[i].type, 
                                     self->fieldArr[i].len,
                                     self->fieldArr[i].digits,
                                     self->fieldArr[i].dec, decsep);                
                *(buf + len) = fieldsep;
                len++;
            }
        }
        /* if tuple get specified fields */
        else if (PySequence_Check(fields)) {
            int pos, seqlen = PySequence_Length(fields);
            for (i = 0; i < seqlen; i++) {
                fo = PySequence_GetItem(fields, i);
                pos = f_getFieldPos(self, fo);
                Py_DECREF(fo);
                if (pos < 0 ) {
                    PyErr_SetString(file400Error, "Field not valid.");
                    return NULL;        
                } else {
                    if (self->fieldArr[pos].len > size - len - 50) {
                        size += self->fieldArr[pos].len;
                        _PyString_Resize(&newstr, size);
                    }
                    len += f_addToString(buf + len, fb + self->fieldArr[pos].offset,
                                        self->fieldArr[pos].type, 
                                        self->fieldArr[pos].len,
                                        self->fieldArr[pos].digits,
                                        self->fieldArr[pos].dec, decsep);
                    *(buf + len) = fieldsep;
                    len++;
                }
            }
        } else {
            PyErr_SetString(file400Error, "Field not valid.");
            return NULL;        
        }
        _PyString_Resize(&newstr, len);
        return newstr;
    }
}

static char getXml_doc[] =
"f.getXml([fields][labels][decsep][output][tag][cls]) -> String.\n\
\n\
Get the field values as a xml string.\n\
f.getXml() - All fields.\n\
f.getXml('cusno') - One field.\n\
f.getXml(0) - One field by position in record.\n\
f.getXml(('cusno','name')) - List of fields.\n\  
f.getXml((0,1)) - List of fields.\n\
f.getXml((0,1),('pn','desc')) - Include labels to use.\n\
Default value for decsep(decimal seperator) is .(periode).\n\
output can have the following values:\n\
 0 - output as a tuple(default)\n\
 1 - output as an object\n\
 2 - output as a dictionary\n\
If tag = 0, not tag for the tuple/obj/dict is generated.\n\
if output = 1 and tag = 1, cls would be the class of object";

static PyObject *
File400_getXml(File400Object *self, PyObject *args, PyObject *keywds)
{
    PyObject *labels = Py_None;
    PyObject *fields = Py_None;
    PyObject *cls=NULL, *newstr, *fo;
    char decsep = '.';
    static char *kwlist[] = {"fields","labels","decsep","output","tag","cls", NULL};
    char *fb = (unsigned char *)(*(self->fp->in_buf));
    char *buf, *label;
    int i, len = 0, size = 4000, output = LIST, tag = 0;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OOciiO:getXml", kwlist,
                                     &fields, &labels, &decsep, &output, &tag, &cls))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    
    newstr = PyString_FromStringAndSize(NULL, size);
    buf = PyString_AsString(newstr);
    /* one value */
    if (PyInt_Check(fields) || PyString_Check(fields)) {
        int pos = f_getFieldPos(self, fields);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else {
            if (PyString_Check(labels))
                label = PyString_AsString(labels);
            else
                label = self->fieldArr[pos].name;
            if (self->fieldArr[pos].len > size - len - strlen(label) - 100) {
                size += self->fieldArr[pos].len;
                _PyString_Resize(&newstr, size);
            }
            len += f_addToXml(buf + len, label, fb + self->fieldArr[pos].offset,
                              self->fieldArr[pos].type, 
                              self->fieldArr[pos].len,
                              self->fieldArr[pos].digits,
                              self->fieldArr[pos].dec, decsep, output);
        }
    } else {
        if (tag) {
            if (output == LIST)
                len += strlen(strcpy(buf + len, "<rec t=\"T\">"));
            else if (output == OBJ) {
                if (cls && PyClass_Check(cls)) {
                    PyObject *mod;
                    PyClassObject *class = (PyClassObject *)cls;
                    len += strlen(strcpy(buf + len, "<rec t=\"O\" c=\""));
                    len += strlen(strcpy(buf + len, PyString_AsString(class->cl_name)));
                    mod = PyDict_GetItemString(class->cl_dict, "__module__");
                    if (mod) {
                        len += strlen(strcpy(buf + len, "\" m=\""));
                        len += strlen(strcpy(buf + len, PyString_AsString(mod)));
                    }
                    len += strlen(strcpy(buf + len, "\">"));
                } else
                    len += strlen(strcpy(buf + len, "<rec t=\"O\">"));
            } else if (output == DICT)
                len += strlen(strcpy(buf + len, "<rec t=\"D\">"));
        }
        /* if none, return all fields*/ 
        if (fields == NULL || fields == Py_None) {
            for (i = 0; i < self->fieldCount; i++) {
                /* get label*/
                if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                    label = PyString_AsString(PyTuple_GetItem(labels, i));
                } else
                    label = self->fieldArr[i].name;
                if (self->fieldArr[i].len > size - len - strlen(label) - 100) {
                    size += self->fieldArr[i].len;
                    _PyString_Resize(&newstr, size);
                }
                len += f_addToXml(buf + len, label, fb + self->fieldArr[i].offset,
                                  self->fieldArr[i].type, 
                                  self->fieldArr[i].len,
                                  self->fieldArr[i].digits,
                                  self->fieldArr[i].dec, decsep, output);
            }
        }
        /* if tuple get specified fields */
        else if (PySequence_Check(fields)) {
            int pos, seqlen = PySequence_Length(fields);
            for (i = 0; i < seqlen; i++) {
                fo = PySequence_GetItem(fields, i);
                pos = f_getFieldPos(self, fo);
                Py_DECREF(fo);
                if (pos < 0 ) {
                    PyErr_SetString(file400Error, "Field not valid.");
                    return NULL;        
                } else {
                    /* get label*/
                    if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                        label = PyString_AsString(PyTuple_GetItem(labels, i));
                    } else
                        label = self->fieldArr[pos].name;
                    if (self->fieldArr[pos].len > size - len - strlen(label) - 100) {
                        size += self->fieldArr[pos].len;
                        _PyString_Resize(&newstr, size);
                    }
                    len += f_addToXml(buf + len, label, fb + self->fieldArr[pos].offset,
                                      self->fieldArr[pos].type, 
                                      self->fieldArr[pos].len,
                                      self->fieldArr[pos].digits,
                                      self->fieldArr[pos].dec, decsep, output);
                }
            }
        } else {
            PyErr_SetString(file400Error, "Fields not valid.");
            return NULL;        
        }
        if (tag)
            len += strlen(strcpy(buf + len, "</rec>"));
    }
    _PyString_Resize(&newstr, len);
    return newstr;
}

static char isOpen_doc[] =
"f.isOpen() -> Int.\n\
\n\
Returns 1 if open otherwise 0.";

static PyObject *
File400_isOpen(File400Object *self, PyObject *args)
{
    PyObject o;
    if (!PyArg_ParseTuple(args, ":isOpen"))
        return NULL;
    return PyInt_FromLong((self->fp != NULL));
}

static char mode_doc[] =
"f.mode() -> String.\n\
\n\
Returns open mode.";

static PyObject *
File400_mode(File400Object *self, PyObject *args)
{
    char *mode;
    PyObject o;
    if (!PyArg_ParseTuple(args, ":mode"))
        return NULL;
    if (self->omode == F4_OREAD)
        return PyString_FromString("r");
    else if (self->omode == F4_OUPDATE)
        return PyString_FromString("r+");
    else if (self->omode == F4_OWRITE)
        return PyString_FromString("a");
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static char recordSize_doc[] =
"f.recordSize() -> Long.\n\
\n\
Get the size of the record in bytes.";

static PyObject *
File400_recordSize(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":recordSize"))
        return NULL;
    return PyInt_FromLong(self->recLen); 
}

static char fileName_doc[] =
"f.fileName() -> String.\n\
\n\
Get name of file.";

static PyObject *
File400_fileName(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":fileName"))
        return NULL;
    return PyString_FromString(self->file); 
}

static char libName_doc[] =
"f.libName() -> String.\n\
\n\
Get name of library.";

static PyObject *
File400_libName(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":libName"))
        return NULL;
    return PyString_FromString(self->retLib); 
}

static char mbrName_doc[] =
"f.mbrName() -> String.\n\
\n\
Get name of current member (can be *first).";

static PyObject *
File400_mbrName(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":mbrName"))
        return NULL;
    return PyString_FromString(self->mbr); 
}

static char mbrList_doc[] =
"f.memberList([member]) -> Tuple.\n\
\n\
Returns tuple of member names.\n\
Member can be generic (use an astrix).";

static PyObject *
File400_mbrList(File400Object *self, PyObject *args)
{
    PyObject *mbrs;
    Qus_EC_t error;
    Qus_Generic_Header_0300_t *ushdr;
    char usname[] = "MBRLIST   QTEMP     ";
    char file[21], mbr[11], desc[50];
    char *pus, *pmbr = NULL;
    int i;

    if (!PyArg_ParseTuple(args, "|s:mbrList", &pmbr))
        return NULL;        
    if (pmbr != NULL)
        strtostr400(mbr, pmbr, 10);
    else
        strcpy(mbr, "*ALL      ");
    strtostr400(file, self->file, 10);
    strtostr400((file + 10), self->lib, 10);
    memset(desc, ' ', 50); 
    /* run quslmbr */
    error.Bytes_Provided = sizeof(error);
    QUSCRTUS(usname,"          ", 1, " ", "*USE      ", desc,
             "*YES      ", &error);
    if (error.Bytes_Available > 0) {
        PyErr_SetString(file400Error, "Error when creating userspace.");
        return NULL;
    }
    Py_BEGIN_ALLOW_THREADS
    QUSLMBR(usname, "MBRL0100  ", file, mbr, "0", &error);
    Py_END_ALLOW_THREADS
    if (error.Bytes_Available > 0) {
        PyErr_SetString(file400Error, "Error getting memberlist.");
        return NULL;
    }
    QUSPTRUS(usname, &ushdr);
    mbrs = PyTuple_New(ushdr->Number_List_Entries);
    if (!mbrs) 
        return NULL;
    pus = (char *)ushdr;
    pus += ushdr->Offset_List_Data;
    for (i = 0; i < ushdr->Number_List_Entries; i++) {      
        PyTuple_SetItem(mbrs, i, PyString_FromString(str400tostr(mbr, pus, 10)));
        pus += ushdr->Size_Each_Entry;
    }
    QUSDLTUS(usname, &error);
    return mbrs;
}

static char fieldCount_doc[] =
"f.fieldCount() -> Int.\n\
\n\
Get number of fields.";

static PyObject *
File400_fieldCount(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":fieldCount"))
        return NULL;
    return PyInt_FromLong(self->fieldCount);
}

static char fieldDesc_doc[] =
"f.fieldDesc(Value) -> String.\n\
\n\
Get description for field.\n\
Value can be both number and field name";

static PyObject *
File400_fieldDesc(File400Object *self, PyObject *args)
{
    int pos;
    PyObject *name;
    if (!PyArg_ParseTuple(args, "O:fieldDesc", &name))
        return NULL;
    pos = f_getFieldPos(self, name);
    if (pos == -1) {
        Py_INCREF(Py_None);
        return Py_None;
    } else
        return PyString_FromString(self->fieldArr[pos].desc); 
}

static char fieldSize_doc[] =
"f.fieldSize(Value) -> Int.\n\
\n\
Get field size in bytes.\n\
Value can be both number and field name";

static PyObject *
File400_fieldSize(File400Object *self, PyObject *args)
{
    int pos;
    PyObject *name;
    if (!PyArg_ParseTuple(args, "O:fieldSize", &name))
        return NULL;
    pos = f_getFieldPos(self, name);
    if (pos == -1) {
        Py_INCREF(Py_None);
        return Py_None;
    } else
        return PyInt_FromLong(self->fieldArr[pos].len);
}

static char fieldType_doc[] =
"f.fieldType(Value) -> String.\n\
\n\
Get field type.\n\
Value can be both number and field name";

static PyObject *
File400_fieldType(File400Object *self, PyObject *args)
{
    int pos, itype;
    PyObject *name;
    char *type;
    if (!PyArg_ParseTuple(args, "O:fieldType", &name))
        return NULL;
    pos = f_getFieldPos(self, name);
    if (pos == -1) {
        Py_INCREF(Py_None);
        return Py_None;
    } else {
        itype = self->fieldArr[pos].type;
        if (itype == 0)
            type = "BINARY";
        else if (itype == 1)
            type = "FLOAT";
        else if (itype == 2)
            type = "ZONED";
        else if (itype == 3)
            type = "PACKED";
        else if (itype == 4)
            type = "CHAR";
        else if (itype == 0x8004)
            type = "VARCHAR";
        else if (itype == 5)
            type = "GRAPHIC";
        else if (itype == 6)
            type = "DBCS";
        else if (itype == 0x8005)
            type = "VARGRAPHIC";
        else if (itype == 0x8006)
            type = "VARDBCS";
        else if (itype == 11)
            type = "DATE";
        else if (itype == 12)
            type = "TIME";
        else if (itype == 13)
            type = "TIMESTAMP";
        else if (itype == 0x4004)
            type = "BLOB";
        else if (itype == 0x4005)
            type = "DBCLOB";
        else if (itype == 0x4006)
            type = "CLOBOPEN";
        else if (itype == 0x802c)
            type = "LINKCHAR";
        else if (itype == 0x802e)
            type = "LINKOPEN";          
        else type = "UNKNOWN";
        return PyString_FromString(type);
    }
}

static char fieldList_doc[] =
"f.fieldList([full=0]) -> Tuple.\n\
\n\
If full is False, returns a tuple of field names,\n\
else returns tuples with name, desc, type, size, digits, decimals.";

static PyObject *
File400_fieldList(File400Object *self, PyObject *args)
{
    PyObject *fld, *ftype, *fargs;
    int i, full = 0;
    fieldInfoStruct fi;

    if (!PyArg_ParseTuple(args, "|i:fieldList", &full))
        return NULL;        
    fld = PyTuple_New(self->fieldCount);
    if (!fld) 
        return NULL;
    for (i = 0; i < self->fieldCount; i++) {
        if (full) {
            fi = self->fieldArr[i];
            fargs = Py_BuildValue("(i)", i);
            ftype = File400_fieldType(self, fargs);
            PyTuple_SetItem(fld, i, Py_BuildValue("ssOiii", fi.name, fi.desc,
                            ftype, fi.len, fi.digits, fi.dec));
            Py_DECREF(fargs);
            Py_XDECREF(ftype);
        } else
            PyTuple_SetItem(fld, i, PyString_FromString(self->fieldArr[i].name)); 
    }
    return fld;
}

static char keyCount_doc[] =
"f.keyCount() -> Int.\n\
\n\
Get number of key fields.";

static PyObject *
File400_keyCount(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":keyCount"))
        return NULL;
    return PyInt_FromLong(self->keyCount);
}

static char keyList_doc[] =
"f.keyList() -> Tuple.\n\
\n\
Get a list(tuple) of key field names.";

static PyObject *
File400_keyList(File400Object *self, PyObject *args)
{
    PyObject *key;
    int i;

    if (!PyArg_ParseTuple(args, ":keyList"))
        return NULL;        
    key = PyTuple_New(self->keyCount);
    if (!key) 
        return NULL;
    for (i = 0; i < self->keyCount; i++) {
        PyTuple_SetItem(key, i, PyString_FromString(self->keyArr[i].name)); 
    }
    return key;
}

static char getRecord_doc[] =
"f.getRecord() -> Record400.\n\
\n\
Get a copy of the record object.\n\
If file is not open the record is cleared.";

static PyObject *
File400_getRecord(File400Object *self, PyObject *args)
{
    Record400Object *newrec;

    if (!PyArg_ParseTuple(args, ":getRecord"))
        return NULL;
    newrec = (Record400Object *) PyObject_MALLOC(sizeof(Record400Object) + self->recLen);
    if (newrec == NULL)
        return PyErr_NoMemory();
    PyObject_INIT_VAR(newrec, &Record400_Type, self->recLen);
    Py_INCREF(self);
    newrec->file = self;
    memcpy(newrec->recId, self->recId, sizeof(self->recId));
    if (self->fp) {
        newrec->rrn = self->fp->riofb.rrn;
        memcpy(newrec->buffer, (char *)(*(self->fp->in_buf)), self->recLen);
    } else {
        newrec->rrn = -1;
        f_clear(self, newrec->buffer);
    }
    return (PyObject *) newrec;
}

static char getBuffer_doc[] =
"f.getBuffer() -> String.\n\
\n\
Get the raw value of buffer as a String.\n\
Same as getBuffer() for the record.";

static PyObject *
File400_getBuffer(File400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":getBuffer"))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    return PyString_FromStringAndSize((char *)(*(self->fp->in_buf)), self->recLen);
}

static char set_doc[] =
"f.set(Field, Value) -> String.\n\
\n\
Put value from Field into the buffer.\n\
Field can be both number and field name.\n\
Same as f['CUSNO'] = 123, or f[0] = 123, or f._CUSNO = 123";

static PyObject *
File400_set(File400Object *self, PyObject *args)
{
    PyObject *k, *o;
    int i, pos;

    if (!PyArg_ParseTuple(args, "OO:set", &k, &o))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    /* if dict update fields in dict with values */
    if (PyDict_Check(o)) {
        PyErr_SetString(file400Error, "Not yet supported.");
        return NULL;
    }
    /* one value */
    else {
        pos = f_getFieldPos(self, k);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } 
        else {
            if (self->omode == F4_OWRITE)
                i = f_setFieldValue(self, pos, o, (char *)(*(self->fp->out_buf)));
            else
                i = f_setFieldValue(self, pos, o, (char *)(*(self->fp->in_buf)));
        }
        if (i)
            return NULL;
    } 
    Py_INCREF(Py_None);
    return Py_None;
}

static char setKey_doc[] =
"f.setKey(sequence) -> None.\n\
\n\
Updates the key buffer.";

static PyObject *
File400_setKey(File400Object *self, PyObject *args)
{
    PyObject *k, *v;
    int i, len;

    if (!PyArg_ParseTuple(args, "O:set", &k))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (!PyTuple_Check(k) && !PyList_Check(k)) {
        PyErr_SetString(file400Error, "Key must be a tuple or list with values.");
        return NULL;
    }
    len = PySequence_Length(k);
    if (len > self->keyCount) {
        PyErr_SetString(file400Error, "Too many key fields.");
        return NULL;
    }
    for (i = 0; i < len; i++) {
        v = PySequence_GetItem(k, i);
        if (f_setKeyValue(self, i, v)) {
            Py_DECREF(v);
            return NULL;
        }
        Py_DECREF(v);
    }
    memcpy(self->keybuf, self->fp->riofb.key, self->keyLen);
    self->keyset = 1;
    Py_INCREF(Py_None);
    return Py_None;
}

static char setRecord_doc[] =
"f.setRecord() -> None.\n\
\n\
Put values from record object to record buffer";

static PyObject *
File400_setRecord(File400Object *self, PyObject *args)
{
    Record400Object *o;

    if (!PyArg_ParseTuple(args, "O:setRecord", &o))
        return NULL;    
    if (!f_isOpen(self)) 
        return NULL;
    if (!Record400Object_Check(o)) {
        PyErr_SetString(file400Error, "This is not a record object.");
        return NULL;
    }
    if (strcmp(self->recId, o->recId)) {
        PyErr_SetString(file400Error, "Record format check failed.");
        return NULL;
    }
    if (self->omode == F4_OWRITE)
        memcpy(((char *)(*(self->fp->out_buf))), o->buffer, self->recLen);
    else    
        memcpy(((char *)(*(self->fp->in_buf))), o->buffer, self->recLen);
    Py_INCREF(Py_None);
    return Py_None;
}

static char setBuffer_doc[] =
"f.setBuffer(String) -> None.\n\
\n\
Copy the value of string to the internal buffer.\n\
Use this with caution";

static PyObject *
File400_setBuffer(File400Object *self, PyObject *args)
{
    char *buf;
    PyStringObject *o;

    if (!PyArg_ParseTuple(args, "S:setBuffer", &o))
        return NULL;
    if (!f_isOpen(self)) 
        return NULL;
    if (self->omode == F4_OWRITE)
        buf = (char *)(*(self->fp->out_buf));
    else
        buf = (char *)(*(self->fp->in_buf));
    if (self->recLen > o->ob_size)
        memcpy(buf, o->ob_sval, o->ob_size);
    else
        memcpy(buf, o->ob_sval, self->recLen);
    Py_INCREF(Py_None);
    return Py_None;
}

static int
File400_length(File400Object *self)
{
    return self->fieldCount;
}

static int
File400_ass_subscript(File400Object *self, PyObject *v, PyObject *w)
{
    int pos;
    if (!f_isOpen(self)) 
        return -1;
    if (w == NULL) {
        PyErr_SetString(file400Error, "NULL value not supported.");
        return -1;
    } else {
        pos = f_getFieldPos(self, v);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Field not found.");
            return -1;
        } else {
            if (self->omode == F4_OWRITE)
                return f_setFieldValue(self, pos, w, (char *)(*(self->fp->out_buf)));
            else
                return f_setFieldValue(self, pos, w, (char *)(*(self->fp->in_buf)));
        }
    }
}

static PyObject *
File400_subscript(File400Object *self, PyObject *v)
{
    int pos;
    if (!f_isOpen(self)) 
        return NULL;
    pos = f_getFieldPos(self, v);
    if (pos < 0) {
        PyErr_SetString(file400Error, "Parameter not valid.");
        return NULL;
    } else
        return f_getFieldValue(self, pos, (unsigned char *)(*(self->fp->in_buf)));
}

static PyMappingMethods File400_as_mapping = {
    (inquiry)File400_length, /*mp_length*/
    (binaryfunc)File400_subscript, /*mp_subscript*/
    (objobjargproc)File400_ass_subscript, /*mp_ass_subscript*/
};

static PyMethodDef File400Object_methods[] = {
    {"open",    (PyCFunction)File400_open,  METH_VARARGS|METH_KEYWORDS, open_doc},
    {"close",   (PyCFunction)File400_close, METH_VARARGS,close_doc},
    {"readn",   (PyCFunction)File400_readn, METH_VARARGS, readn_doc},
    {"readp",   (PyCFunction)File400_readp, METH_VARARGS, readp_doc},
    {"readrrn",  (PyCFunction)File400_readrrn,METH_VARARGS, readrrn_doc},
    {"readeq",  (PyCFunction)File400_readeq,METH_VARARGS, readeq_doc},
    {"readne",  (PyCFunction)File400_readne,METH_VARARGS, readne_doc},
    {"readpe",  (PyCFunction)File400_readpe,METH_VARARGS, readpe_doc},
    {"readf",   (PyCFunction)File400_readf, METH_VARARGS, readf_doc},
    {"readl",   (PyCFunction)File400_readl, METH_VARARGS, readl_doc},
    {"posa",   (PyCFunction)File400_posa,   METH_VARARGS, posa_doc},
    {"posb",   (PyCFunction)File400_posb,   METH_VARARGS, posb_doc},
    {"posf",    (PyCFunction)File400_posf,  METH_VARARGS, posf_doc},
    {"posl",    (PyCFunction)File400_posl,  METH_VARARGS, posl_doc},
    {"undelete",  (PyCFunction)File400_undelete,METH_VARARGS, undelete_doc},
    {"write",   (PyCFunction)File400_write, METH_VARARGS, write_doc},
    {"delete",  (PyCFunction)File400_delete,METH_VARARGS, delete_doc},
    {"update",  (PyCFunction)File400_update,METH_VARARGS, update_doc},
    {"rlsLock", (PyCFunction)File400_rlsLock,METH_VARARGS, rlsLock_doc},
    {"clear",   (PyCFunction)File400_clear, METH_VARARGS, clear_doc},
    {"get",     (PyCFunction)File400_get, METH_VARARGS|METH_KEYWORDS, get_doc},
    {"getRecord",(PyCFunction)File400_getRecord, METH_VARARGS, getRecord_doc},
    {"getBuffer",(PyCFunction)File400_getBuffer, METH_VARARGS, getBuffer_doc},
    {"getKey",  (PyCFunction)File400_getKey, METH_VARARGS, getKey_doc},
    {"getRrn",  (PyCFunction)File400_getRrn, METH_VARARGS, getRrn_doc},
    {"getString",(PyCFunction)File400_getString, METH_VARARGS|METH_KEYWORDS, getString_doc},
    {"getXml",(PyCFunction)File400_getXml, METH_VARARGS|METH_KEYWORDS, getXml_doc},
    {"isOpen",  (PyCFunction)File400_isOpen, METH_VARARGS, isOpen_doc},
    {"mode",    (PyCFunction)File400_mode,   METH_VARARGS, mode_doc},
    {"fileName",(PyCFunction)File400_fileName, METH_VARARGS, fileName_doc},
    {"libName", (PyCFunction)File400_libName, METH_VARARGS, libName_doc},
    {"mbrName", (PyCFunction)File400_mbrName, METH_VARARGS, mbrName_doc},
    {"mbrList", (PyCFunction)File400_mbrList, METH_VARARGS, mbrList_doc},
    {"fieldCount",(PyCFunction)File400_fieldCount, METH_VARARGS, fieldCount_doc},
    {"fieldList",(PyCFunction)File400_fieldList, METH_VARARGS, fieldList_doc},
    {"fieldDesc",(PyCFunction)File400_fieldDesc, METH_VARARGS, fieldDesc_doc},
    {"fieldSize",(PyCFunction)File400_fieldSize, METH_VARARGS, fieldSize_doc},
    {"fieldType",(PyCFunction)File400_fieldType, METH_VARARGS, fieldType_doc},
    {"recordSize",(PyCFunction)File400_recordSize, METH_VARARGS, recordSize_doc},
    {"keyCount",(PyCFunction)File400_keyCount, METH_VARARGS, keyCount_doc},
    {"keyList",(PyCFunction)File400_keyList, METH_VARARGS, keyList_doc},
    {"set", (PyCFunction)File400_set, METH_VARARGS, set_doc},
    {"setKey",  (PyCFunction)File400_setKey, METH_VARARGS, setKey_doc},
    {"setRecord",(PyCFunction)File400_setRecord, METH_VARARGS, setRecord_doc},
    {"setBuffer",(PyCFunction)File400_setBuffer, METH_VARARGS, setBuffer_doc},
    {NULL,      NULL}       /* sentinel */
};

static PyObject *
File400_str(File400Object *self)
{
    return PyString_FromString(self->file); 
}

static PyObject *
File400_getattr(File400Object *self, char *name)
{
    PyObject *fieldName, *retval;
    if (*name == '_' && *(name + 1) != '_') {       
        fieldName = PyString_FromString(name + 1);
        retval = File400_subscript(self, fieldName);
        Py_DECREF(fieldName);
        return retval;
    } else
        return Py_FindMethod(File400Object_methods, (PyObject *)self, name);
}

static int
File400_setattr(File400Object *self, char *name, PyObject *v)
{
    PyObject *fieldName;
    int retval;
    if (*name == '_') {
        fieldName = PyString_FromString(name + 1);
        retval = File400_ass_subscript(self, fieldName, v);
        Py_DECREF(fieldName);
        return retval;
    } else
        return -1;
}

PyTypeObject File400_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyObject_HEAD_INIT(NULL)
    0,          /*ob_size*/
    "File400",          /*tp_name*/
    sizeof(File400Object),  /*tp_basicsize*/
    0,          /*tp_itemsize*/
    /* methods */
    (destructor)File400_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    (getattrfunc)File400_getattr,/*tp_getattr*/
    (setattrfunc)File400_setattr,/*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    &File400_as_mapping,    /*tp_as_mapping*/
    0,          /*tp_hash*/
    0,          /*tp_call*/
   (reprfunc)File400_str,   /*tp_str*/
};

/* --------------------------------------------------------------------- */

/* Record400 methods */
static void
Record400_dealloc(Record400Object *self)
{
    Py_XDECREF(self->file);
    PyObject_Del(self);
}

static char rec_getFile_doc[] =
"r.getFile() -> File400 Object.\n\
\n\
Return the File400 object where this record came from.";

static PyObject *
Record400_getFile(Record400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":getFile"))
        return NULL;        
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    Py_INCREF(self->file);
    return (PyObject *)self->file;
}

static char rec_refresh_doc[] =
"r.refresh() -> None.\n\
\n\
Update Record with the values of the record buffer.";

static PyObject *
Record400_refresh(Record400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":refresh"))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    if (!f_isOpen(self->file))
        return NULL;
    self->rrn = self->file->fp->riofb.rrn;
    memcpy(self->buffer, (char *)(*(self->file->fp->in_buf)), self->file->recLen);
    Py_INCREF(Py_None);
    return Py_None;
}

static char rec_get_doc[] =
"r.get([fields][output][cls][labels]) -> List or Values.\n\
\n\
Get information from the buffer.\n\
f.get() - Get list of all values.\n\
f.get('cusno') - Value of the field cusno. Same as f['cusno']\n\
f.get(0) - Value of the first field. Same as f[0]\n\
f.get(('cusno','name')) - Returns list of values.\n\  
f.get((0,1)) - Returns list of values.\n\
output can have the following values:\n\
 0 - output as a list(default)\n\
 1 - output as a object.\n\
 2 - output as a dictionary.\n\
cls is class of object if output object is selected.\n\
labels could be a tuple of labels to use insted of fieldnames\n\
if output is dictionary.";

static PyObject *
Record400_get(Record400Object *self, PyObject *args, PyObject *keywds)
{
    PyObject *o = Py_None, *labels = Py_None, *cls = Py_None, 
        *fo, *va, *obj, *dict;
    int i, pos, len, output = LIST;
    static char *kwlist[] = {"fields","output","cls","labels", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OiOO:get", kwlist,
                                     &o, &output, &cls, &labels))
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    if (!cls || cls == Py_None)
        cls = dummyClass;       
    else if (!PyClass_Check(cls) && !PyType_Check(cls)) { 
        PyErr_SetString(file400Error, "cls not a vlid type.");
        return NULL;
    }
    /* if none, return all fields*/ 
    if (o == NULL || o == Py_None) {
        if (output == LIST) {
            obj = PyList_New(self->file->fieldCount);
            for (i = 0; i < self->file->fieldCount; i++) {
                PyList_SetItem(obj, i, f_getFieldValue(self->file, i, self->buffer)); 
            }
        } else {
            char *label;
            if (output == OBJ) {
                obj = f_createObject(cls);
                if(obj == NULL) {
                    PyErr_SetString(file400Error, "Could not create object.");
                    return NULL;
                }
                dict = PyObject_GetAttrString(obj, "__dict__");
                if(dict == NULL) {
                    PyErr_SetString(file400Error, "Object has no __dict__.");
                    return NULL;
                }
                Py_DECREF(dict);
            } else
                obj = dict = PyDict_New();
            for (i = 0; i < self->file->fieldCount; i++) {
                if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                    label = PyString_AsString(PyTuple_GetItem(labels, i));
                } else
                    label = self->file->fieldArr[i].name;
                PyDict_SetItemString(dict, label,
                  f_getFieldValue(self->file, i, self->buffer)); 
            }
        }
        return obj;
    }
    /* one value */
    else if (PyInt_Check(o) || PyString_Check(o)) {
        pos = f_getFieldPos(self->file, o);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else
            return f_getFieldValue(self->file, pos, self->buffer);
    }
    /* if tuple get specified fields */
    else if (PySequence_Check(o)) {
        char *label;
        len = PySequence_Length(o);
        if (output == LIST)
            obj = PyList_New(len);
        else {
            if (output == OBJ) {
                obj = f_createObject(cls);
                if(obj == NULL) {
                    PyErr_SetString(file400Error, "Could not create object.");
                    return NULL;
                }
                dict = PyObject_GetAttrString(obj, "__dict__");
                if(dict == NULL) {
                    PyErr_SetString(file400Error, "Object has no __dict__.");
                    return NULL;
                }
                Py_DECREF(dict);
            } else
                obj = dict = PyDict_New();
        }
        for (i = 0; i < len; i++) {
            fo = PySequence_GetItem(o, i);
            pos = f_getFieldPos(self->file, fo);
            Py_DECREF(fo);
            if (pos < 0 ) {
                PyErr_SetString(file400Error, "Field not valid.");
                Py_DECREF(obj);
                return NULL;
            } else {
                if (output == LIST)
                    PyList_SetItem(obj, i, f_getFieldValue(self->file, pos, self->buffer));
                else {
                    if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                        label = PyString_AsString(PyTuple_GetItem(labels, i));
                    } else
                        label = self->file->fieldArr[pos].name;
                    PyDict_SetItemString(dict, label,
                       f_getFieldValue(self->file, pos, self->buffer)); 
                }
            }
        }
        return obj;
    } 
}

static char rec_getString_doc[] =
"r.get([fields][fieldsep][decsep]) -> String.\n\
\n\
Get the field values as string seperated by tab.\n\
r.getString() - All fields.\n\
r.getString('cusno') - One field.\n\
r.getString(0) - One field by psoition in record.\n\
r.getString(('cusno','name')) - List of fields.\n\  
r.getString((0,1)) - List of fields.\n\
Default value for fieldsep(field seperator) is tab(\t).\n\
Default value for decsep(decimal seperator) is .(periode).";

static PyObject *
Record400_getString(Record400Object *self, PyObject *args, PyObject *keywds)
{
    PyObject *fields = Py_None;
    char decsep = '.';
    char fieldsep = '\t';
    static char *kwlist[] = {"fields","fieldsep","decsep", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|Occ:getString", kwlist, 
                                     &fields, &fieldsep, &decsep))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    /* one value */
    if (PyInt_Check(fields) || PyString_Check(fields)) {
        int pos = f_getFieldPos(self->file, fields);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else
            return f_getFieldString(self->file, pos, decsep, self->buffer);
    } else {
        char *buf;
        int i, len = 0, size = 4000;
        PyObject *newstr, *fo;
        newstr = PyString_FromStringAndSize(NULL, size);
        buf = PyString_AsString(newstr);
        /* if none, return all fields*/ 
        if (fields == NULL || fields == Py_None) {
            for (i = 0; i < self->file->fieldCount; i++) {
                if (self->file->fieldArr[i].len > size - len - 50) {
                    size += self->file->fieldArr[i].len;
                    _PyString_Resize(&newstr, size);
                }
                len += f_addToString(buf + len, self->buffer + self->file->fieldArr[i].offset,
                                    self->file->fieldArr[i].type,
                                    self->file->fieldArr[i].len,
                                    self->file->fieldArr[i].digits,
                                    self->file->fieldArr[i].dec, decsep);
                *(buf + len) = fieldsep;
                len++;
            }
        }
        /* if tuple get specified fields */
        else if (PySequence_Check(fields)) {
            int pos, seqlen = PySequence_Length(fields);
            for (i = 0; i < seqlen; i++) {
                fo = PySequence_GetItem(fields, i);
                pos = f_getFieldPos(self->file, fo);
                Py_DECREF(fo);
                if (pos < 0 ) {
                    PyErr_SetString(file400Error, "Field not valid.");
                    return NULL;        
                } else {
                    if (self->file->fieldArr[pos].len > size - len - 50) {
                        size += self->file->fieldArr[pos].len;
                        _PyString_Resize(&newstr, size);
                    }
                    len += f_addToString(buf + len, self->buffer + self->file->fieldArr[pos].offset,
                                        self->file->fieldArr[pos].type,
                                        self->file->fieldArr[pos].len,
                                        self->file->fieldArr[pos].digits,
                                        self->file->fieldArr[pos].dec, decsep);
                    *(buf + len) = fieldsep;
                    len++;
                }
            }
        } else {
            PyErr_SetString(file400Error, "Field not valid.");
            return NULL;        
        }
        _PyString_Resize(&newstr, len);
        return newstr;
    }
}

static char rec_getXml_doc[] =
"r.getXml([fields][labels][decsep][output][tag][cls]) -> String.\n\
\n\
Get the field values as a xml string from the buffer.\n\
r.getXml() - All fields.\n\
r.getXml('cusno') - One field.\n\
r.getXml(0) - One field by position in record.\n\
r.getXml(('cusno','name')) - List of fields.\n\  
r.getXml((0,1)) - List of fields.\n\
r.getXml((0,1),('pn','desc')) - Include labels to use.\n\
Default value for decsep(decimal seperator) is .(periode).\n\
output can have the following values:\n\
 0 - output as a tuple(default)\n\
 1 - output as an object\n\
 2 - output as a dictionary\n\
If tag = 0, not tag for the tuple/obj/dict is generated.\n\
if output = 1 and tag = 1, cls would be the class of object";

static PyObject *
Record400_getXml(Record400Object *self, PyObject *args, PyObject *keywds)
{
    PyObject *labels = Py_None;
    PyObject *fields = Py_None;
    PyObject *cls=NULL, *newstr, *fo;
    char decsep = '.';
    static char *kwlist[] = {"fields","labels","decsep","output","tag","cls", NULL};
    char *buf, *label;
    int i, len = 0, size = 4000, output = LIST, tag = 0;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|OOciiO:getXml", kwlist,
                                     &fields, &labels, &decsep, &output, &tag, &cls))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }
    newstr = PyString_FromStringAndSize(NULL, size);
    buf = PyString_AsString(newstr);
    /* one value */
    if (PyInt_Check(fields) || PyString_Check(fields)) {
        int pos = f_getFieldPos(self->file, fields);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } else {
            if (PyString_Check(labels))
                label = PyString_AsString(labels);
            else
                label = self->file->fieldArr[pos].name;
            if (self->file->fieldArr[pos].len > size - len - strlen(label) - 100) {
                size += self->file->fieldArr[pos].len;
                _PyString_Resize(&newstr, size);
            }
            len += f_addToXml(buf + len, label,
                              self->buffer + self->file->fieldArr[pos].offset,
                              self->file->fieldArr[pos].type,
                              self->file->fieldArr[pos].len,
                              self->file->fieldArr[pos].digits,
                              self->file->fieldArr[pos].dec, decsep, output);
        }
    } else {
        if (tag) {
            if (output == LIST)
                len += strlen(strcpy(buf + len, "<rec t=\"T\">"));
            else if (output == OBJ) {
                if (cls && PyClass_Check(cls)) {
                    PyObject *mod;
                    PyClassObject *class = (PyClassObject *)cls;
                    len += strlen(strcpy(buf + len, "<rec t=\"O\" c=\""));
                    len += strlen(strcpy(buf + len, PyString_AsString(class->cl_name)));
                    mod = PyDict_GetItemString(class->cl_dict, "__module__");
                    if (mod) {
                        len += strlen(strcpy(buf + len, "\" m=\""));
                        len += strlen(strcpy(buf + len, PyString_AsString(mod)));
                    }
                    len += strlen(strcpy(buf + len, "\">"));
                } else
                    len += strlen(strcpy(buf + len, "<rec t=\"O\">"));
            } else if (output == DICT)
                len += strlen(strcpy(buf + len, "<rec t=\"D\">"));
        }
        /* if none, return all fields*/ 
        if (fields == NULL || fields == Py_None) {
            for (i = 0; i < self->file->fieldCount; i++) {
                /* get label*/
                if (labels != Py_None && PyTuple_Check(labels)
                    && PyTuple_Size(labels) > i) {
                    label = PyString_AsString(PyTuple_GetItem(labels, i));
                } else
                    label = self->file->fieldArr[i].name;
                if (self->file->fieldArr[i].len > size - len - strlen(label) - 100) {
                    size += self->file->fieldArr[i].len;
                    _PyString_Resize(&newstr, size);
                }
                len += f_addToXml(buf + len, label,
                                  self->buffer + self->file->fieldArr[i].offset,
                                  self->file->fieldArr[i].type,
                                  self->file->fieldArr[i].len,
                                  self->file->fieldArr[i].digits,
                                  self->file->fieldArr[i].dec, decsep, output);
            }
        }
        /* if tuple get specified fields */
        else if (PySequence_Check(fields)) {
            int pos, seqlen = PySequence_Length(fields);
            for (i = 0; i < seqlen; i++) {
                fo = PySequence_GetItem(fields, i);
                pos = f_getFieldPos(self->file, fo);
                Py_DECREF(fo);
                if (pos < 0 ) {
                    PyErr_SetString(file400Error, "Field not valid.");
                    return NULL;
                } else {
                    /* get label*/
                    if (PyTuple_Check(labels) && PyTuple_Size(labels) > i) {
                        label = PyString_AsString(PyTuple_GetItem(labels, i));
                    } else
                        label = self->file->fieldArr[pos].name;
                    if (self->file->fieldArr[pos].len > size - len - strlen(label) - 100) {
                        size += self->file->fieldArr[pos].len;
                        _PyString_Resize(&newstr, size);
                    }
                    len += f_addToXml(buf + len, label,
                                      self->buffer + self->file->fieldArr[pos].offset,
                                      self->file->fieldArr[pos].type, 
                                      self->file->fieldArr[pos].len,
                                      self->file->fieldArr[pos].digits,
                                      self->file->fieldArr[pos].dec, decsep, output);
                }
            }
        } else {
            PyErr_SetString(file400Error, "Field not valid.");
            return NULL;        
        }
        if (tag)
            len += strlen(strcpy(buf + len, "</rec>"));
    }
    _PyString_Resize(&newstr, len);
    return newstr;
}

static char rec_getRrn_doc[] =
"r.getRrn() -> Int.\n\
\n\
Return record number.";

static PyObject *
Record400_getRrn(Record400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":getRrn"))
        return NULL;        
    return PyInt_FromLong(self->rrn);
}

static char rec_set_doc[] =
"r.set(Field, Value) -> String.\n\
\n\
Put value from Field into the buffer.\n\
Field can be both number and name.\n\
Same as f['cusno'] = 123, or f[0] = 123";

static PyObject *
Record400_set(Record400Object *self, PyObject *args)
{
    PyObject *k, *o;
    int i, pos;

    if (!PyArg_ParseTuple(args, "OO:set", &k, &o))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    /* if dict update fields in dict with values */
    if (PyDict_Check(o)) {
        PyErr_SetString(file400Error, "Not yet supported.");
        return NULL;
    }
    /* one value */
    else {
        pos = f_getFieldPos(self->file, k);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Parameter not valid.");
            return NULL;
        } 
        else if (f_setFieldValue(self->file, pos, o, self->buffer))
            return NULL;
    } 
    Py_INCREF(Py_None);
    return Py_None;
}

static char rec_getBuffer_doc[] =
"r.getBuffer() -> String.\n\
\n\
Get the raw value of buffer as a String.";

static PyObject *
Record400_getBuffer(Record400Object *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":getBuffer"))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    return PyString_FromStringAndSize(self->buffer, self->file->recLen);
    Py_INCREF(Py_None);
    return Py_None;
}


static char rec_setBuffer_doc[] =
"r.setBuffer(String/Record400) -> None.\n\
\n\
Copy the value of string to the internal record buffer.\n\
Use this with caution";

static PyObject *
Record400_setBuffer(Record400Object *self, PyObject *args)
{
    PyObject *o;

    if (!PyArg_ParseTuple(args, "S:setBuffer", &o))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    if (PyString_Check(o)) {
        if (self->file->recLen > ((PyStringObject *)o)->ob_size)
            memcpy(self->buffer, ((PyStringObject *)o)->ob_sval, 
                   ((PyStringObject *)o)->ob_size);
        else
            memcpy(self->buffer, ((PyStringObject *)o)->ob_sval, 
                   self->file->recLen);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static char rec_clear_doc[] =
"r.clear() -> None.\n\
\n\
Clear the record";

static PyObject *
Record400_clear(Record400Object *self, PyObject *args)
{
    PyObject *k, *o;
    int i, pos;

    if (!PyArg_ParseTuple(args, ":clear"))
        return NULL;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    f_clear(self->file, self->buffer);
    Py_INCREF(Py_None);
    return Py_None;
}

static int
Record400_length(Record400Object *self)
{
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return -1;
    }       
    return self->file->fieldCount;
}

static int
Record400_ass_subscript(Record400Object *self, PyObject *v, PyObject *w)
{
    int pos;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return -1;
    }       
    if (w == NULL) {
        PyErr_SetString(file400Error, "NULL value not supported.");
        return -1;
    } else {
        pos = f_getFieldPos(self->file, v);
        if (pos < 0) {
            PyErr_SetString(file400Error, "Field not found.");
            return -1;
        } else
            return f_setFieldValue(self->file, pos, w, self->buffer);
    }
}

static PyObject *
Record400_subscript(Record400Object *self, PyObject *v)
{
    int pos;
    if (self->file == NULL) {
        PyErr_SetString(file400Error, "No file for this record.");
        return NULL;
    }       
    pos = f_getFieldPos(self->file, v);
    if (pos < 0) {
        PyErr_SetString(file400Error, "Parameter not valid.");
        return NULL;
    } else
        return f_getFieldValue(self->file, pos, self->buffer);
}

static PyMappingMethods Record400_as_mapping = {
    (inquiry)Record400_length, /*mp_length*/
    (binaryfunc)Record400_subscript, /*mp_subscript*/
    (objobjargproc)Record400_ass_subscript, /*mp_ass_subscript*/
};

static PyMethodDef Record400Object_methods[] = {
    {"get", (PyCFunction)Record400_get, METH_VARARGS|METH_KEYWORDS, rec_get_doc},
    {"getFile",(PyCFunction)Record400_getFile, METH_VARARGS, rec_getFile_doc},
    {"getRrn",  (PyCFunction)Record400_getRrn, METH_VARARGS, rec_getRrn_doc},
    {"getBuffer", (PyCFunction)Record400_getBuffer, METH_VARARGS, rec_getBuffer_doc},
    {"getString",(PyCFunction)Record400_getString, METH_VARARGS|METH_KEYWORDS, rec_getString_doc},
    {"getXml",(PyCFunction)Record400_getXml, METH_VARARGS|METH_KEYWORDS, rec_getXml_doc},
    {"refresh", (PyCFunction)Record400_refresh, METH_VARARGS, rec_refresh_doc},
    {"set", (PyCFunction)Record400_set, METH_VARARGS, rec_set_doc},
    {"setBuffer", (PyCFunction)Record400_setBuffer, METH_VARARGS, rec_setBuffer_doc},
    {"clear",   (PyCFunction)Record400_clear, METH_VARARGS, rec_clear_doc},
    {NULL,      NULL}       /* sentinel */
};

static PyObject *
Record400_getattr(Record400Object *self, char *name)
{
    PyObject *fieldName, *retval;
    if (*name == '_' && *(name + 1) != '_') {       
        fieldName = PyString_FromString(name + 1);
        retval = Record400_subscript(self, fieldName);
        Py_DECREF(fieldName);
        return retval;
    } else
        return Py_FindMethod(Record400Object_methods, (PyObject *)self, name);
}

static int
Record400_setattr(Record400Object *self, char *name, PyObject *v)
{
    PyObject *fieldName;
    int retval;
    if (*name == '_') {
        fieldName = PyString_FromString(name + 1);
        retval = Record400_ass_subscript(self, fieldName, v);
        Py_DECREF(fieldName);
        return retval;
    } else
        return -1;
}

PyTypeObject Record400_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyObject_HEAD_INIT(NULL)
    0,          /*ob_size*/
    "Record400",            /*tp_name*/
    sizeof(Record400Object),    /*tp_basicsize*/
    0,          /*tp_itemsize*/
    /* methods */
    (destructor)Record400_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    (getattrfunc)Record400_getattr,/*tp_getattr*/
    (setattrfunc)Record400_setattr,/*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    &Record400_as_mapping,  /*tp_as_mapping*/
    0,          /*tp_hash*/
};

/* --------------------------------------------------------------------- */

static int
file400_initFile(File400Object *f, char *ovr)
{
    char retFileLib[20], fileLib[21];
    Qus_EC_t error;
    char *buff, *p, *c;
    Qdb_Qddfmt_t *foHd, foHd1;
    Qdb_Qddffld_t *fiHd;
    Qdb_Qdbwh_t *kyHd, kyHd1;
    Qdb_Qdbwhkey_t *kyKey;
    fieldInfoStruct *fieldInfo;
    keyInfoStruct *keyInfo;
    PyObject *obj;
    int i, j, offset;


    memset(fileLib, ' ', 20);
    fileLib[20] = '\0';
    memcpy(fileLib, f->file, strlen(f->file));
    memcpy(fileLib + 10, f->lib, strlen(f->lib));
    /* first call is to get size of buffer. */
    error.Bytes_Provided = sizeof(error);
    QDBRTVFD(&foHd1, sizeof(foHd1), retFileLib, "FILD0200", fileLib,
             "*FIRST    ", ovr, "*LCL      ", "*EXT      ", &error);
    if (error.Bytes_Available > 0)
        return -1;
    buff = p = PyMem_Malloc(foHd1.Qddbyava);
    QDBRTVFD(buff, foHd1.Qddbyava, retFileLib, "FILD0200", fileLib,
                 "*FIRST    ", ovr, "*LCL      ", "*EXT      ", &error);
    if (error.Bytes_Available > 0) {
        PyMem_Free(buff);
        return -1;
    }       
    /* set returned file name */
    str400tostr(f->retFile, retFileLib, 10);
    str400tostr(f->retLib, (retFileLib + 10), 10);
    /* get record info */
    foHd = (Qdb_Qddfmt_t *) p;
    str400tostr(f->recName, foHd->Qddfname, 10);
    str400tostr(f->recId, foHd->Qddfseq, 13);
    f->recLen = foHd->Qddfrlen;
    /* get field info */
    f->fieldCount = foHd->Qddffldnum; 
    f->fieldDict = PyDict_New();
    f->fieldArr = fieldInfo = PyMem_Malloc(f->fieldCount * sizeof(fieldInfoStruct));
    p += sizeof(Qdb_Qddfmt_t);
    fiHd = (Qdb_Qddffld_t *) p;
    for (i = 0; i < f->fieldCount; i++) {
        str400tostr(fieldInfo->name, fiHd->Qddffldi, 10);
        memcpy(&fieldInfo->type, fiHd->Qddfftyp, 2);
        fieldInfo->offset = fiHd->Qddffobo;
        fieldInfo->len = fiHd->Qddffldb;
        fieldInfo->digits = fiHd->Qddffldd;
        fieldInfo->dec = fiHd->Qddffldp;
        /* description */
        p += fiHd->Qddftxtd;
        str400tostr(fieldInfo->desc, p, 50);
        p -= fiHd->Qddftxtd;
        /* add to dictionary */
        obj = PyInt_FromLong(i);
        PyDict_SetItemString(f->fieldDict, fieldInfo->name, obj);
        Py_DECREF(obj);
        /* next field */
        fieldInfo++;
        p += fiHd->Qddfdefl;
        fiHd = (Qdb_Qddffld_t *) p;
    }
    /* free allocated storage */
    PyMem_Free(buff);
    /* get key info */
    f->keyDict = PyDict_New();
    f->keyCount = 0; 
    f->keyLen = 0; 
    QDBRTVFD(&kyHd1, sizeof(kyHd1), retFileLib, "FILD0300", fileLib,
             "          ", ovr, "*LCL      ", "*EXT      ", &error);
    if (error.Bytes_Available == 0) {
        buff = p = PyMem_Malloc(kyHd1.Byte_Avail);
        QDBRTVFD(buff, kyHd1.Byte_Avail, retFileLib, "FILD0300", fileLib,
                 "          ", ovr, "*LCL      ", "*EXT      ", &error);
        if (error.Bytes_Available > 0) {
            PyMem_Free(buff);
            return -1;
        }       
        /* find offset to first record format */
        kyHd = (Qdb_Qdbwh_t *) p;
        f->keyCount = kyHd->Rec_Key_Info->Num_Of_Keys;
        if (f->keyCount > 0) {
            p += kyHd->Rec_Key_Info->Key_Info_Offset;
            kyKey = (Qdb_Qdbwhkey_t *) p;
            f->keyArr = keyInfo = PyMem_Malloc(f->keyCount * sizeof(keyInfoStruct));
            for (j = 0; j < f->keyCount; j++) {
                str400tostr(keyInfo->name, kyKey->Int_Field_Name, 10);
                keyInfo->type = kyKey->Data_Type;
                keyInfo->offset = f->keyLen;
                keyInfo->len = kyKey->Field_Len;
                keyInfo->digits = kyKey->Num_Of_Digs;
                keyInfo->dec = kyKey->Dec_Pos;
                /* add to dictionary */
                obj = PyInt_FromLong(j);
                PyDict_SetItemString(f->keyDict, keyInfo->name, obj);
                Py_DECREF(obj);
                f->keyLen += keyInfo->len;
                keyInfo++;
                kyKey++;
            }
        }
    }
    /* allocate storage for temporary record and key buffer */
    f->recbuf = PyMem_Malloc(f->recLen + 1);
    f->keybuf = PyMem_Malloc(f->keyLen + 1);
    /* return ok */ 
    return 0;
}

/* Exporting integer values */
static int
initint(PyObject *d, char *name, int val)
{
    PyObject *o = PyInt_FromLong((long) val);
    if (o != NULL) {
        PyDict_SetItemString(d, name, o);
        Py_DECREF(o);
    }
}

char File400_doc[] =
"File400(Filename[mode, options, lib, mbr, qry]) -> File400 Object\n\
\n\
Creates a new File400 Object.\n\
Filename must exist. Mode None(default) does not open the file,\n\
Use mode 'r' for reading , 'a' for appending \n\
and 'r+' for read, update and append.\n\
The options shold be a string with this format: 'arrseq=Y, secure=Y'.\n\
Options:\n\ 
arrseq  - Arrival sequence. (Y, N(default))\n\
blkrcd  - Preforms record blocking. (Y(default), N)\n\
ccsid   - Specifies the CCSID. Default is 0 (the job CCSID is used.)\n\
commit  - Open under commitment control. (Y, N(default)).\n\
secure  - Secure from overrides. (Y, N(default)).\n\
You can specify the library, special values are *LIBL(default) and *CURLIB.\n\
You can also specify the member, special value are *FIRST(default).\n\
The keyword qry specifies an opnqryf string.\n\
\n\
Methodes:\n\
  open       - Open file.\n\
  close      - Close file.\n\
  read..     - Read from file.\n\
  pos..      - Position in file.\n\
  write      - Write new record to file.\n\
  update     - Update current record.\n\
  rlsLock    - Release lock on current record.\n\
  clear      - Clear the record buffer.\n\
  get        - Get values from record buffer.\n\
  getRecord  - Get copy of record buffer.\n\
  getKey     - Get key values.\n\
  getRrn     - Get relative record number.\n\
  getString  - Get record as tab seperated string.\n\
  isOpen     - Returns 1 if file is open otherwise 0.\n\
  mode       - Returns open mode (r, a, r+).\n\
  fileName   - Returns Name of the file.\n\
  libName    - Returns the library name.\n\
  mbrName    - Returns current member name.\n\
  mbrList    - Returns tuple of member names.\n\
  fieldCount - Number of fields in file.\n\
  fieldList  - Returns tuple of field names.\n\
  fieldDesc  - Get field description.\n\
  fieldSize  - Get field size.\n\
  fieldType  - Get field type.\n\
  recordSize - Record size in bytes.\n\
  keyCount   - Number of key fields.\n\
  keyList    - Returns tuple of key field names.\n\
  set        - Set field values into record buffer.\n\
  setKey     - Set key values into key buffer.\n\
  setRecord  - Updates the record buffer.\n\
\n\
See the __doc__ string on each method for details.\n\
>>> f = File400('YOURFILE')\n\
>>> print f.open__doc__";

DL_EXPORT(PyObject *)
file400_File400(PyObject *self, PyObject *args, PyObject *keywds)
{
    int i;
    char *file, *override = "1";
    char *lib = "*LIBL";
    char *mbr = "*FIRST";
    char *qry = NULL;
    char qrybuf[1024];
    static char *kwlist[] = {"file", "mode", "options", "lib", "mbr", "qry", NULL};
    PyObject *mode = Py_None, *keyw = Py_None;
    File400Object *nf;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s|OOsss:File400", kwlist, 
                                     &file, &mode, &keyw, &lib, &mbr, &qry))
        return NULL;
    if (strlen(file) > 10 || strlen(lib) > 10 || strlen(mbr) > 10) {
        PyErr_SetString(file400Error, "File,Lib and Member have max length of 10.");
        return NULL;
    }
    /* convert file, lib and mbr to uppercase */
    strtoupper(file);
    strtoupper(lib);
    strtoupper(mbr);
    /* check if keyword secure */
    if (PyString_Check(keyw) && strstr(PyString_AS_STRING(keyw), "secure=Y"))
        override = "0";
    /* if opnqryfile  */
    if (qry) {
        if (strlen(qry) > 1000) {
            PyErr_SetString(file400Error, "Query string exceeds 1000.");
            return NULL;
        }
        sprintf(qrybuf, "ovrdbf %s TOFILE(%s/%s) share(*yes)", file, lib, file); 
        if (f_system(qrybuf)) {
            PyErr_SetString(file400Error, "Ovrdbf failed.");
            return NULL;
        }
        /* check if file parameter exists */
        strcpy(qrybuf, qry);
        strtoupper(qrybuf);
        if (!strstr(qrybuf, "FILE(")) {
            sprintf(qrybuf, "opnqryf %s ", file);
        } else
            strcpy(qrybuf, "opnqryf ");         
        strcat(qrybuf, qry);
        if (f_system(qrybuf)) {
            PyErr_SetString(file400Error, "Opnqryf failed.");
            return NULL;
        }
    }
    nf = PyObject_New(File400Object, &File400_Type);
    if (nf == NULL)
        return NULL;
    nf->fp = NULL;
    strcpy(nf->file, file);
    strcpy(nf->lib, lib);
    strcpy(nf->mbr, mbr);
    nf->qry = (qry != NULL);
    nf->recbuf = NULL;
    nf->keybuf = NULL;
    nf->fieldDict = NULL;
    nf->fieldArr = NULL;
    nf->keyDict = NULL;
    nf->keyArr = NULL;
    /* get and store file information */
    if (file400_initFile(nf, override)) {
        PyErr_SetString(file400Error, "Error getting file information.");
        Py_DECREF(nf);
        return NULL;
    }
    /* open the file */
    if (mode != Py_None) { 
        if (File400_open(nf, Py_BuildValue("OO", mode, keyw), NULL) == NULL) {
            Py_DECREF(nf);
            return NULL;
        }
    }
    return (PyObject *) nf;
}

/* List of functions defined in the module */
static PyMethodDef file400_memberlist[] = {
    {"File400", (PyCFunction)file400_File400, METH_VARARGS|METH_KEYWORDS, File400_doc},
    {NULL,      NULL}
};

/* Initialization function for the module */
/* Not a normal initialization because this goes into builtins */
DL_EXPORT(void)
initfile400(void)
{
    PyObject *m, *d;

    /* Initialize the type of the new type object here; doing it here
     * is required for portability to Windows without requiring C++. */
    File400_Type.ob_type = &PyType_Type;
    Record400_Type.ob_type = &PyType_Type;

    /* Create the module and add the functions */
    m = Py_InitModule("file400", file400_memberlist);

    /* Add some symbolic constants to the module */
    d = PyModule_GetDict(m);
    file400Error = PyErr_NewException("file400.error", NULL, NULL);
    PyDict_SetItemString(d, "error", file400Error);
    dummyClassName = PyString_FromString("dummy");
    dummyClass = PyClass_New(NULL, PyDict_New(), dummyClassName);
}








