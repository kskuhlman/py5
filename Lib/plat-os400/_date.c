/*
 * _date
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
#include "_date.h"
#include <time.h>

#define ISLEAPYEAR ((year%4 == 0) && ((year%100 != 0) || (year%400 == 0)))
#define DAYSYEAR 365.2425
#define COM_OFFSET 693594  /* 1 = 1899.12.31 */
#define EPOCH_OFFSET 719163  /* 1 = 1970.01.01 */
#define SECPERDAY 86400  /* seconds per day */

static int monthOffset[2][13] = {{0,31,59,90,120,151,181,212,243,273,304,334,365},
                                 {0,31,60,91,121,152,182,213,244,274,305,335,366}};
static int daysInMonth[2][12] = {{31,28,31,30,31,30,31,31,30,31,30,31},
                                 {31,29,31,30,31,30,31,31,30,31,30,31}};
/* save the two last dates with information */
static DateInfo dc[2] = {{0},{0}};
static int lastdc = 0;

/* calculates date value. */
/* returns 0 if date not valid */
long
dateToValue(int year, int month, int day) {
    int leap = ISLEAPYEAR;
    /* check if valid */
    if (year < 1 || month < 1 || month > 12 || day < 1
        || day > daysInMonth[leap][month -1])
        return 0;   
    /* offset this year */
    year--;
    return year*365 + year/4 - year/100 + year/400
        + monthOffset[leap][month -1] + day;
}

/* calculate date information from value. */
DateInfo *
dateFromValue(long value, DateInfo *di)
{
    /* check last used values */    
    if (dc[0].value == value) {
        return memcpy(di, &dc[0], sizeof(dc[0]));
    } else if (dc[1].value == value) {
        return memcpy(di, &dc[1], sizeof(dc[1]));
    } else {        
        int year, leap, i;
        di->value = value;
        year = (int)(value / DAYSYEAR) + 1;
        /* get offset for this year */
        year--;
        value -= (year*365 + year/4 - year/100 + year/400);
        /* if zero this has to be 31.12 year before */
        if (value <= 0) {
            di->year = year;
            di->month = 12;
            di->day = 31;
            di->leap = ISLEAPYEAR;
            di->days = 365 + di->leap;
        } else {    
            year++;
            /* get month and day */
            leap = ISLEAPYEAR;
            for (i=0; i<13; i++) {
                if (monthOffset[leap][i] >= value) {                 
                    di->year = year;                    
                    di->month = i;
                    di->day = value - monthOffset[leap][i-1];
                    di->days = value;
                    di->leap = leap;
                    break;
                }
            }
            /* next year 01.01 */
            if (i == 13) {
                year++;
                di->year = year;
                di->month = 1;
                di->day = 1;
                di->days = 1;
                di->leap = ISLEAPYEAR;
            }
        }
        di->date = di->year * 10000 + di->month * 100 + di->day;
        memcpy(&dc[lastdc], di, sizeof(dc[lastdc])); 
        lastdc = (!lastdc);
        return di;
    }
}   
    
long
dateToTicks(long value)
{
    struct tm tm;
    DateInfo di;
    dateFromValue(value, &di);
    memset(&tm, 0, sizeof(tm));
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_mday = di.day;
    tm.tm_mon = di.month - 1;
    tm.tm_year = di.year - 1900;
    tm.tm_isdst = -1;
    return mktime(&tm); 
}
    
int
dateToWeek(long value)
{
    int jan04, week1start;
    DateInfo di;
    dateFromValue(value, &di);
    /* find first week this year jan.4 is always in week 1*/
    jan04 = value - di.days + 4;
    week1start = jan04 - ((jan04 - 1) % 7);
    if (value < week1start) {
        /* first week previous year */
        di.year--;
        if (dateIsLeapYear(di.year)) jan04 -= 366;
        else jan04 -= 365;
        week1start = jan04 - ((jan04 - 1) % 7);
    } else if (di.days > 362) {
        /* check first week next year */
        int week1next;
        jan04 += monthOffset[di.leap][12];
        week1next = jan04 - ((jan04 - 1) % 7);
        if (value >= week1next) {
            di.year++;
            week1start = week1next;
        }
    }
    return di.year * 100 + ((value - week1start) / 7) + 1;
}

