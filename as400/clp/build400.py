from sys import stdin, stdout, argv, exit
import os
try: import os400
except:
             print "Not running on as400. Faking calls"
             class OS400Call:
                          def __call__(self, *args):
                                       print "os400.Program called", args
             class OS400:
                          def Program(self, *args):
                                       print "os400.Program", args
                                       return OS400Call()
             os400 = OS400()

#TODO: Only re-create if source is newer than object
def execCmd(cmd):
             #TODO: There is a limit on length of strings printed to STDOUT. Find & Fix!
             debug = 9
             if debug:
                          print "Executing command:\n"
                          remain = cmd
                          while remain:
                                       print remain[:80]
                                       remain = remain[80:]
             cmdRef = os400.Program("QCMDEXC", "*LIBL", (('c',2048), ('d',15,5)))
             cmdRef(cmd, len(cmd))

def crtCModule(module, path, lib=None, opt=30, inline="*ON *AUTO", dbg="*NONE", tgtRls="*CURRENT"):
             if not lib: lib=targetLibrary
             execCmd("""CRTCMOD MODULE(%s/%s) SRCSTMF('%s') OPTIMIZE(%s) INLINE(%s)  \
                    DBGVIEW(%s) TGTRLS(%s) SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) \
                    """ % (lib, module, path, opt, inline, dbg, tgtRls))
def crtSrvPgm(srvPgm, modules=None, bndSrvPgm=None, lib=None, tgtRls="*CURRENT"):
             if not lib: lib=targetLibrary
             if not modules: modules = srvPgm
             bndSrvPgmTxt = ' '
             if bndSrvPgm:
                          bndSrvPgmTxt = 'BNDSRVPGM(%s)' % bndSrvPgm
             modParm = ''
             if not isinstance(modules, list): modules = [modules]
             for module in modules:
                          modParm += '%s/%s ' % (lib, module)
             execCmd("""CRTSRVPGM  SRVPGM(%s/%s) MODULE(%s) EXPORT(*ALL) \
                     %s TGTRLS(%s)"""
                     % (lib, srvPgm, modParm, bndSrvPgmTxt, tgtRls))
def crtCmd(cmd, pgm, text="", thdSafe="*YES"):
             lib, pySrcLib = targetLibrary, targetLibrary
             execCmd("""CRTCMD CMD(%s/%s) PGM(%s/%s) SRCFILE(%s/QCMDSRC) THDSAFE(%s) TEXT(%s)"""
             % (lib, cmd, lib, pgm, pySrcLib, thdSafe, text))
def crtPgm(pgm, srvPgm, lib=None, tgtRls="*CURRENT" ):
             if not lib: lib=targetLibrary
             execCmd("""CRTPGM PGM(%s/%s) BNDSRVPGM(%s/%s) ALWLIBUPD(*YES) TGTRLS(%s)"""
                      % (lib, pgm, lib, srvPgm, tgtRls))

moduleMods ={'ITERTOOLSM':'ITERTOOLSMODULE.C','CSV':'_CSV.C','RANDOM':'_RANDOMMODULE.C',
             'ZIPIMPORT':'ZIPIMPORT.C','SRE':'_SRE.C','POSIXMODUL':'POSIXMODULE.C',
             'BINASCII':'BINASCII.C','CSTRINGIO':'CSTRINGIO.C','CPICKLE':'CPICKLE.C',
             'ERRNOMODUL':'ERRNOMODULE.C','FCNTLMODUL':'FCNTLMODULE.C','MD5C':'MD5C.C',
             'MD5MODULE':'MD5MODULE.C','IMAGEOP':'IMAGEOP.C','PARSERMODU':'PARSERMODULE.C',
             'PUREMODULE':'PUREMODULE.C','SELECTMODU':'SELECTMODULE.C','SHAMODULE':'SHAMODULE.C',
             'SIGNALMODU':'SIGNALMODULE.C','SOCKETMODU':'SOCKETMODULE.C','THREADMODU':'THREADMODULE.C',
             'TIMINGMODU':'TIMINGMODULE.C',
           # REQUIRED
             'PYTHON':'PYTHON.C','CONFIG':'CONFIG.C','MAIN':'MAIN.C','ARRAYMODUL':'ARRAYMODULE.C',
             'CMATHMODUL':'CMATHMODULE.C','GETPATH':'GETPATH.C','MATHMODULE':'MATHMODULE.C',
             'STROPMODUL':'STROPMODULE.C','STRUCTMODU':'STRUCTMODULE.C',
           # IKKE BRUK *IFSIO FOR TIMEMODULE
             'TIMEMODULE':'TIMEMODULE.C','OPERATOR':'OPERATOR.C','CODECSMODU':'_CODECSMODULE.C',
             'LOCALEMODU':'_LOCALEMODULE.C','UNICODEDAT':'UNICODEDATA.C','GETBUILDIN':'GETBUILDINFO.C',

             'WEAKREF':'_WEAKREF.C','XREADLINES':'XREADLINESMODULE.C','SYMTABLEMO':'SYMTABLEMODULE.C',
             'TESTCAPIMO':'_TESTCAPIMODULE.C','GCMODULE':'GCMODULE.C','HOTSHOT':'_HOTSHOT.C',
             'DATETIMEMO':'DATETIMEMODULE.C'}
