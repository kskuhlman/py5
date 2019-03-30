#TODO: Build data area 'PYTHONHOME.' Must not have trailing slash
#TODO: Note, lib dir currently used is ~/py241/lib. (cp issues)
from sys import stdin, stdout, argv, exit
import os
from os import stat
from stat import *
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

def crtCModule(module, path, lib=None, incDir='', opt=30, inline="*ON *AUTO", dbg="*NONE", tgtRls="*CURRENT"):
             if not lib: lib=targetLibrary
             if opt < 30:
                 dbg = "*SOURCE"
                 inline = "*OFF"
             dataModel = '*LLP64'
             if module in ['ERRORS', 'MATHMODULE', 'FILEOBJECT', 'DATETIMEMO', 'IMPORT',
                           'TOKENIZER', 'DATETIMEMO', 'SOCKETMODU', 'POSIXMODUL', 'TIMEMODULE',
                           'AS400MISC', 'ZOS400']:
                dataModel = '*P128'
             if module not in ['GRAMMAR']:
                 execCmd("""CRTCMOD MODULE(%s/%s) SRCSTMF('%s') OPTIMIZE(%s) INLINE(%s)  \
                    DBGVIEW(%s) TGTRLS(%s) SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) INCDIR('%s')\
                    """ % (lib, module, path, opt, inline, dbg, tgtRls, incDir))
             else:
                 execCmd("""CRTCPPMOD MODULE(%s/%s) SRCSTMF('%s') OPTIMIZE(%s) INLINE(%s)\
                    DBGVIEW(%s) TGTRLS(%s) SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) INCDIR('%s')\
                    """ % (lib, module, path, opt, inline, dbg, tgtRls, incDir))

def crtSrvPgm(srvPgm, modules=None, bndSrvPgm=None, lib=None, tgtRls="*CURRENT"):
             if not lib: lib=targetLibrary
             if not modules: modules = srvPgm
             bndSrvPgmTxt = ' '
             if bndSrvPgm:
                          bndSrvPgmTxt = 'BNDSRVPGM(%s/%s)' % (lib, bndSrvPgm)
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
def crtPgm(pgm, srvPgm, lib=None, tgtRls="*CURRENT", actGrp="*CALLER"):
             if not lib: lib=targetLibrary
             execCmd("""CRTPGM PGM(%s/%s) BNDSRVPGM(%s/%s) ALWLIBUPD(*YES) TGTRLS(%s) ACTGRP(%s)"""
                      % (lib, pgm, lib, srvPgm, tgtRls, actGrp))

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
objectMods = {'ABSTRACT':'ABSTRACT.C',
              'BOOLOBJECT':'BOOLOBJECT.C','BUFFEROBJE':'BUFFEROBJECT.C','CLASSOBJEC':'CLASSOBJECT.C',
              'COBJECT':'COBJECT.C','COMPLEXOBJ':'COMPLEXOBJECT.C','DICTOBJECT':'DICTOBJECT.C',
              'ENUMOBJECT':'ENUMOBJECT.C','FILEOBJECT':'FILEOBJECT.C','FLOATOBJEC':'FLOATOBJECT.C',
              'FRAMEOBJEC':'FRAMEOBJECT.C','FUNCOBJECT':'FUNCOBJECT.C','INTOBJECT':'INTOBJECT.C',
              'LISTOBJECT':'LISTOBJECT.C','LONGOBJECT':'LONGOBJECT.C','METHODOBJE':'METHODOBJECT.C',
              'MODULEOBJE':'MODULEOBJECT.C','OBJECT':'OBJECT.C','RANGEOBJE':'RANGEOBJECT.C',
              'SLICEOBJE':'SLICEOBJECT.C','STRINGOBJE':'STRINGOBJECT.C','TUPLEOBJE':'TUPLEOBJECT.C',
              'TYPEOBJE':'TYPEOBJECT.C','UNICODECTY':'UNICODECTYPE.C','UNICODEOBJ':'UNICODEOBJECT.C',
              'CELLOBJECT':'CELLOBJECT.C','WEAKREFOBJ':'WEAKREFOBJECT.C','STRUCTSEQ':'STRUCTSEQ.C',
              'DESCROBJEC':'DESCROBJECT.C','ITEROBJECT':'ITEROBJECT.C','OBMALLOC':'OBMALLOC.C',
              #TODO: This is new with v2.4. Document & add to clps
              'SETOBJECT':'SETOBJECT.C','GENOBJECT':'GENOBJECT.C'}
