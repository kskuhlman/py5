/*
 * as400misc  Misc routines 
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
#include <qleawi.h>
#include <qusec.h>
#include <qmhchgem.h>
#include <qwcrdtaa.h>
#include <iconv.h>
#include <miptrnam.h>
#include "as400misc.h"

static int cp37Init = 0;
static iconv_t cdToCp37;
static iconv_t cdFromCp37;

typedef struct { 
    Qus_EC_t ec_fields;
    char     exception_data[100];
} error_code_struct;

/* returns a conversion descriptor for from and to conversion */
/* between given ccsid and the jobs current ccsid */
void
initConversion(char *ccsid, iconv_t *cdfrom, iconv_t *cdto)
{
    char fromcode1[33] = "IBMCCSID000000000101";
    char tocode1[33] = "IBMCCSID00037";
    char fromcode2[33] = "IBMCCSID000370000101";
    char tocode2[33] = "IBMCCSID00000";
    memset(&fromcode1[20],'\0',13);
    memcpy(&tocode1[13 - strlen(ccsid)], ccsid, strlen(ccsid));
    memset(&tocode1[13],'\0',20);
    *cdto = iconv_open(tocode1, fromcode1);
    memcpy(&fromcode2[13 - strlen(ccsid)], ccsid, strlen(ccsid));
    memset(&fromcode2[20],'\0',13);
    memset(&tocode2[13],'\0',20);
    *cdfrom = iconv_open(tocode2, fromcode2);
}

void
initCp37(void)
{
    initConversion("0037", &cdFromCp37, &cdToCp37);
    cp37Init = 1;
}

/* convert size number of characters */
/* in and out are updated */
void
convertChar(iconv_t cd, char *in, char *out, int size)
{
    size_t sizefrom, sizeto;
    sizefrom = sizeto = size;
    iconv(cd, &in, &sizefrom, &out, &sizeto);
}

/* convert string */
char *
convertString(iconv_t cd, char *in, char *out) 
{
    char *p1, *p2;
    size_t sizefrom, sizeto;
    int ssize = strlen(in);
    if (ssize > 0) {
        p1 = in;
        p2 = out;
        sizefrom = sizeto = ssize;
        iconv(cd, &p1, &sizefrom, &p2, &sizeto);
        memset(p2, '\0', 1);
    } else {
        memset(out, '\0', 1);
    }
    return out;
}

/* convert string from job ccsid to ccsid 37 */
char *
strToCp37(char *in, char *out)
{
    if (cp37Init == 0) initCp37();
    return convertString(cdToCp37, in, out);
}

/* convert string from ccsid 37 to job ccsid */
char *
strFromCp37(char *in, char *out)
{
    if (cp37Init == 0) initCp37();
    return convertString(cdFromCp37, in, out);
}

/* convert size number of chars to ccsid 37 */
/* in and out are updated */
void
toCp37(char *in, char *out, int size)
{
    if (cp37Init == 0) initCp37();
    convertChar(cdToCp37, in, out, size);
}

/* convert characters from ccsid 37 to job ccsid */
/* in and out are updated */
void
fromCp37(char *in, char *out, int size)
{
    if (cp37Init == 0) initCp37();
    convertChar(cdFromCp37, in, out, size);
}

/* check if alpha but use cp37 */   
int isalpha37(int c) {
    return ((c > 128 && c < 138) || (c > 144 && c < 154) || 
            (c > 161 && c < 170) || (c > 192 && c < 202) || 
            (c > 208 && c < 218) || (c > 225 && c < 234));
}
/* check if alphanumeric but use cp37 */    
int isalnum37(int c) {
    return ((c > 128 && c < 138) || (c > 144 && c < 154) || 
            (c > 161 && c < 170) || (c > 192 && c < 202) || 
            (c > 208 && c < 218) || (c > 225 && c < 234) ||
            (c > 239 && c < 250));
}
/* convert string to as/400 string */   
char * strtostr400(char *asstr, char *str, int len) {
    int i = strlen(str);

    if (i > len)
        i = len; 
    if (i)
        memcpy(asstr, str, i);
    if (i < len)
        memset((asstr + i), ' ', len - i); 
    asstr[len] = '\0';
    return asstr;
}

/* convert as/400 string to string */   
char * str400tostr(char *str, char *asstr, int len) 
{
    int i = len - 1;
    while (i >= 0 && isspace(Py_CHARMASK(asstr[i])))
        i--;
    if (++i)
        memcpy(str, asstr, i);
    str[i] = '\0';
    return str;
}
/* convert string to upper */
char *
strtoupper(char *str)
{
    int i, c;
    char *s;
    s = str;
    while (*s != '\0') {
        c = Py_CHARMASK(*s);
        if (islower(c))
            *s = toupper(c);               
        s++;
    }
    return str;
}

