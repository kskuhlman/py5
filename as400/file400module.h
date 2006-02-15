#ifndef FILE400MODUKE_H
#define FILE400MODULE_H

#include <recio.h>

/* Field information structure */
typedef struct {
	char name[11];
	char desc[51];
	unsigned short type;
	long  offset;
	long  len;
	long  digits;
	long  dec;
} fieldInfoStruct;

/* Key information structure */
typedef struct {
	char name[11];
	unsigned short type;
	long  offset;
	long  len;
	long  digits;
	long  dec;
} keyInfoStruct;

/* File object type */
typedef struct {
	PyObject_HEAD
	char      file[11];
	char      lib[11];
	char      mbr[11];
	int      omode;
	int      qry;
	_RFILE    *fp;      /* file pointer */ 
	char      *recbuf;	/* record buffer */
	char      *keybuf;	/* key buffer */
	char      retFile[11]; /* returned file name */
	char      retLib[11];
	char      recName[11];
	char      recId[14];
	long      recLen;	
	long      keyLen;	
	long      fieldCount;	
	long      keyCount;	
	int       keyset;
	fieldInfoStruct *fieldArr;
    PyObject *fieldDict;	/* dictionary over fields */
	keyInfoStruct *keyArr;
    PyObject *keyDict;	    /* dictionary over keys */
} File400Object;

/* record object type */
typedef struct {
    PyObject_VAR_HEAD
	File400Object *file;
	char      recId[14];
	unsigned long  rrn;
	char      buffer[1];
} Record400Object;

extern DL_IMPORT(PyTypeObject) File400_Type;
extern DL_IMPORT(PyTypeObject) Record400_Type;

#define File400Object_Check(v) ((v)->ob_type == &File400_Type)
#define Record400Object_Check(v) ((v)->ob_type == &Record400_Type)


#endif /* FILE400MODULE_H */
