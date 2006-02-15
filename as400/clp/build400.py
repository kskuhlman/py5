
# conversion of crt_pgm.clp
lib, tgtRls = argv[1], argv[2]

os.call(pythonLib +"/" +"crt_srvpy", parm(lib, tgtRls)) # CALL PYTHON23/CRT_SRVPY PARM(&LIB &TGTRLS)

crtSrvCmd = """CRTSRVPGM  SRVPGM(%s/%s) MODULE(%s/%s) EXPORT(*ALL)
                   BNDSRVPGM(%s/%s) TGTRLS(%s)""" % (lib, srvPgm, lib, module,
                    lib, bndSrvPgm)
crtPgmCmd = """CRTPGM PGM(%s/%s) BNDSRVPGM(%s/%s) ALWLIBUPD(*YES)
                   TGTRLS(%s)""" % (lib, program, lib, srvPgm, tgtRls)
crtCmd = """CRTCMD CMD(%s/PYTHON) PGM(%s/PYTHONCMD) SRCFILE(%s/QCMDSRC)
             THDSAFE(*YES) TEXT(PYTHON)""" (lib, lib, pySrcLib)


crtCmds =   (("SrvPgm", "array", "arrayModul"),
             ("srv", "binascii", "binascii"),
             ("srv", "cmath", "cmathmodul"),
             ("srv", "cpickle", "cpickle"),
             ("srv", "cstringio", "cstringio"),
             ("srv", "zdate", "zdate"),
             ("srv", "datetime", "datetimemo"),
             ("srv", "errno", "errnomodul"),
             ("srv", "fcntl", "fcntlmodul"),
             ("srv", "imageop", "imageop"),
             ("srv", "math", "mathmodule"),
             ("srv", "md5", "md5"),
             ("srv", "file400", "file400mod"),
             ("srv", "zos400", "zos400"),
             ("srv", "parser", "parsermodu"),
             ("srv", "pure", "puremodule"),
             ("srv", "select", "selectmodu"),
             ("srv", "sgml", "sgmlmodule"),
             ("srv", "zsocket", "socketmodu"),
             ("srv", "zsre", "sre"),
             ("srv", "strop", "stropmodul"),
             ("srv", "struct", "structmod"),
             ("srv", "thread", "threadmodu"),
             ("srv", "time", "timemodule"),
             ("srv", "TIMING", "TIMINGMODU"),
             ("srv", "UNICODEDAT", "UNICODEDAT"),
             ("srv", "ZLIB", "ZLIBMODULE"),
             ("srv", "XREADLINES", "XREADLINES"),
             ("srv", "SHA", "SHAMODULE"),
             ("srv", "ZSYMTABLE", "SYMTABLEMO"),
             ("srv", "TESTCAPIMO", "TESTCAPIMO"),
             ("srv", "ZWEAKREF", "WEAKREF"),
             ("srv", "ZHOTSHOT", "HOTSHOT"),
             ("srv", "ITERTOOLS", "ITERTOOLSM"),
             ("srv", "ZCSV", "CSV"),
             ("srv", "ZRANDOM", "RANDOM"),
             ("srv", "ZDB", "ZDB"),


             ("pgm", "PYTHON", "PYTHON"),
             ("pgm", "PYTHONCMD", "PYTHON"))

crtCmdCmd = """CRTCMD CMD(&LIB/PYTHON) PGM(&LIB/PYTHONCMD)
             SRCFILE(PYTHON23/QCMDSRC) THDSAFE(*YES) TEXT(PYTHON)"""