objectMods = {'BOOLOBJECT':'BOOLOBJECT.C','BUFFEROBJE':'BUFFEROBJECT.C','CLASSOBJEC':'CLASSOBJECT.C',
              'COBJECT':'COBJECT.C','COMPLEXOBJ':'COMPLEXOBJECT.C','DICTOBJECT':'DICTOBJECT.C',
              'ENUMOBJECT':'ENUMOBJECT.C','FILEOBJECT':'FILEOBJECT.C','FLOATOBJEC':'FLOATOBJECT.C',
              'FRAMEOBJEC':'FRAMEOBJECT.C','FUNCOBJECT':'FUNCOBJECT.C','INTOBJECT':'INTOBJECT.C',
              'LISTOBJECT':'LISTOBJECT.C','LONGOBJECT':'LONGOBJECT.C','METHODOBJE':'METHODOBJECT.C',
              'MODULEOBJE':'MODULEOBJECT.C','OBJECT':'OBJECT.C','RANGEOBJE':'RANGEOBJECT.C',
              'SLICEOBJE':'SLICEOBJECT.C','STRINGOBJE':'STRINGOBJECT.C','TUPLEOBJE':'TUPLEOBJECT.C',
              'TYPEOBJE':'TYPEOBJECT.C','UNICODECT':'UNICODECTYPE.C','UNICODEOBJ':'UNICODEOBJECT.C',
              'CELLOBJECT':'CELLOBJECT.C','WEAKREFOBJ':'WEAKREFOBJECT.C','STRUCTSEQ':'STRUCTSEQ.C',
              'DESCROBJEC':'DESCROBJECT.C','ITEROBJECT':'ITEROBJECT.C','OBMALLOC':'OBMALLOC.C'}
parserMods = {'BITSET':'BITSET.C','FIRSTSETS':'FIRSTSETS.C','GRAMMAR':'GRAMMAR.C','GRAMMAR1':'GRAMMAR1.C',
              'LISTNODE':'LISTNODE.C','METAGRAMMA':'METAGRAMMAR.C','MYREADLINE':'MYREADLINE.C','NODE':'NODE.C',
              'PARSER':'PARSER.C','PARSETOK':'PARSETOK.C','PGEN':'PGEN.C','PRINTGRAMM':'PRINTGRAMMAR.C',
              'TOKENIZER':'TOKENIZER.C'}
pythonMods = {'BLTINMODUL':'BLTINMODULE.C','CEVAL':'CEVAL.C','CODECS':'CODECS.C',
             'COMPILE':'COMPILE.C','ERRORS':'ERRORS.C','EXCEPTIONS':'EXCEPTIONS.C','FROZEN':'FROZEN.C',
             'FROZENMAIN':'FROZENMAIN.C','GETARGS':'GETARGS.C','GETCOMPILE':'GETCOMPILER.C',
             'GETCOPYRIG':'GETCOPYRIGHT.C','GETMTIME':'GETMTIME.C','GETPLATFOR':'GETPLATFORM.C',
             'GETVERSION':'GETVERSION.C','GRAMINIT':'GRAMINIT.C','IMPORT':'IMPORT.C','IMPORTDL':'IMPORTDL.C',
             'MARSHAL':'MARSHAL.C','MODSUPPORT':'MODSUPPORT.C','MYSNPRINTF':'MYSNPRINTF.C','MYSTRTOUL':'MYSTRTOUL.C',
             'PYFPE':'PYFPE.C','PYSTATE':'PYSTATE.C','PYTHONRUN':'PYTHONRUN.C','STRDUP':'STRDUP.C',
             'STRUCTMEMB':'STRUCTMEMBER.C','SYSMODULE':'SYSMODULE.C','THREAD':'THREAD.C','TRACEBACK':'TRACEBACK.C',
             'FUTURE':'FUTURE.C','SYMTABLE':'SYMTABLE.C','GETOPT':'GETOPT.C'}