parserMods = {                        'ACCELER':'ACCELER.C',
              'BITSET':'BITSET.C','FIRSTSETS':'FIRSTSETS.C','GRAMMAR':'GRAMMAR.C','GRAMMAR1':'GRAMMAR1.C',
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
             'FUTURE':'FUTURE.C','SYMTABLE':'SYMTABLE.C','GETOPT':'GETOPT.C',
             #TODO: Looks like this was added with 2.4. Put in notes & clp builder
             'PYSTRTOD':'PYSTRTOD.C'}
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
             'SGMLMODULE':'SGMLMODULE.C','ZDATE':'_DATE.C',
             #TODO: I added this.  Document & add to CLPs
             'ISATTY':'isatty.c'}

# These are all the modules that are required to build the "python" service program
pythonSrvPgmMods = ["ABSTRACT", "ACCELER", "AS400MISC", "BITSET", "BLTINMODUL",
             "BUFFEROBJE", "BOOLOBJECT", "CEVAL", "CLASSOBJEC", "COBJECT", "CODECS",
             "COMPILE", "COMPLEXOBJ", "CODECSMODU", "CONFIG", "CELLOBJECT", "DESCROBJEC",
             "DICTOBJECT", "DYNLOAD_AS", "ENUMOBJECT", "ERRORS", "EXCEPTIONS",
             "FILEOBJECT", "FIRSTSETS", "FLOATOBJEC", "FRAMEOBJEC", "FROZEN",
             "FROZENMAIN",
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
             "SYMTABLE", "WEAKREFOBJ", "UNICODEOBJ", "UNICODECTY", "ZIPIMPORT",
             #TODO: These are new with 2.4. Document & add to clps
             "PYSTRTOD", "SETOBJECT", "GENOBJECT", "ISATTY"]


# The service programs to create, and the modules they depend on.
#TODO: It would be clearer if this was done before creating the Python srvpgm above
srvPgms = {'BINASCII':"BINASCII",'CMATH':"CMATHMODUL",'CPICKLE':"CPICKLE",
           'CSTRINGIO':"CSTRINGIO",'ZDATE':"ZDATE",'DATETIME':"DATETIMEMO",
           'ERRNO':"ERRNOMODUL",'FCNTL':"FCNTLMODUL",'IMAGEOP':"IMAGEOP",
           'MATH':"MATHMODULE",'MD5':["MD5C","MD5MODULE"],'FILE400':"FILE400MOD",'ZOS400':"ZOS400",
           'PARSER':"PARSERMODU",'PURE':"PUREMODULE",'SELECT':"SELECTMODU",
           'SGML':"SGMLMODULE",'ZSOCKET':"SOCKETMODU",'ZSRE':"SRE",'STROP':"STROPMODUL",
           'STRUCT':"STRUCTMODU",'TIME':"TIMEMODULE",'thread':'threadmodu',
           'TIMING':"TIMINGMODU",'UNICODEDAT':"UNICODEDAT",'ZLIB':"ZLIBMODULE Z_*",
           'XREADLINES':"XREADLINES",'SHA':"SHAMODULE",'ZSYMTABLE':"SYMTABLEMO",
           'TESTCAPIMO':"TESTCAPIMO",'ZWEAKREF':"WEAKREF",'ZHOTSHOT':"HOTSHOT",
           'ITERTOOLS':"ITERTOOLSM",'ZCSV':"CSV",'ZRANDOM':"RANDOM",'ZDB':"ZDB"}

#def crtCmd(lib, cmd, pgm, text=None, thdSafe="*NO"):
srvPgmMods2 = {'array':"arrayModul",'binascii':"binascii",'cmath':"cmathmodul",'cpickle':"cpickle",
               'cstringio':"cstringio",'zdate':"zdate",'DATETIME':"datetimemo",'errno':"errnomodul",
               'fcntl':"fcntlmodul",'imageop':"imageop",'math':"mathmodule",'MD5':["MD5C","MD5MODULE"],
               'file400':"file400mod",'zos400':"zos400",'parser':"parsermodu",
               'pure':"puremodule",'select':"selectmodu",'sgml':"sgmlmodule",
               'zsocket':"socketmodu",'zsre':"sre",'strop':"stropmodul",
               'struct':"structmodu",'time':"timemodule",'thread':"threadmodu",
               'TIMING':"TIMINGMODU",'UNICODEDAT':"UNICODEDAT",'ZLIB':"ZLIBMODULE",
               'XREADLINES':"XREADLINES",'SHA':"SHAMODULE",'ZSYMTABLE':"SYMTABLEMO",
               'TESTCAPIMO':"TESTCAPIMO",'ZWEAKREF':"WEAKREF",'ZHOTSHOT':"HOTSHOT",
               'ITERTOOLS':"ITERTOOLSM",'ZCSV':"CSV",'ZRANDOM':"RANDOM",'ZDB':"ZDB"}
              #TODO: re-add:                       to dict above
