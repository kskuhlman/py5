
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
def crt_module(lib, opt, inline, dbg, tgt):
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
def crt_object(lib, opt, inline, dbg, tgt):
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
             (("BOOLOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/BOOLOBJECT.C'),
             ("BUFFEROBJE", '/SOURCE/PYTHON2.3/OBJECTS/BUFFEROBJECT.C'),
             ("CLASSOBJEC", '/SOURCE/PYTHON2.3/OBJECTS/CLASSOBJECT.C'),
             ("COBJECT", '/SOURCE/PYTHON2.3/OBJECTS/COBJECT.C'),
             ("COMPLEXOBJ", '/SOURCE/PYTHON2.3/OBJECTS/COMPLEXOBJECT.C'),
             ("DICTOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/DICTOBJECT.C'),
             ("ENUMOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/ENUMOBJECT.C'),
             ("FILEOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/FILEOBJECT.C'),
             ("FLOATOBJEC", '/SOURCE/PYTHON2.3/OBJECTS/FLOATOBJECT.C'),
             ("FRAMEOBJEC", '/SOURCE/PYTHON2.3/OBJECTS/FRAMEOBJECT.C'),
             ("FUNCOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/FUNCOBJECT.C'),
             ("INTOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/INTOBJECT.C'),
             ("LISTOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/LISTOBJECT.C'),
             ("LONGOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/LONGOBJECT.C'),
             ("METHODOBJE", '/SOURCE/PYTHON2.3/OBJECTS/METHODOBJECT.C'),
             ("MODULEOBJE", '/SOURCE/PYTHON2.3/OBJECTS/MODULEOBJECT.C'),
             ("OBJECT", '/SOURCE/PYTHON2.3/OBJECTS/OBJECT.C'),
             ("RANGEOBJE", '/SOURCE/PYTHON2.3/OBJECTS/RANGEOBJECT.C'),
             ("SLICEOBJE", '/SOURCE/PYTHON2.3/OBJECTS/SLICEOBJECT.C'),
             ("STRINGOBJE", '/SOURCE/PYTHON2.3/OBJECTS/STRINGOBJECT.C'),
             ("TUPLEOBJE", '/SOURCE/PYTHON2.3/OBJECTS/TUPLEOBJECT.C'),
             ("TYPEOBJE", '/SOURCE/PYTHON2.3/OBJECTS/TYPEOBJECT.C'),
             ("UNICODECT", '/SOURCE/PYTHON2.3/OBJECTS/UNICODECTYPE.C'),
             ("UNICODEOBJ", '/SOURCE/PYTHON2.3/OBJECTS/UNICODEOBJECT.C'),
             ("CELLOBJECT", '/SOURCE/PYTHON2.3/OBJECTS/CELLOBJECT.C'),
             ("WEAKREFOBJ", '/SOURCE/PYTHON2.3/OBJECTS/WEAKREFOBJECT.C'),
             ("STRUCTSEQ", '/SOURCE/PYTHON2.3/OBJECTS/STRUCTSEQ.C'),
             ("DESCROBJEC", '/SOURCE/PYTHON2.3/OBJECTS/DESCROBJECT.C'),
             ("ITEROBJECT", '/SOURCE/PYTHON2.3/OBJECTS/ITEROBJECT.C'),
             ("OBMALLOC", '/SOURCE/PYTHON2.3/OBJECTS/OBMALLOC.C')
             )

#From crt_parser.clp:
### TODO: Fix this
def crt_CModule(module, path, lib, opt, inline, dbg, tgt):
             os.cmd("""CRTCMOD MODULE(%s/%s) OPTIMIZE(%s) INLINE(%s *AUTO)
                DBGVIEW(%s) TGTRLS(%s) SYSIFCOPT(*IFSIO)
                LOCALETYPE(*LOCALEUCS2) SRCSTMF('%s')""" % (lib, module, opt,
                          inline, dbg, tgt, path)

def crt_parser(lib, opt, inline, dbg, tgt):
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/python2.3/as400:/source/pyth+
                          on2.3/parser:/source/python2.3:/source/pyth+
                          on2.3/include:/qibm/proddata/ilec:/qibm/pro+
                          ddata/ilec/include:/qibm/include:/qibm/incl+
                          ude/sys')

             ("BITSET", '/SOURCE/PYTHON2.3/PARSER/BITSET.C')
             ("FIRSTSETS", '/SOURCE/PYTHON2.3/PARSER/FIRSTSETS.C')
             ("GRAMMAR", '/SOURCE/PYTHON2.3/PARSER/GRAMMAR.C')
             ("GRAMMAR1", '/SOURCE/PYTHON2.3/PARSER/GRAMMAR1.C')
             ("LISTNODE", '/SOURCE/PYTHON2.3/PARSER/LISTNODE.C')
             ("METAGRAMMA", '/SOURCE/PYTHON2.3/PARSER/METAGRAMMAR.C')
             ("MYREADLINE", '/SOURCE/PYTHON2.3/PARSER/MYREADLINE.C')
             ("NODE", '/SOURCE/PYTHON2.3/PARSER/NODE.C')
             ("PARSER", '/SOURCE/PYTHON2.3/PARSER/PARSER.C')
             ("PARSETOK", '/SOURCE/PYTHON2.3/PARSER/PARSETOK.C')
             ("PGEN", '/SOURCE/PYTHON2.3/PARSER/PGEN.C')
             ("PRINTGRAMM", '/SOURCE/PYTHON2.3/PARSER/PRINTGRAMMAR.C')
             ("TOKENIZER", '/SOURCE/PYTHON2.3/PARSER/TOKENIZER.C')

def crtPgm(lib, pgm, srvPgm, tgtRls):
             sys.callstats("""CRTPGM PGM(%s/%s) BNDSRVPGM(%s/%s) +
                          ALWLIBUPD(*YES) TGTRLS(%s)""" % (lib, pgm, lib,
                                                    srvPgm, tgtRls))

def crtCmd(lib, cmd, pgm, text=None, thdSafe="*NO"):
             sys.cmd("""CRTCMD CMD(%s/%s) PGM(%s/%s) SRCFILE(%s/QCMDSRC)
                     THDSAFE(%s) TEXT(%s)""" % (lib, cmd, lib, pgm, pySrcLib,
                                                thdSafe)

# From crt_pgm.clp:
def crt_pgm(lib, tgtRls)
             crt_srvpy(lib, tgtRls)

#             CRTSRVPGM  SRVPGM(&LIB/ARRAY) MODULE(&LIB/ARRAYMODUL) +
#                          EXPORT(*ALL) BNDSRVPGM(&LIB/PYTHON) +
#                          TGTRLS(&TGTRLS)
              #(SrvPgm, Module)
             (("BINASCII", "BINASCII"),
             ("CMATH", "CMATHMODUL"),
             ("CPICKLE", "CPICKLE"),
             ("CSTRINGIO", "CSTRINGIO"),
             ("ZDATE", "ZDATE"),
             ("DATETIME", "DATETIMEMO"),
             ("ERRNO", "ERRNOMODUL"),
             ("FCNTL", "FCNTLMODUL"),
             ("IMAGEOP", "IMAGEOP"),
             ("MATH", "MATHMODULE"),
             ("MD5", "MD5*"),
             ("FILE400", "FILE400MOD"),
             ("ZOS400", "ZOS400"),
             ("PARSER", "PARSERMODU"),
             ("PURE", "PUREMODULE"),
             ("SELECT", "SELECTMODU"),
             ("SGML", "SGMLMODULE"),
             ("ZSOCKET", "SOCKETMODU"),
             ("ZSRE", "SRE"),
             ("STROP", "STROPMODUL"),
             ("STRUCT", "STRUCTMODU"),
             ("THREAD", "THREADMODU"),
             ("TIME", "TIMEMODULE"),
             ("TIMING", "TIMINGMODU"),
             ("UNICODEDAT", "UNICODEDAT"),
             ("ZLIB", ("ZLIBMODULE &LIB/Z_*")),
             ("XREADLINES", "XREADLINES"),
             ("SHA", "SHAMODULE"),
             ("ZSYMTABLE", "SYMTABLEMO"),
             ("TESTCAPIMO", "TESTCAPIMO"),
             ("ZWEAKREF", "WEAKREF"),
             ("ZHOTSHOT", "HOTSHOT"),
             ("ITERTOOLS", "ITERTOOLSM"),
             ("ZCSV", "CSV"),
             ("ZRANDOM", "RANDOM"),
             ("ZDB", "ZDB")

             crtPgm (lib, "python", "python", tgtRls)
             crtPgm (lib, "pythoncmd", "python", tgtRls)

             crtCmd (lib, "Python", pgm="PythonCmd", text="Python", thdSafe="*YES")

#From crt_python.clp:
### TODO: Fix this
def crt_python (lib, opt, inline, dbg, tgt):
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE('/source/python2.3/as400:/source/pyth+
                          on2.3/python:/source/python2.3:/source/pyth+
                          on2.3/include:/qibm/proddata/ilec:/qibm/pro+
                          ddata/ilec/include:/qibm/include:/qibm/incl+
                          ude/sys')
             ("BLTINMODUL SRCSTMF('/SOURCE/PYTHON2.3/PYTHON/BLTINMODULE.C')
             ("CEVAL", '/SOURCE/PYTHON2.3/PYTHON/CEVAL.C')
             ("CODECS", '/SOURCE/PYTHON2.3/PYTHON/CODECS.C')
             ("COMPILE", '/SOURCE/PYTHON2.3/PYTHON/COMPILE.C')
             ("ERRORS", '/SOURCE/PYTHON2.3/PYTHON/ERRORS.C')
             ("EXCEPTIONS", '/SOURCE/PYTHON2.3/PYTHON/EXCEPTIONS.C')
             ("FROZEN", '/SOURCE/PYTHON2.3/PYTHON/FROZEN.C')
             ("FROZENMAIN", '/SOURCE/PYTHON2.3/PYTHON/FROZENMAIN.C')
             ("GETARGS", '/SOURCE/PYTHON2.3/PYTHON/GETARGS.C')
             ("GETCOMPILE", '/SOURCE/PYTHON2.3/PYTHON/GETCOMPILER.C')
             ("GETCOPYRIG", '/SOURCE/PYTHON2.3/PYTHON/GETCOPYRIGHT.C')
             ("GETMTIME", '/SOURCE/PYTHON2.3/PYTHON/GETMTIME.C')
             ("GETPLATFOR", '/SOURCE/PYTHON2.3/PYTHON/GETPLATFORM.C')
             ("GETVERSION", '/SOURCE/PYTHON2.3/PYTHON/GETVERSION.C')
             ("GRAMINIT", '/SOURCE/PYTHON2.3/PYTHON/GRAMINIT.C')
             ("IMPORT", '/SOURCE/PYTHON2.3/PYTHON/IMPORT.C')
             ("IMPORTDL", '/SOURCE/PYTHON2.3/PYTHON/IMPORTDL.C')
             ("MARSHAL", '/SOURCE/PYTHON2.3/PYTHON/MARSHAL.C')
             ("MODSUPPORT", '/SOURCE/PYTHON2.3/PYTHON/MODSUPPORT.C')
             ("MYSNPRINTF", '/SOURCE/PYTHON2.3/PYTHON/MYSNPRINTF.C')
             ("MYSTRTOUL", '/SOURCE/PYTHON2.3/PYTHON/MYSTRTOUL.C')
             ("PYFPE", '/SOURCE/PYTHON2.3/PYTHON/PYFPE.C')
             ("PYSTATE", '/SOURCE/PYTHON2.3/PYTHON/PYSTATE.C')
             ("PYTHONRUN", '/SOURCE/PYTHON2.3/PYTHON/PYTHONRUN.C')
             ("STRDUP", '/SOURCE/PYTHON2.3/PYTHON/STRDUP.C')
             ("STRUCTMEMB", '/SOURCE/PYTHON2.3/PYTHON/STRUCTMEMBER.C')
             ("SYSMODULE", '/SOURCE/PYTHON2.3/PYTHON/SYSMODULE.C')
             ("THREAD", '/SOURCE/PYTHON2.3/PYTHON/THREAD.C')
             ("TRACEBACK", '/SOURCE/PYTHON2.3/PYTHON/TRACEBACK.C')
             ("FUTURE", '/SOURCE/PYTHON2.3/PYTHON/FUTURE.C')
             ("SYMTABLE", '/SOURCE/PYTHON2.3/PYTHON/SYMTABLE.C')
             ("GETOPT", '/SOURCE/PYTHON2.3/PYTHON/GETOPT.C')
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
                          VALUE="""/source/zlib1.1.4:/source/python2.3/modules:
                          /source/python2.3:/source/python2.3/include:
                          /qibm/proddata/ilec:/qibm/proddata/ilec/include:
                          /qibm/include:/qibm/include/sys"""

             ("ZLIBMODULE", '/SOURCE/PYTHON2.3/MODULES/ZLIBMODULE.C')
             ("Z_ADLER32", '/SOURCE/ZLIB1.1.4/ADLER32.C')
             # Compile this next one with TERASPACE(*YES *TSIFC)
             ("Z_COMPRESS", '/SOURCE/ZLIB1.1.4/COMPRESS.C')
             ("Z_CRC32", '/SOURCE/ZLIB1.1.4/CRC32.C')
             ("Z_DEFLATE", '/SOURCE/ZLIB1.1.4/DEFLATE.C')
             ("Z_GZIO", '/SOURCE/ZLIB1.1.4/GZIO.C')
             ("Z_INFBLOCK", '/SOURCE/ZLIB1.1.4/INFBLOCK.C')
             ("Z_INFCODES", '/SOURCE/ZLIB1.1.4/INFCODES.C')
             ("Z_INFFAST", '/SOURCE/ZLIB1.1.4/INFFAST.C')
             ("Z_INFLATE", '/SOURCE/ZLIB1.1.4/INFLATE.C')
             ("Z_INFTREES", '/SOURCE/ZLIB1.1.4/INFTREES.C')
             ("Z_INFUTIL", '/SOURCE/ZLIB1.1.4/INFUTIL.C')
             ("Z_TREES", '/SOURCE/ZLIB1.1.4/TREES.C')
             ("Z_UNCOMPR", '/SOURCE/ZLIB1.1.4/UNCOMPR.C')
             ("Z_ZUTIL", '/SOURCE/ZLIB1.1.4/ZUTIL.C')

#From crt_as400.clp:
### TODO: Fix this
def crt_as400(lib, opt, inline, dbg, tgt):
             CHGENVVAR  ENVVAR(INCLUDE) +
                          VALUE("""/source/PYTHON2.3/as400:/source/python2.3:
                          /source/PYTHON2.3/include:/source/python2.3/python:
                          /qibm/proddata/ilec:/qibm/proddata/ilec/include:
                          /qibm/include:/qibm/include/sys')
             crtCModCmd = """  CRTCMOD    MODULE(&LIB/ZDATE) +
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/_DATE.C') +
                          OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +
                          DBGVIEW(&DBG) TGTRLS(&TGT)"""

             (("FILE400MOD", '/SOURCE/PYTHON2.3/AS400/FILE400MODULE.C'),
          /* ALWAYS COMPILE THESE NEXT 2 WITH NO OPTIMIZE */
             ("ZOS400", '/SOURCE/PYTHON2.3/AS400/_OS400.C'),
             ("AS400MISC", '/SOURCE/PYTHON2.3/AS400/AS400MISC.C'),

             ("DYNLOAD_AS", '/SOURCE/PYTHON2.3/AS400/DYNLOAD_AS400.C'),
             ("PYTHONCMD", '/SOURCE/PYTHON2.3/AS400/PYTHONCMD.C'),
             ("ZDB", '/SOURCE/PYTHON2.3/AS400/_DB.C'),
             ("SGMLMODULE", '/SOURCE/PYTHON2.3/AS400/SGMLMODULE.C'),
             ("&LIB/ZDATE", '/SOURCE/PYTHON2.3/AS400/_DATE.C'),
             ENDPGM