#TODO: First item had 'modules' instead of 'zlib' for path. odd!
zlibMods = {'ZLIBMODULE':'ZLIBMODULE.C','Z_ADLER32':'ADLER32.C',
            #TODO: Compile this next one with TERASPACE(*YES *TSIFC)
            'Z_COMPRESS':'COMPRESS.C','Z_CRC32':'CRC32.C','Z_DEFLATE':'DEFLATE.C',
            'Z_GZIO':'GZIO.C','Z_INFBLOCK':'INFBLOCK.C','Z_INFCODES':'INFCODES.C',
            'Z_INFFAST':'INFFAST.C','Z_INFLATE':'INFLATE.C','Z_INFTREES':'INFTREES.C',
            'Z_INFUTIL':'INFUTIL.C','Z_TREES':'TREES.C','Z_UNCOMPR':'UNCOMPR.C','Z_ZUTIL':'ZUTIL.C'}
as400Mods = {'FILE400MOD':'FILE400MODULE.C',
             #TODO':  ALWAYS COMPILE THESE NEXT 2 WITH NO OPTIMIZE */
             'ZOS400':'_OS400.C','AS400MISC':'AS400MISC.C',
             'DYNLOAD_AS':'DYNLOAD_AS400.C','PYTHONCMD':'PYTHONCMD.C','ZDB':'_DB.C',
             'SGMLMODULE':'SGMLMODULE.C','ZDATE':'_DATE.C'}

