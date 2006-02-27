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
