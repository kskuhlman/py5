#ifndef DATEMODULE_H
#define DATEMODULE_H

/* date type definition */
typedef struct {
    PyObject_HEAD
    long value; /* 1 = 0000.01.01 (calculated) */
} DateObject;
/* additional date info */
typedef struct {
    long value;
	long date;
    int year;
    int month;
    int day;
	int days;
	int leap;
} DateInfo;

/* date module functions */
extern DL_IMPORT(PyTypeObject) Date_Type;

#define Date_Check(op) ((op)->ob_type == &Date_Type)

extern DL_IMPORT(long) dateToValue(int year, int month, int day);
extern DL_IMPORT(DateInfo *) dateFromValue(long value, DateInfo *di);
extern DL_IMPORT(long) dateToTicks(long value);
extern DL_IMPORT(int) dateToWeek(long value);
extern DL_IMPORT(long) dateEasterSunday(int year);
extern DL_IMPORT(int) dateIsLeapYear(int year);
extern DL_IMPORT(long) dateAddMonths(long value, int months);
extern DL_IMPORT(long) dateParse(char *s, char *format, char datesep, int year4);
extern DL_IMPORT(PyObject *) dateNew(long value);
extern DL_IMPORT(PyObject *) dateNewTicks(long ticks);

#endif /* !DATEMODULE_H */
