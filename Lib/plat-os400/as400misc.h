#ifndef AS400MISC_H
#define AS400MISC_H
#include <python.h>
#include <qleawi.h>
#include <iconv.h>

extern DL_IMPORT(void)initConversion(char *ccsid, iconv_t *cdfrom, iconv_t *cdto);
extern DL_IMPORT(void)convertChar(iconv_t cd, char *in, char *out, int size);
extern DL_IMPORT(char *)convertString(iconv_t cd, char *in, char *out);
extern DL_IMPORT(void)toCp37(char *in, char *out, int size);
extern DL_IMPORT(void)fromCp37(char *in, char *out, int size);
extern DL_IMPORT(char *)strToCp37(char *in, char *out);
extern DL_IMPORT(char *)strFromCp37(char *in, char *out);
extern DL_IMPORT(int) isalpha37(int c);
extern DL_IMPORT(int) isalnum37(int c);
extern DL_IMPORT(char *) strtostr400(char *asstr, char *str, int len);
extern DL_IMPORT(char *) str400tostr(char *str, char *asstr, int len);
extern DL_IMPORT(char *) strtoupper(char *str);
extern DL_IMPORT(char *) zonedtostr(char *buf, char *p, int digits, int dec, char decsep);
extern DL_IMPORT(char *) packedtostr(char *buf, char *p, int digits, int dec, char decsep);
extern DL_IMPORT(char *) getpythonhome(void);
extern DL_IMPORT(_SYSPTR) getPgm(char *name, char *lib);
extern DL_IMPORT(int) getSrvpgm(char *name, char *lib);
extern DL_IMPORT(_SYSPTR) getFunction(int actmark, char *name);


#endif /* AS400MISC.H */