# from crt_srvpy.clp
def crt_SrvPy(lib, tgtRls):
             modules = ["ABSTRACT", "ACCELER", "AS400MISC", "BITSET", "BLTINMODUL",
             "BUFFEROBJE", "BOOLOBJECT", "CEVAL", "CLASSOBJEC", "COBJECT", "CODECS",
             "COMPILE", "COMPLEXOBJ", "CODECSMODU", "CONFIG", "CELLOBJECT", "DESCROBJEC",
             "DICTOBJECT", "DYNLOAD_AS", "ENUMOBJECT", "ERRORS", "EXCEPTIONS",
             "FILEOBJECT", "FIRSTSETS", "FLOATOBJEC", "FRAMEOBJEC", "FROZEN",
             "FUTURE", "FUNCOBJECT", "GETARGS", "GETBUILDIN", "GETCOMPILE",
             "GETCOPYRIG", "GETMTIME", "GETPATH", "GETPLATFOR", "GETOPT",
             "GETVERSION", "GRAMINIT", "GRAMMAR", "GRAMMAR1", "IMPORT",
             "IMPORTDL", "INTOBJECT", "ITEROBJECT", "LISTNODE", "LISTOBJECT",
             "LOCALEMODU", "LONGOBJECT", "MAIN", "MARSHAL", "METAGRAMMA",
             "METHODOBJE", "MODSUPPORT", "MODULEOBJE", "MYREADLINE", "MYSTRTOUL",
             "MYSNPRINTF", "NODE", "OBMALLOC", "OBJECT", "OPERATOR", "PARSER",
             "PARSETOK", "PGEN", "POSIXMODUL", "PRINTGRAMM", "PYFPE", "PYSTATE",
             "PYTHONRUN", "RANGEOBJE","SIGNALMODU", "SLICEOBJE", "STRDUP",
             "STRINGOBJE", "STRUCTMEMB", "SYSMODULE", "THREAD", "TOKENIZER",
             "TRACEBACK", "TUPLEOBJE", "TYPEOBJE", "GCMODULE", "STRUCTSEQ",
             "SYMTABLE", "WEAKREFOBJ", "UNICODEOBJ", "UNICODECTY", "ZIPIMPORT"]

             crtSrvPgm(srvPgm="Python", module=modules, tgtRls=tgtRls)