# These are all the modules that are required to build the "python" service program
pythonSrvPgmMods = ["ABSTRACT", "ACCELER", "AS400MISC", "BITSET", "BLTINMODUL",
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

# The service programs to create, and the modules they depend on.
#TODO: It would be clearer if this was done before creating the Python srvpgm above
srvPgms = {'BINASCII':"BINASCII",'CMATH':"CMATHMODUL",'CPICKLE':"CPICKLE",
           'CSTRINGIO':"CSTRINGIO",'ZDATE':"ZDATE",'DATETIME':"DATETIMEMO",
           'ERRNO':"ERRNOMODUL",'FCNTL':"FCNTLMODUL",'IMAGEOP':"IMAGEOP",
           'MATH':"MATHMODULE",'MD5':"MD5*",'FILE400':"FILE400MOD",'ZOS400':"ZOS400",
           'PARSER':"PARSERMODU",'PURE':"PUREMODULE",'SELECT':"SELECTMODU",
           'SGML':"SGMLMODULE",'ZSOCKET':"SOCKETMODU",'ZSRE':"SRE",'STROP':"STROPMODUL",
           'STRUCT':"STRUCTMODU",'THREAD':"THREADMODU",'TIME':"TIMEMODULE",
           'TIMING':"TIMINGMODU",'UNICODEDAT':"UNICODEDAT",'ZLIB':"ZLIBMODULE &LIB/Z_*",
           'XREADLINES':"XREADLINES",'SHA':"SHAMODULE",'ZSYMTABLE':"SYMTABLEMO",
           'TESTCAPIMO':"TESTCAPIMO",'ZWEAKREF':"WEAKREF",'ZHOTSHOT':"HOTSHOT",
           'ITERTOOLS':"ITERTOOLSM",'ZCSV':"CSV",'ZRANDOM':"RANDOM",'ZDB':"ZDB"}

#def crtCmd(lib, cmd, pgm, text=None, thdSafe="*NO"):
srvPgmMods2 = {'array':"arrayModul",'binascii':"binascii",'cmath':"cmathmodul",'cpickle':"cpickle",
               'cstringio':"cstringio",'zdate':"zdate",'datetime':"datetimemo",'errno':"errnomodul",
               'fcntl':"fcntlmodul",'imageop':"imageop",'math':"mathmodule",'md5':"md5",
               'file400':"file400mod",'zos400':"zos400",'parser':"parsermodu",
               'pure':"puremodule",'select':"selectmodu",'sgml':"sgmlmodule",
               'zsocket':"socketmodu",'zsre':"sre",'strop':"stropmodul",
               'struct':"structmod",'thread':"threadmodu",'time':"timemodule",
               'TIMING':"TIMINGMODU",'UNICODEDAT':"UNICODEDAT",'ZLIB':"ZLIBMODULE",
               'XREADLINES':"XREADLINES",'SHA':"SHAMODULE",'ZSYMTABLE':"SYMTABLEMO",
               'TESTCAPIMO':"TESTCAPIMO",'ZWEAKREF':"WEAKREF",'ZHOTSHOT':"HOTSHOT",
               'ITERTOOLS':"ITERTOOLSM",'ZCSV':"CSV",'ZRANDOM':"RANDOM",'ZDB':"ZDB"}
programs = {'PYTHON':"PYTHON",'PYTHONCMD':"PYTHON"}


# Set the target library & release
if len(argv) == 3:
             targetLibrary, tgtRls = argv[1], argv[2]
else:
             targetLibrary, tgtRls = 'Python241', '*CURRENT'

#def setEnvVar(var=None, value=None):
#             os.environ["ORACLE_HOME"]="/opt/app/ora/"

#TODO: Move the source to a reasonable path!
#sourcePath = '/source/python2.4.1'
sourcePath = '/home/E11571/python-2.4.1'

os.environ["INCLUDE"]=("""%s:%s/as400:%s/include:%s/modules:%s/objects:\
          %s/parser:%s/python:%s/zlib1.1.4:\
          /qibm/proddata/ilec:/qibm/proddata/ilec/include:/qibm/include:/qibm/include/sys"""
          % (sourcePath, sourcePath, sourcePath, sourcePath,
             sourcePath, sourcePath, sourcePath, sourcePath))
def buildDirectoryModules(modules, path, skip=None):
             for module, source in modules.items():
                          if skip and module in skip: continue
                          crtCModule(module, path+source)

# Create as/400 modules for each c program in Python distribution directories:
#TODO: I can't find the source for XREADLINES.  Is it a ghost?

buildDirectoryModules(modules=objectMods, path=sourcePath+'/objects/')#,skip=['UNICODEOBJ','STRINGOBJE'])
buildDirectoryModules(modules=pythonMods, path=sourcePath+'/python/')#,skip=['THREAD', 'FROZENMAIN']
buildDirectoryModules(modules=parserMods, path=sourcePath+'/parser/')
buildDirectoryModules(modules=moduleMods, path=sourcePath+'/modules/',skip=['XREADLINES'])
buildDirectoryModules(modules=as400Mods, path=sourcePath+'/as400/')
##TODO: Track down source for zlib! No source means this will fail! :-)
buildDirectoryModules(modules=zlibMods, path=sourcePath+'/zlib1.1.4/')

# Create the "Python" service program
#TODO: Remove this hack when dependent modules compile ok
##goodmods = []
##for pymod in pythonSrvPgmMods:
##             if pymod not in ('ABSTRACT', 'ACCELER', 'STRINGOBJE', 'THREAD',
##                              'UNICODEOBJ', 'UNICODECTY'):
##                          goodmods.append(pymod)
##pythonSrvPgmMods = goodmods
#end hack
crtSrvPgm(srvPgm="Python", modules=pythonSrvPgmMods)

# Create remaining service programs. These are simpler: usually 1 mod per svcpgm
for srvPgm, modules in srvPgms.items():
             crtSrvPgm(srvPgm, modules, bndSrvPgm='Python')

for srvPgm, modules in srvPgmMods2.items():
             crtSrvPgm(srvPgm=srvPgm, modules=modules, bndSrvPgm='Python')
for pgm, bndSrvPgm in programs.items():
             crtPgm(pgm=pgm, srvPgm=bndSrvPgm)

crtPgm (targetLibrary, "python", "python")
crtPgm (targetLibrary, "pythoncmd", "python")

crtCmd ("Python", pgm="PythonCmd", text="Python", thdSafe="*YES")
print "Build completed sucessfully"