/* find easter sunday returns a date value */
long 
dateEasterSunday(int year)
{
    int year19Cycle, century, x, z, d, e, n;
    year19Cycle = (year % 19) + 1;
    century = year/100 + 1;
    x =(3*century)/4 - 12;
    z = (8*century + 5)/25 - 5;
    d = 5*year/4 - x - 10;
    e = (11*year19Cycle + 20 + z - x) % 30;
    if (((e == 25) && (year19Cycle > 11)) || (e == 24))
        e++;
    n = 44 - e;
    if ( n < 21 ) 
        n += 30;
    n += 7 - ((d + n) % 7);
    if ( n <= 31 ) {
        return dateToValue(year, 3, n);
    } else {
        return dateToValue(year, 4, n-31);
    }
}

int
dateIsLeapYear(int year) 
{
    return ISLEAPYEAR;
}

long
dateAddMonths(long value, int months)
{
    DateInfo di;
    dateFromValue(value, &di);
    di.month += months;
    if (abs(di.month) > 12) {
        di.year += di.month / 12;
        di.month = di.month % 12;
    }
    if (di.month <= 0) {
        di.month += 12;
        di.year--;
    }
    di.leap = dateIsLeapYear(di.year);
    if (daysInMonth[di.leap][di.month -1] < di.day)
        di.day = daysInMonth[di.leap][di.month -1];
    return dateToValue(di.year, di.month, di.day);
}

/* parses a string, returns 0 on failure */
long
dateParse(char *s, char *format, char datesep, int year4)
{
    char *s2;
    int x[3], i = 0, year, month, day;
    s2 = s;
    while (1) {
        if (isdigit(*s)) {
            s++;
        } else if (*s == '\0' || *s == datesep) {
            x[i++] = atoi(s2);
            if (*s == '\0' || i == 3) 
                break;
            s++;
            s2 = s;
        } else {
            return 0;
        }
    }
    if (!strcmp(format,"ymd")) {
        if (datesep == '\0') {
            year = x[0]/10000; month = (x[0]%10000)/100; day = x[0]%100;
        } else {
            year = x[0]; month = x[1]; day = x[2];
        }
    } else if (!strcmp(format,"dmy")) {
        if (datesep == '\0') {
            if (year4) {
                year = x[0]%10000; month = (x[0]%1000000)/10000; day = x[0]/1000000;
            } else {
                year = x[0]%100; month = (x[0]%10000)/100; day = x[0]/10000;
            }
        } else {
            year = x[2]; month = x[1]; day = x[0];
        }
    } else if (!strcmp(format,"mdy")) {
        if (datesep == '\0') {
            if (year4) {
                year = x[0]%10000; month = x[0]/1000000; day = (x[0]%1000000)/10000;
            } else {
                year = x[0]%100; month = x[0]/10000; day = (x[0]%10000)/100;
            }
        } else {
            year = x[2]; month = x[0]; day = x[1];
        }
    } else {
        return 0;
    }   
    if (!year4) {
        if (year >= 100) {
            return 0;
        }
        if (year < 50) year += 2000;
        else year += 1900;             
    }       
    return dateToValue(year, month, day);
}

/* create date from date value (days since 0) */
PyObject*
dateNew(long value)
{
    DateObject* self;
    if (value <= 0) {
        PyErr_SetString(PyExc_ValueError, "Date value not valid.");
        return NULL;
    }
    self = PyObject_NEW(DateObject, &Date_Type);
    if (self == NULL)
        return NULL;
    self->value = value;
    return (PyObject*) self;
}

/* create date from ticks, if zero localtime(time()) is used */
PyObject*
dateNewTicks(long ticks) 
{
    struct tm *tm;
    if (ticks == 0)
        ticks = time(&ticks);   
    tm = localtime(&ticks);
    return dateNew(dateToValue(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday));

}

static char date_Date_doc[] =
"date.Date([value]) -> DateObject.\n\
\n\
Creates a new date object from a value (days since 0).\n\
If date is zero or not passed, current day is returned.";
 
static PyObject *
date_Date(PyObject *self, PyObject *args)
{
    int value=0;
    if (!PyArg_ParseTuple(args, "|i:Date", &value))
        return NULL;
    if (value == 0)
        return dateNewTicks(0);
    else
        return dateNew(value);
}