# From crt_module.clp:
dfe crt_module(lib, opt, inline, dbg, tgt):
             CHGENVVAR(ENVVAR="INCLUDE", VALUE ='/source/python2.3/as400:' +
                       '/source/python2.3/modules:/source/python2.3:' +
                       '/source/python2.3/include:/qibm/proddata/ilec:' +
                       '/qibm/proddata/ilec/include:/qibm/include:/qibm/include/sys')
             crtCCmdCmd = """ CRTCMOD MODULE(&LIB/ITERTOOLSM)
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/ITERTOOLSMODULE.C')
                          OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) DBGVIEW(&DBG)
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)"""

             cMods =(("ITERTOOLSM", '/SOURCE/PYTHON2.3/MODULES/ITERTOOLSMODULE.C'),
               ("CSV", '/SOURCE/PYTHON2.3/MODULES/_CSV.C'),
               ("RANDOM", '/SOURCE/PYTHON2.3/MODULES/_RANDOMMODULE.C'),
               ("ZIPIMPORT", '/SOURCE/PYTHON2.3/MODULES/ZIPIMPORT.C'),
               ("SRE", '/SOURCE/PYTHON2.3/MODULES/_SRE.C'),
               ("POSIXMODUL", '/SOURCE/PYTHON2.3/MODULES/POSIXMODULE.C'),
               ("BINASCII", '/SOURCE/PYTHON2.3/MODULES/BINASCII.C'),
               ("CSTRINGIO", '/SOURCE/PYTHON2.3/MODULES/CSTRINGIO.C'),
               ("CPICKLE)", '/SOURCE/PYTHON2.3/MODULES/CPICKLE.C'),
               ("ERRNOMODUL", '/SOURCE/PYTHON2.3/MODULES/ERRNOMODULE.C'),
               ("FCNTLMODUL", '/SOURCE/PYTHON2.3/MODULES/FCNTLMODULE.C'),
               ("MD5C", '/SOURCE/PYTHON2.3/MODULES/MD5C.C'),
               ("MD5MODULE", '/SOURCE/PYTHON2.3/MODULES/MD5MODULE.C'),
               ("IMAGEOP", '/SOURCE/PYTHON2.3/MODULES/IMAGEOP.C'),
               ("PARSERMODU", '/SOURCE/PYTHON2.3/MODULES/PARSERMODULE.C'),
               ("PUREMODULE", '/SOURCE/PYTHON2.3/MODULES/PUREMODULE.C'),
               ("SELECTMODU", '/SOURCE/PYTHON2.3/MODULES/SELECTMODULE.C'),
               ("SHAMODULE", '/SOURCE/PYTHON2.3/MODULES/SHAMODULE.C'),
               ("SIGNALMODU", '/SOURCE/PYTHON2.3/MODULES/SIGNALMODULE.C'),
               ("SOCKETMODU", '/SOURCE/PYTHON2.3/MODULES/SOCKETMODULE.C'),
               ("THREADMODU", '/SOURCE/PYTHON2.3/MODULES/THREADMODULE.C'),
               ("TIMINGMODU", '/SOURCE/PYTHON2.3/MODULES/TIMINGMODULE.C'),
           /* REQUIRED */
               ("PYTHON", '/SOURCE/PYTHON2.3/MODULES/PYTHON.C'),
               ("CONFIG", '/SOURCE/PYTHON2.3/MODULES/CONFIG.C'),
               ("MAIN", '/SOURCE/PYTHON2.3/MODULES/MAIN.C'),
               ("ARRAYMODUL", '/SOURCE/PYTHON2.3/MODULES/ARRAYMODULE.C'),
               ("CMATHMODUL", '/SOURCE/PYTHON2.3/MODULES/CMATHMODULE.C'),
               ("GETPATH", '/SOURCE/PYTHON2.3/MODULES/GETPATH.C'),
               ("MATHMODULE", '/SOURCE/PYTHON2.3/MODULES/MATHMODULE.C'),
               ("STROPMODUL", '/SOURCE/PYTHON2.3/MODULES/STROPMODULE.C'),
               ("STRUCTMODU", '/SOURCE/PYTHON2.3/MODULES/STRUCTMODULE.C'),
                        /* IKKE BRUK *IFSIO FOR TIMEMODULE */
               ("TIMEMODULE", '/SOURCE/PYTHON2.3/MODULES/TIMEMODULE.C'),
               ("OPERATOR", '/SOURCE/PYTHON2.3/MODULES/OPERATOR.C'),
               ("CODECSMODU", '/SOURCE/PYTHON2.3/MODULES/_CODECSMODULE.C'),
               ("LOCALEMODU", '/SOURCE/PYTHON2.3/MODULES/_LOCALEMODULE.C'),
               ("UNICODEDAT", '/SOURCE/PYTHON2.3/MODULES/UNICODEDATA.C'),
               ("GETBUILDIN", '/SOURCE/PYTHON2.3/MODULES/GETBUILDINFO.C'),

               ("WEAKREF", '/SOURCE/PYTHON2.3/MODULES/_WEAKREF.C'),
               ("XREADLINES", '/SOURCE/PYTHON2.3/MODULES/XREADLINESMODULE.C'),
               ("SYMTABLEMO", '/SOURCE/PYTHON2.3/MODULES/SYMTABLEMODULE.C'),
               ("TESTCAPIMO", '/SOURCE/PYTHON2.3/MODULES/_TESTCAPIMODULE.C'),
               ("GCMODULE", '/SOURCE/PYTHON2.3/MODULES/GCMODULE.C'),
               ("HOTSHOT", '/SOURCE/PYTHON2.3/MODULES/_HOTSHOT.C'),
               ("DATETIMEMO", '/SOURCE/PYTHON2.3/MODULES/DATETIMEMODULE.C'),
                    )

