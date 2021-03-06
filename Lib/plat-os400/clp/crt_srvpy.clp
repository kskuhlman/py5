             PGM        PARM(&LIB &TGTRLS)                                      
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)                           
             DCL        VAR(&TGTRLS) TYPE(*CHAR) LEN(10)                        
                                                                                
             CRTSRVPGM  SRVPGM(&LIB/PYTHON) MODULE(&LIB/ABSTRACT +              
                          &LIB/ACCELER &LIB/AS400MISC &LIB/BITSET +             
                          &LIB/BLTINMODUL &LIB/BUFFEROBJE +                     
                          &LIB/BOOLOBJECT +                                     
                          &LIB/CEVAL &LIB/CLASSOBJEC &LIB/COBJECT +             
                          &LIB/CODECS &LIB/COMPILE &LIB/COMPLEXOBJ +            
                          &LIB/CODECSMODU &LIB/CONFIG +                         
                          &LIB/CELLOBJECT &LIB/DESCROBJEC +                     
                          &LIB/DICTOBJECT &LIB/DYNLOAD_AS +                     
                          &LIB/ENUMOBJECT +                                     
                          &LIB/ERRORS &LIB/EXCEPTIONS +                         
                          &LIB/FILEOBJECT +                                     
                          &LIB/FIRSTSETS &LIB/FLOATOBJEC +                      
                          &LIB/FRAMEOBJEC &LIB/FROZEN +                         
                          &LIB/FUTURE +                                         
                          &LIB/FUNCOBJECT &LIB/GETARGS +                        
                          &LIB/GETBUILDIN &LIB/GETCOMPILE +                     
                          &LIB/GETCOPYRIG &LIB/GETMTIME +                       
                          &LIB/GETPATH &LIB/GETPLATFOR +                        
                          &LIB/GETOPT +                                         
                          &LIB/GETVERSION &LIB/GRAMINIT +                       
                          &LIB/GRAMMAR &LIB/GRAMMAR1 &LIB/IMPORT +              
                          &LIB/IMPORTDL &LIB/INTOBJECT +                        
                          &LIB/ITEROBJECT +                                     
                          &LIB/LISTNODE &LIB/LISTOBJECT +                       
                          &LIB/LOCALEMODU &LIB/LONGOBJECT &LIB/MAIN +           
                          &LIB/MARSHAL +                                        
                          &LIB/METAGRAMMA &LIB/METHODOBJE +                     
                          &LIB/MODSUPPORT &LIB/MODULEOBJE +                     
                          &LIB/MYREADLINE &LIB/MYSTRTOUL +                      
                          &LIB/MYSNPRINTF &LIB/NODE &LIB/OBMALLOC +             
                          &LIB/OBJECT &LIB/OPERATOR &LIB/PARSER +               
                          &LIB/PARSETOK &LIB/PGEN &LIB/POSIXMODUL +             
                          &LIB/PRINTGRAMM &LIB/PYFPE &LIB/PYSTATE +             
                          &LIB/PYTHONRUN &LIB/RANGEOBJE +                       
                          &LIB/SIGNALMODU &LIB/SLICEOBJE +                      
                          &LIB/STRDUP &LIB/STRINGOBJE +                         
                          &LIB/STRUCTMEMB &LIB/SYSMODULE +                      
                          &LIB/THREAD &LIB/TOKENIZER &LIB/TRACEBACK +           
                          &LIB/TUPLEOBJE &LIB/TYPEOBJE &LIB/GCMODULE +          
                          &LIB/STRUCTSEQ &LIB/SYMTABLE &LIB/WEAKREFOBJ +        
                          &LIB/UNICODEOBJ &LIB/UNICODECTY &LIB/ZIPIMPORT) +     
                          EXPORT(*ALL) +                                        
                          TGTRLS(&TGTRLS)                                       
                                                                                
             ENDPGM                                                             