static char date_fromInt_doc[] =
"date.fromInt(date,[format],[year4]) -> DateObject.\n\
\n\
Creates a new date object from an int and a format.\n\
Format can be \"ymd\"(default), \"mdy\" or \"dmy\".\n\
Year4 could be true(default) or false(2 digits year).\n\
If the date is zero a special NULL date is returned.";
 
static PyObject *
date_fromInt(PyObject *self, PyObject *args)
{
    long date = 0; 
    int year4 = 1;
    char *format = NULL;
    if (!PyArg_ParseTuple(args, "i|sii:fromInt", &date, &format, &year4))
        return NULL;
    if (date == 0) {
        PyObject *d = dateNew(1);
        ((DateObject*)d)->value = 0;
        return d;
    }       
    if (!year4 && date > 1000000) {
        PyErr_SetString(PyExc_ValueError, "Year not valid.");
        return NULL;        
    }
    if (!format || !strcmp(format,"ymd")) {         
        if (!year4) {
            if (date < 500000) date += 20000000;
            else date += 19000000;
        }          
        return dateNew(dateToValue(date/10000, (date%10000)/100, date%100));
    } else {
        int year, month, day;
        if (!strcmp(format,"dmy")) {
            if (year4) {
                year = date%10000; month = (date%1000000)/10000; day = date/1000000;
            } else {
                year = date%100; month = (date%10000)/100; day = date/10000;
            }
        } else if (!strcmp(format,"mdy")) {
            if (year4) {
                year = date%10000; month = (date%1000000)/10000; day = date/1000000;
            } else {
                year = date%100; month = (date%10000)/100; day = date/10000;
            }
        } else {
            PyErr_SetString(PyExc_ValueError, "Format not valid.");
            return NULL;
        }
        if (!year4) {
            if (year < 50) year += 2000;
            else year += 1900;
        }
        return dateNew(dateToValue(year, month, day));
    }
}

static char date_fromISO_doc[] =
"date.fromISO(date, [zerovalid]) -> DateObject.\n\
\n\
Creates a new date object from an ISO value.\n\
If zerovalid is true(default) a value of zero gives a NULL date.";
 
static PyObject *
date_fromISO(PyObject *self, PyObject *args)
{
    long date = 0; 
    int zerovalid = 1;
    if (!PyArg_ParseTuple(args, "i|i:fromISO", &date, &zerovalid))
        return NULL;
    if (zerovalid && date == 0) {
        PyObject *d = dateNew(1);
        ((DateObject*)d)->value = 0;
        return d;
    } else {
        return dateNew(dateToValue(date/10000, (date%10000)/100, date%100));
    }
}

static char date_parse_doc[] =
"date.dateParse(datestring, format, datesep, [year4]) -> DateObject.\n\
\n\
Parses the string and creates a new date object.\n\
Format can be \"ymd\"(default), \"mdy\" or \"dmy\".\n\
Datesep is date seperator.\n\
Year4 could be 1(default) - year has 4 digits, 0 - year has 2 digits."; 

static PyObject *
date_parse(PyObject *self, PyObject *args)
{
    PyObject *dsobj;
    char *s, *format, datesep;
    int year4 = 1;
    if (!PyArg_ParseTuple(args, "ssO|i:dateParse", &s, &format,
                          &dsobj, &year4))
        return NULL;
    /* check date seperator */
    if (dsobj == Py_None)
        datesep = '\0';
    else if(!PyString_Check(dsobj))
        return NULL;
    else
        datesep = *(PyString_AsString(dsobj));  
    return dateNew(dateParse(s, format, datesep, year4));
}

static char date_fromYMD_doc[] =
"date.dateFromYMD(year, month, day) -> DateObject.\n\
\n\
Creates a new date object from year, month, day.";
 
static PyObject *
date_fromYMD(PyObject *self, PyObject *args)
{
    int year, month, day;
    if (!PyArg_ParseTuple(args, "iii:fromYMD", &year, &month, &day))
        return NULL;
    return dateNew(dateToValue(year, month, day));
}

static char date_fromCOM_doc[] =
"date.fromCOM(value) -> DateObject.\n\
\n\
Creates a new date object from a COM value (days since 1900).";
 
static PyObject *
date_fromCOM(PyObject *self, PyObject *args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i:fromCOM", &value))
        return NULL;
    return dateNew(value + COM_OFFSET);
}

static char date_fromTicks_doc[] =
"date.fromTicks(value) -> DateObject.\n\
\n\
Creates a new date object from seconds since 1970.";
 