# from crt_object.clp:
### TODO: Fix this
             PGM        PARM(&LIB &OPT &INLINE &DBG &TGT)
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)
             DCL        VAR(&OPT) TYPE(*CHAR) LEN(2)
             DCL        VAR(&INLINE) TYPE(*CHAR) LEN(4)
             DCL        VAR(&DBG) TYPE(*CHAR) LEN(10)
             DCL        VAR(&TGT) TYPE(*CHAR) LEN(10)
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/python2.3/as400:/source/pyth+
                          on2.3/objects:/source/python2.3:/source/pyt+
                          hon2.3/include:/qibm/proddata/ilec:/qibm/pr+
                          oddata/ilec/include:/qibm/include:/qibm/inc+
                          lude/sys')
             CRTCMOD    MODULE(&LIB/ABSTRACT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/ABSTRACT.C')
             CRTCMOD    MODULE(&LIB/BOOLOBJECT) +
                          SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/BOOLOBJE+
                          CT.C') OPTIMIZE(&OPT) INLINE(&INLINE +
                          *AUTO) DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)
             CRTCMOD    MODULE(&LIB/BUFFEROBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/BUFFEROBJECT.C')
             CRTCMOD    MODULE(&LIB/CLASSOBJEC) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/CLASSOBJECT.C')
             CRTCMOD    MODULE(&LIB/COBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/COBJECT.C')
             CRTCMOD    MODULE(&LIB/COMPLEXOBJ) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/COMPLEXOBJECT.C')
             CRTCMOD    MODULE(&LIB/DICTOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/DICTOBJECT.C')
             CRTCMOD    MODULE(&LIB/ENUMOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/ENUMOBJECT.C')
             CRTCMOD    MODULE(&LIB/FILEOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/FILEOBJECT.C')
             CRTCMOD    MODULE(&LIB/FLOATOBJEC) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/FLOATOBJECT.C')
             CRTCMOD    MODULE(&LIB/FRAMEOBJEC) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/FRAMEOBJECT.C')
             CRTCMOD    MODULE(&LIB/FUNCOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/FUNCOBJECT.C')
             CRTCMOD    MODULE(&LIB/INTOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/INTOBJECT.C')
             CRTCMOD    MODULE(&LIB/LISTOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/LISTOBJECT.C')
             CRTCMOD    MODULE(&LIB/LONGOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/LONGOBJECT.C')
             CRTCMOD    MODULE(&LIB/METHODOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/METHODOBJECT.C')
             CRTCMOD    MODULE(&LIB/MODULEOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/MODULEOBJECT.C')
             CRTCMOD    MODULE(&LIB/OBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/OBJECT.C')
             CRTCMOD    MODULE(&LIB/RANGEOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/RANGEOBJECT.C')
             CRTCMOD    MODULE(&LIB/SLICEOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/SLICEOBJECT.C')
             CRTCMOD    MODULE(&LIB/STRINGOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/STRINGOBJECT.C')
             CRTCMOD    MODULE(&LIB/TUPLEOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/TUPLEOBJECT.C')
             CRTCMOD    MODULE(&LIB/TYPEOBJE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/TYPEOBJECT.C')
             CRTCMOD    MODULE(&LIB/UNICODECTY) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/UNICODECTYPE.C')
             CRTCMOD    MODULE(&LIB/UNICODEOBJ) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/UNICODEOBJECT.C')
             CRTCMOD    MODULE(&LIB/CELLOBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/CELLOBJECT.C')
             CRTCMOD    MODULE(&LIB/WEAKREFOBJ) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/WEAKREFOBJECT.C')
             CRTCMOD    MODULE(&LIB/STRUCTSEQ) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/STRUCTSEQ.C')
             CRTCMOD    MODULE(&LIB/DESCROBJEC) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/DESCROBJECT.C')
             CRTCMOD    MODULE(&LIB/ITEROBJECT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/ITEROBJECT.C')
             CRTCMOD    MODULE(&LIB/OBMALLOC) +
                          SRCSTMF('/SOURCE/PYTHON2.3/OBJECTS/OBMALLOC+
                          .C') OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +
                          DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)
             ENDPGM

#From crt_parser.clp:
### TODO: Fix this
            PGM        PARM(&LIB &OPT &INLINE &DBG &TGT)
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)
             DCL        VAR(&OPT) TYPE(*CHAR) LEN(2)
             DCL        VAR(&INLINE) TYPE(*CHAR) LEN(4)
             DCL        VAR(&DBG) TYPE(*CHAR) LEN(10)
             DCL        VAR(&TGT) TYPE(*CHAR) LEN(10)
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/python2.3/as400:/source/pyth+
                          on2.3/parser:/source/python2.3:/source/pyth+
                          on2.3/include:/qibm/proddata/ilec:/qibm/pro+
                          ddata/ilec/include:/qibm/include:/qibm/incl+
                          ude/sys')
             CRTCMOD    MODULE(&LIB/ACCELER) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/ACCELER.C')
             CRTCMOD    MODULE(&LIB/BITSET) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/BITSET.C')
             CRTCMOD    MODULE(&LIB/FIRSTSETS) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/FIRSTSETS.C')
             CRTCMOD    MODULE(&LIB/GRAMMAR) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/GRAMMAR.C')
             CRTCMOD    MODULE(&LIB/GRAMMAR1) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/GRAMMAR1.C')
             CRTCMOD    MODULE(&LIB/LISTNODE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/LISTNODE.C')
             CRTCMOD    MODULE(&LIB/METAGRAMMA) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/METAGRAMMAR.C')
             CRTCMOD    MODULE(&LIB/MYREADLINE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/MYREADLINE.C')
             CRTCMOD    MODULE(&LIB/NODE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/NODE.C')
             CRTCMOD    MODULE(&LIB/PARSER) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/PARSER.C')
             CRTCMOD    MODULE(&LIB/PARSETOK) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/PARSETOK.C')
             CRTCMOD    MODULE(&LIB/PGEN) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/PGEN.C')
             CRTCMOD    MODULE(&LIB/PRINTGRAMM) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/PRINTGRAMMAR.C')
             CRTCMOD    MODULE(&LIB/TOKENIZER) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PARSER/TOKENIZER.C')
             ENDPGM
# From crt_pgm.clp:
### TODO: Fix this
            PGM        PARM(&LIB &TGTRLS)
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)
             DCL        VAR(&TGTRLS) TYPE(*CHAR) LEN(10)

             CALL PYTHON23/CRT_SRVPY PARM(&LIB &TGTRLS)

             CRTSRVPGM  SRVPGM(&LIB/ARRAY) MODULE(&LIB/ARRAYMODUL) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/BINASCII) MODULE(&LIB/BINASCII) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/CMATH) MODULE(&LIB/CMATHMODUL) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/CPICKLE) MODULE(&LIB/CPICKLE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/CSTRINGIO) +
                          MODULE(&LIB/CSTRINGIO) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZDATE) MODULE(&LIB/ZDATE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/DATETIME) MODULE(&LIB/DATETIMEMO) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ERRNO) MODULE(&LIB/ERRNOMODUL) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/FCNTL) MODULE(&LIB/FCNTLMODUL) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/IMAGEOP) MODULE(&LIB/IMAGEOP) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/MATH) MODULE(&LIB/MATHMODULE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/MD5) MODULE(&LIB/MD5*) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/FILE400) MODULE(&LIB/FILE400MOD) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZOS400) MODULE(&LIB/ZOS400) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/PARSER) MODULE(&LIB/PARSERMODU) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/PURE) MODULE(&LIB/PUREMODULE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/SELECT) MODULE(&LIB/SELECTMODU) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/SGML) MODULE(&LIB/SGMLMODULE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZSOCKET) MODULE(&LIB/SOCKETMODU) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZSRE) MODULE(&LIB/SRE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/STROP) MODULE(&LIB/STROPMODUL) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/STRUCT) MODULE(&LIB/STRUCTMODU) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/THREAD) MODULE(&LIB/THREADMODU) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/TIME) MODULE(&LIB/TIMEMODULE) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/TIMING) MODULE(&LIB/TIMINGMODU) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/UNICODEDAT) +
                          MODULE(&LIB/UNICODEDAT) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZLIB) MODULE(&LIB/ZLIBMODULE +
                          &LIB/Z_*) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/XREADLINES) +
                          MODULE(&LIB/XREADLINES) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/SHA) +
                          MODULE(&LIB/SHAMODULE) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZSYMTABLE) +
                          MODULE(&LIB/SYMTABLEMO) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/TESTCAPIMO) +
                          MODULE(&LIB/TESTCAPIMO) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZWEAKREF) +
                          MODULE(&LIB/WEAKREF) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZHOTSHOT) +
                          MODULE(&LIB/HOTSHOT) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ITERTOOLS) +
                          MODULE(&LIB/ITERTOOLSM) EXPORT(*ALL) +
                          BNDSRVPGM(&LIB/PYTHON) TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZCSV) MODULE(&LIB/CSV) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZRANDOM) MODULE(&LIB/RANDOM) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)
             CRTSRVPGM  SRVPGM(&LIB/ZDB) MODULE(&LIB/ZDB) +
                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
                          TGTRLS(&TGTRLS)

             CRTPGM     PGM(&LIB/PYTHON) BNDSRVPGM(&LIB/PYTHON) +
                          ALWLIBUPD(*YES) TGTRLS(&TGTRLS)
             CRTPGM     PGM(&LIB/PYTHONCMD) BNDSRVPGM(&LIB/PYTHON) +
                          ALWLIBUPD(*YES) TGTRLS(&TGTRLS)
             CRTCMD     CMD(&LIB/PYTHON) PGM(&LIB/PYTHONCMD) +
                          SRCFILE(PYTHON23/QCMDSRC) THDSAFE(*YES) +
                          TEXT(PYTHON)
             ENDPGM