programs = {'PYTHON':"PYTHON",'PYTHONCMD':"PYTHON"}

def buildDirectoryModules(modules, path, incDir, skip=None):
             for module, source in modules.items():
                if skip and module in skip:
                    print "Skipping %s cause I was told to" % module
                    continue
                fullSrcPath = path +source
                try: modTime = os.stat('/qsys.lib/%s.lib/%s.module' % (targetLibrary, module))[ST_MTIME]
                except:  modTime = 0
                srcTime = os.stat(fullSrcPath)[ST_MTIME]
                if srcTime < modTime:
                    print "Source hasn't changed. Skipping " +fullSrcPath
                    continue
                try:
                   crtCModule(module, path+source, opt=optimize, incDir=incDir)
                except:
                   print "Error creating module %s " % (module)

# Set the target library & release
if len(argv) == 3:
             targetLibrary, tgtRls = argv[1], argv[2]
else:
             targetLibrary, tgtRls = 'Python241', '*CURRENT'
optimize = 10

#TODO: Move the source to a reasonable path!
#sourcePath = '/source/python2.4.1'
sourcePath = '/home/E11571/python2.4.1'


include =("""%s/..:%s:%s/lib/plat-os400:%s/include:%s/modules:%s/objects:\
          %s/parser:%s/python:%s/zlib1.1.4:\
          """
          % (sourcePath, sourcePath, sourcePath, sourcePath, sourcePath,
             sourcePath, sourcePath, sourcePath, sourcePath))
os.environ["INCLUDE"]=include

# Create as/400 modules for each c program in Python distribution directories:
# First, create an alias for this long function name!
buildDM = buildDirectoryModules
buildDM(modules=objectMods, path=sourcePath+'/objects/',incDir=include)#,skip=['FILEOBJECT'])
buildDM(modules=pythonMods, path=sourcePath+'/python/',incDir=include)#,skip=['THREAD']) #, 'FROZENMAIN']
buildDM(modules=parserMods, path=sourcePath+'/parser/',incDir=include)
#TODO: I can't find the source for XREADLINES.  Is it a ghost?
buildDM(modules=moduleMods, path=sourcePath+'/modules/',incDir=include,skip=['XREADLINES'])
buildDM(modules=as400Mods, path=sourcePath+'/lib/plat-os400/',incDir=include)
##TODO: Track down source for zlib! No source means this will fail! :-)
###buildDirectoryModules(modules=zlibMods, path=sourcePath+'/zlib1.1.4/',incDir=include)

# Create the "Python" service program
crtSrvPgm(srvPgm="Python", modules=pythonSrvPgmMods)

# Create remaining service programs. These are simpler: usually 1 mod per svcpgm
for srvPgm, modules in srvPgms.items():
  # Can't find source for XREADLINES & need to compile zlib in from fresh source
             if srvPgm in ['XREADLINES','ZLIB', 'DATETIME']:
                    print 'Skipping creation of service program %s' % srvPgm
                    continue
             crtSrvPgm(srvPgm, modules, bndSrvPgm='Python')

print 'Starting second round of service program creates'
print 'Not sure why.. need to look into this! :-)'
for srvPgm, modules in srvPgmMods2.items():
             if srvPgm in ['XREADLINES','ZLIB','DATETIME']:
                    print '2 Skipping creation of service program %s' % srvPgm
                    continue
             crtSrvPgm(srvPgm=srvPgm, modules=modules, bndSrvPgm='Python')

for pgm, bndSrvPgm in programs.items():
             crtPgm(pgm=pgm, srvPgm=bndSrvPgm, actGrp="*NEW")

crtCmd ("Python", pgm="PythonCmd", text="Python", thdSafe="*YES")
print "Build completed sucessfully"