static PyObject *
date_fromTicks(PyObject *self, PyObject *args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i:fromTicks", &value))
        return NULL;
    return dateNewTicks(value);
}

static char date_easterSunday_doc[] =
"date.easterSunday(year) -> DateObject.\n\
\n\
Creates a new Date object for easter sunday for given year.";
 
static PyObject *
date_easterSunday(PyObject *self, PyObject *args)
{
    int year;
    if (!PyArg_ParseTuple(args, "i:easterSunday", &year))
        return NULL;
    return dateNew(dateEasterSunday(year));
}

static char date_isLeapYear_doc[] =
"date.isLeapYear(year) -> int.\n\
\n\
Returns 1 if year is leap year otherwise 0.";
 
static PyObject *
date_isLeapYear(PyObject *self, PyObject *args)
{
    int year;
    if (!PyArg_ParseTuple(args, "i:isLeapYear", &year))
        return NULL;
    return PyInt_FromLong(dateIsLeapYear(year));
}

static char date_daysInMonth_doc[] =
"date.daysInMonth(month, [year]) -> int.\n\
\n\
Returns days in month, year is optional.";
 
static PyObject *
date_daysInMonth(PyObject *self, PyObject *args)
{
    int month, year = 0;
    if (!PyArg_ParseTuple(args, "i|i:daysInMonth", &month, &year))
        return NULL;
    if (year < 0 || month < 1 || month > 12) {
        PyErr_SetString(PyExc_ValueError, "Argument(s) not valid.");
        return NULL;
    } else if(year != 0)
        year = dateIsLeapYear(year);
    return PyInt_FromLong(daysInMonth[year][month-1]);
}

static char Date_addMonths_doc[] =
"Date.addMonths(months) -> None.\n\
\n\
Add months to date. If day becomes invalid, last day in month is used.";
 
static PyObject *
Date_addMonths(PyObject *self, PyObject *args)
{   
    int months;
    if (!PyArg_ParseTuple(args, "i:addMonths", &months))
        return NULL;
    ((DateObject *)self)->value = dateAddMonths(((DateObject *)self)->value, months);
    Py_INCREF(Py_None);
    return Py_None;
}

static char Date_format_doc[] =
"Date.format([formatstr]) -> String.\n\
\n\
Returns a formatted string of the date. Default format is %x.";
 
static PyObject *
Date_format(PyObject *self, PyObject *args)
{   
    struct tm tm;
    DateInfo di;
    char *format = "%x", buf[20];
    if (!PyArg_ParseTuple(args, "|s:format", &format))
        return NULL;
    if (((DateObject *)self)->value == 0)
        return PyString_FromString("");
    dateFromValue(((DateObject *)self)->value, &di);
    memset(&tm, 0, sizeof(tm));
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_mday = di.day;
    tm.tm_mon = di.month - 1;
    tm.tm_year = di.year - 1900;
    strftime(buf, 19, format, &tm);
    return PyString_FromString(buf);
}

static void
date_dealloc(DateObject *self)
{
    PyObject_Del(self);
}
 
static long
date_hash(DateObject *v)
{
    return v->value;
}

static int
date_compare(DateObject *v, DateObject *w)
{
    register long i = v->value;
    register long j = w->value;
    return (i < j) ? -1 : (i > j) ? 1 : 0;
}
 
static PyObject *
date_repr(DateObject *v)
{
    char buf[9];
    sprintf(buf, "%d", v->value);
    return PyString_FromString(buf);
}
  
static PyObject *
date_str(DateObject *v)
{
    DateInfo di;
    char buf[11];
    if (v->value == 0)
        buf[0] = '\0';
    else {
        dateFromValue(v->value, &di);
        sprintf(buf, "%04d-%02d-%02d", di.year, di.month, di.day);
    }
    return PyString_FromString(buf);
}


/* -------------------------------------------------------------------- */
/* type interface */

static PyMethodDef Date_methods[] = {
    {"addMonths", (PyCFunction) Date_addMonths, METH_VARARGS, Date_addMonths_doc},
    {"format", (PyCFunction) Date_format, METH_VARARGS, Date_format_doc},
    {NULL, NULL}
};