/* convert zoned to string */
char *
zonedtostr(char *buf, char *p, int digits, int dec, char decsep)
{
    unsigned char c;
    int decstart, i, j = 0;
    decstart = digits - dec;
    /* check if negative */
    if (((p[digits - 1] >> 4) | 0xF0) != 0xFF) {                
        buf[j] = '-';
        j++;
    }           
    /* get all digits */
    for (i = 0; i < digits; i++) {              
        c = p[i] | 0xF0;
        /* decimal point */
        if (i == decstart) {
            if (j == 0 || (j == 1 && buf[0] == '-')) {
                buf[j] = '0';
                j++;
            }
            buf[j] = decsep;
            j++;
        }
        if (c != 0xF0 || j > 1 || (j > 0 && buf[0] != '-'))  {
            buf[j] = c;
            j++;
        }
    }
    if (j == 0) {
        buf[j] = '0';
        j++;
    }
    buf[j] = '\0';
    return buf;
}

/* convert packed to string */
char *
packedtostr(char *buf, char *p, int digits, int dec, char decsep)
{
    unsigned char c;
    int decstart, i, j = 0;
    /* size should be odd */
    if ((digits % 2) == 0)
        digits++;
    decstart = digits - dec;
    /* check if negative */
    if ((p[digits/2] | 0xF0) != 0xFF) {             
        buf[j] = '-';
        j++;
    }           
    /* get all digits */
    for (i = 0; i < digits; i++) {              
        if (i % 2)
            c = p[i / 2] | 0xF0;
        else
            c = (p[i / 2] >> 4) | 0xF0;
        /* decimal point */
        if (i == decstart) {
            if (j == 0 || (j == 1 && buf[0] == '-')) {
                buf[j] = '0';
                j++;
            }
            buf[j] = decsep;
            j++;
        }
        if (c != 0xF0 || j > 1 || (j > 0 && buf[0] != '-'))  {
            buf[j] = c;
            j++;
        }
    }
    if (j == 0) {
        buf[j] = '0';
        j++;
    }
    buf[j] = '\0';
    return buf;
}

static void rmvmsg(_INTRPT_Hndlr_Parms_T *errmsg)
{
    error_code_struct Error_Code;
    Error_Code.ec_fields.Bytes_Provided = 0;
    QMHCHGEM(&(errmsg->Target), 0, (char *)&errmsg->Msg_Ref_Key,
             "*REMOVE   ", "", 0, &Error_Code);
} 

char *
getpythonhome(void)
{
    static char pyhome[256], prog[20];
    char dtaara[21] = "PYTHONHOME          ";
    char buf[256], *p;
    Qus_EC_t error;
    Qwc_Rdtaa_Data_Returned_t *retData;
    strcpy(prog, Py_GetProgramName());
    p = strchr(prog, '/');
    if (p) strncpy(dtaara + 10, prog, p - prog);
    p = (char *) buf;
    error.Bytes_Provided = sizeof(error);
    QWCRDTAA(p, 256, dtaara, 1, 100, &error);
    if (error.Bytes_Available <= 0) {
        retData = (Qwc_Rdtaa_Data_Returned_t *)p;
        if (!strncmp(retData->Type_Value_Returned, "*CHAR     ", 10)) {
            str400tostr(pyhome, p + sizeof(*retData), retData->Length_Value_Returned);
            return pyhome;
        }
    }
    return Py_GetPythonHome();
}

/* resolve program object */
_SYSPTR getPgm(char *name, char *lib) 
{
    char asname[11], aslib[11];
    volatile _INTRPT_Hndlr_Parms_T  excpData;
    strtostr400(asname, name, 10);
    strtostr400(aslib, lib, 10);
    strtoupper(asname);
    strtoupper(aslib);
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    return rslvsp(_Program, asname, aslib, _AUTH_OBJ_MGMT);
 EXCP1:
    rmvmsg((_INTRPT_Hndlr_Parms_T *)&excpData);
    return NULL;
}

/* resolve service program object */
int getSrvpgm(char *name, char *lib)
{
    char asname[11], aslib[11]; 
    volatile _INTRPT_Hndlr_Parms_T  excpData;
    _SYSPTR ptr;
    strtostr400(asname, name, 10);
    /* if name starts with _ replace it with Z */
    if (asname[0] == '_') asname[0] = 'Z';
    strtostr400(aslib, lib, 10);
    strtoupper(asname);
    strtoupper(aslib);
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    ptr = rslvsp(WLI_SRVPGM, asname, aslib, _AUTH_POINTER);
    return QleActBndPgm(&ptr, NULL, NULL, NULL, NULL);
 EXCP1:
    rmvmsg((_INTRPT_Hndlr_Parms_T *)&excpData);
    return 0;
}

/* get service program function pointer */
_OPENPTR getFunction(int actmark, char *name)
{
    volatile _INTRPT_Hndlr_Parms_T  excpData;
    _OPENPTR ptr;
    int expres;
#pragma exception_handler(EXCP1, excpData, 0, _C2_MH_ESCAPE, _CTLA_HANDLE)
    QleGetExp(&actmark, 0, 0, name, &ptr, &expres, NULL);
    if (expres == 1)
        return ptr;
 EXCP1:
    return NULL;
}