#From crt_python.clp:
### TODO: Fix this
            PGM        PARM(&LIB &OPT &INLINE &DBG &TGT)
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)
             DCL        VAR(&OPT) TYPE(*CHAR) LEN(2)
             DCL        VAR(&INLINE) TYPE(*CHAR) LEN(4)
             DCL        VAR(&DBG) TYPE(*CHAR) LEN(10)
             DCL        VAR(&TGT) TYPE(*CHAR) LEN(10)
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/python2.3/as400:/source/pyth+
                          on2.3/python:/source/python2.3:/source/pyth+
                          on2.3/include:/qibm/proddata/ilec:/qibm/pro+
                          ddata/ilec/include:/qibm/include:/qibm/incl+
                          ude/sys')
             CRTCMOD    MODULE(&LIB/BLTINMODUL) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/BLTINMODULE.C')
             CRTCMOD    MODULE(&LIB/CEVAL) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/CEVAL.C')
             CRTCMOD    MODULE(&LIB/CODECS) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/CODECS.C')
             CRTCMOD    MODULE(&LIB/COMPILE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/COMPILE.C')
             CRTCMOD    MODULE(&LIB/ERRORS) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/ERRORS.C')
             CRTCMOD    MODULE(&LIB/EXCEPTIONS) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/EXCEPTIONS.C')
             CRTCMOD    MODULE(&LIB/FROZEN) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/FROZEN.C')
             CRTCMOD    MODULE(&LIB/FROZENMAIN) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/FROZENMAIN.C')
             CRTCMOD    MODULE(&LIB/GETARGS) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETARGS.C')
             CRTCMOD    MODULE(&LIB/GETCOMPILE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETCOMPILER.C')
             CRTCMOD    MODULE(&LIB/GETCOPYRIG) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETCOPYRIGHT.C')
             CRTCMOD    MODULE(&LIB/GETMTIME) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETMTIME.C')
             CRTCMOD    MODULE(&LIB/GETPLATFOR) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETPLATFORM.C')
             CRTCMOD    MODULE(&LIB/GETVERSION) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETVERSION.C')
             CRTCMOD    MODULE(&LIB/GRAMINIT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GRAMINIT.C')
             CRTCMOD    MODULE(&LIB/IMPORT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/IMPORT.C')
             CRTCMOD    MODULE(&LIB/IMPORTDL) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/IMPORTDL.C')
             CRTCMOD    MODULE(&LIB/MARSHAL) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/MARSHAL.C')
             CRTCMOD    MODULE(&LIB/MODSUPPORT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/MODSUPPORT.C')
             CRTCMOD    MODULE(&LIB/MYSNPRINTF) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/MYSNPRINTF.C')
             CRTCMOD    MODULE(&LIB/MYSTRTOUL) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/MYSTRTOUL.C')
             CRTCMOD    MODULE(&LIB/PYFPE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/PYFPE.C')
             CRTCMOD    MODULE(&LIB/PYSTATE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/PYSTATE.C')
             CRTCMOD    MODULE(&LIB/PYTHONRUN) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/PYTHONRUN.C')
             CRTCMOD    MODULE(&LIB/STRDUP) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/STRDUP.C')
             CRTCMOD    MODULE(&LIB/STRUCTMEMB) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/STRUCTMEMBER.C')
             CRTCMOD    MODULE(&LIB/SYSMODULE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/SYSMODULE.C')
             CRTCMOD    MODULE(&LIB/THREAD) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/THREAD.C')
             CRTCMOD    MODULE(&LIB/TRACEBACK) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/TRACEBACK.C')
             CRTCMOD    MODULE(&LIB/FUTURE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/FUTURE.C')
             CRTCMOD    MODULE(&LIB/SYMTABLE) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/SYMTABLE.C')
             CRTCMOD    MODULE(&LIB/GETOPT) OPTIMIZE(&OPT) +
                        INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                        SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/GETOPT.C')
             ENDPGM
#From crt_zlib.clp:
### TODO: Fix this
def crt_zlib(lib, opt, inline, dbg, tgt): 
             PGM        PARM(&LIB &OPT &INLINE &DBG &TGT)
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)
             DCL        VAR(&OPT) TYPE(*CHAR) LEN(2)
             DCL        VAR(&INLINE) TYPE(*CHAR) LEN(4)
             DCL        VAR(&DBG) TYPE(*CHAR) LEN(10)
             DCL        VAR(&TGT) TYPE(*CHAR) LEN(10)
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/zlib1.1.4:/source/python2.3+
                          /modules:/source/python2.3:/source/python+
                          2.3/include:/qibm/proddata/ilec:/qibm/pr+
                          oddata/ilec/include:/qibm/include:/qibm/inc+
                          lude/sys')

             CRTCMOD    MODULE(&LIB/ZLIBMODULE) +
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/ZLIBMODU+
                          LE.C') OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                          DBGVIEW(&DBG) TGTRLS(&TGT)
             CRTCMOD    MODULE(&LIB/Z_ADLER32) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/ADLER32.C')
             CRTCMOD    MODULE(&LIB/Z_COMPRESS) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/COMPRESS.C') +
                          TERASPACE(*YES *TSIFC)
             CRTCMOD    MODULE(&LIB/Z_CRC32) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/CRC32.C')
             CRTCMOD    MODULE(&LIB/Z_DEFLATE) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/DEFLATE.C')
             CRTCMOD    MODULE(&LIB/Z_GZIO) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/GZIO.C')
             CRTCMOD    MODULE(&LIB/Z_INFBLOCK) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/INFBLOCK.C')
             CRTCMOD    MODULE(&LIB/Z_INFCODES) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/INFCODES.C')
             CRTCMOD    MODULE(&LIB/Z_INFFAST) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/INFFAST.C')
             CRTCMOD    MODULE(&LIB/Z_INFLATE) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/INFLATE.C')
             CRTCMOD    MODULE(&LIB/Z_INFTREES) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/INFTREES.C')
             CRTCMOD    MODULE(&LIB/Z_INFUTIL) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/INFUTIL.C')
             CRTCMOD    MODULE(&LIB/Z_TREES) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/TREES.C')
             CRTCMOD    MODULE(&LIB/Z_UNCOMPR) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/UNCOMPR.C')
             CRTCMOD    MODULE(&LIB/Z_ZUTIL) OPTIMIZE(&OPT) +
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                        SRCSTMF('/SOURCE/ZLIB1.1.4/ZUTIL.C')
             ENDPGM
#From crt_as400.clp:
### TODO: Fix this
def crt_as400(lib, opt, inline, dbg, tgt):
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/PYTHON2.3/as400:/source/pyth+
                          on2.3:/source/PYTHON2.3/include:/source/pyt  +
                          hon2.3/python:/qibm/proddata/ilec:/qibm/pro  +
                          ddata/ilec/include:/qibm/include:/qibm/incl+
                          ude/sys')

             CRTCMOD    MODULE(&LIB/FILE400MOD) OPTIMIZE(&OPT) +
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/FILE400MODULE.C')
          /* ALWAYS COMPILE THESE NEXT 2 WITH NO OPTIMIZE */
             CRTCMOD    MODULE(&LIB/ZOS400) OPTIMIZE(10) +
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/_OS400.C'     )
             CRTCMOD    MODULE(&LIB/AS400MISC) OPTIMIZE(10) +
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/AS400MISC.C')
             CRTCMOD    MODULE(&LIB/DYNLOAD_AS) +
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/DYNLOAD_AS400.C')
             CRTCMOD    MODULE(&LIB/PYTHONCMD) OPTIMIZE(&OPT) +
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/PYTHONCMD.C')
             CRTCMOD    MODULE(&LIB/ZDB) +
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/_DB.C') +
             CRTCMOD    MODULE(&LIB/SGMLMODULE) +
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/SGMLMODULE.C') +
             CRTCMOD    MODULE(&LIB/ZDATE) +
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/_DATE.C') +
             ENDPGM