static PyObject *
date_getattr(DateObject *self, char *name)
{
    DateInfo di;
    if (!strcmp(name,"ISO"))
        return PyInt_FromLong(dateFromValue(self->value, &di)->date);
    else if (!strcmp(name,"year"))
        return PyInt_FromLong(dateFromValue(self->value, &di)->year);
    else if (!strcmp(name,"month"))
        return PyInt_FromLong(dateFromValue(self->value, &di)->month);
    else if (!strcmp(name,"day"))
        return PyInt_FromLong(dateFromValue(self->value, &di)->day);
    else if (!strcmp(name,"value"))
        return PyInt_FromLong(self->value);
    else if (!strcmp(name,"COM"))
        return PyInt_FromLong(self->value - COM_OFFSET);
    else if (!strcmp(name,"dayOfYear"))
        return PyInt_FromLong(dateFromValue(self->value, &di)->days);
    else if (!strcmp(name,"ticks"))
        return PyInt_FromLong(dateToTicks(self->value));
    else if (!strcmp(name,"week"))
        return PyInt_FromLong(dateToWeek(self->value));
    else if (!strcmp(name,"dayOfWeek"))
        return PyInt_FromLong((self->value - 1) % 7 + 1);
    else if (!strcmp(name,"leapYear"))
        return PyInt_FromLong(dateFromValue(self->value, &di)->leap);
    else if (!strcmp(name,"__members__"))
        return Py_BuildValue("[sssssssssss]",
                             "ISO","year","month","day","value","COM",
                             "dayOfYear","ticks","dayOfWeek","week","leapYear");
    else {
        return Py_FindMethod(Date_methods, (PyObject*) self, name);
    }
}       


static int
date_setattr(DateObject *self, char *name, PyObject *v)
{
    if (!strcmp(name,"value")) {
        if (!PyInt_Check(v)) {
            PyErr_SetString(PyExc_ValueError, "Value must be an int.");
            return -1;
        }
        self->value = PyInt_AS_LONG(v);
        return 0;
    } else
        return -1;
}

PyTypeObject Date_Type = {
    PyObject_HEAD_INIT(NULL)
    0, /* ob_size */
    "Date", /* tp_name */
    sizeof(DateObject), /* tp_size */
    0, /* tp_itemsize */
    /* methods */
    (destructor)date_dealloc, /* tp_dealloc */
    0, /* tp_print */
    (getattrfunc)date_getattr, /* tp_getattr */
    (setattrfunc)date_setattr, /* tp_setattr */
    (cmpfunc)date_compare,      /*tp_compare*/
    (reprfunc)date_repr,        /*tp_repr*/
    0,      /*tp_as_number*/
    0,      /*tp_as_sequence*/
    0,      /*tp_as_mapping*/
    (hashfunc)date_hash,        /*tp_hash*/
    (ternaryfunc)0,         /*tp_call*/
    (reprfunc)date_str,     /*tp_str*/
};


/* ==================================================================== */
/* python module interface */

static PyMethodDef date_functions[] = {
    {"Date", date_Date, METH_VARARGS, date_Date_doc},   
    {"fromISO", date_fromISO, METH_VARARGS, date_fromISO_doc},
    {"fromInt", date_fromInt, METH_VARARGS, date_fromInt_doc},
    {"fromCOM", date_fromCOM, METH_VARARGS, date_fromCOM_doc},
    {"fromTicks", date_fromTicks, METH_VARARGS, date_fromTicks_doc},
    {"fromYMD", date_fromYMD, METH_VARARGS, date_fromYMD_doc},
    {"easterSunday", date_easterSunday, METH_VARARGS, date_easterSunday_doc},
    {"isLeapYear", date_isLeapYear, METH_VARARGS, date_isLeapYear_doc},
    {"daysInMonth", date_daysInMonth, METH_VARARGS, date_daysInMonth_doc},
    {"parse", date_parse, METH_VARARGS, date_parse_doc},
    {NULL, NULL}
};

DL_EXPORT(void)
init_date(void)
{
    PyObject *m, *d;
    Date_Type.ob_type = &PyType_Type;
    m = Py_InitModule("_date", date_functions);
    d = PyModule_GetDict(m);
    PyDict_SetItemString(d, "avgYear", PyFloat_FromDouble(DAYSYEAR));
    PyDict_SetItemString(d, "avgMonth", PyFloat_FromDouble(DAYSYEAR / 12));
    Py_INCREF(&Date_Type);
    PyDict_SetItemString(d,"DateType", (PyObject *)&Date_Type);
}



