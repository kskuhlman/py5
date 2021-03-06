             PGM        PARM(&LIB &OPT &INLINE &DBG &TGT)                       
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)                           
             DCL        VAR(&OPT) TYPE(*CHAR) LEN(2)                            
             DCL        VAR(&INLINE) TYPE(*CHAR) LEN(4)                         
             DCL        VAR(&DBG) TYPE(*CHAR) LEN(10)                           
             DCL        VAR(&TGT) TYPE(*CHAR) LEN(10)                           
             CHGENVVAR  ENVVAR(INCLUDE) +                                       
                          VALUE('/source/python2.3/as400:/source/pyth+          
                          on2.3/modules:/source/python2.3:/source/pyt+          
                          hon2.3/include:/qibm/proddata/ilec:/qibm/pr+          
                          oddata/ilec/include:/qibm/include:/qibm/inc+          
                          lude/sys')                                            
                                                                                
             CRTCMOD    MODULE(&LIB/ITERTOOLSM) +                               
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/ITERTOOL+          
                          SMODULE.C') OPTIMIZE(&OPT) INLINE(&INLINE +           
                          *AUTO) DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +              
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)                  
             CRTCMOD    MODULE(&LIB/CSV) OPTIMIZE(&OPT) +                       
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_CSV.C')             
             CRTCMOD    MODULE(&LIB/RANDOM) +                                   
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_RANDOMM+          
                          ODULE.C') OPTIMIZE(&OPT) INLINE(&INLINE +             
                          *AUTO) DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +              
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)                  
             CRTCMOD    MODULE(&LIB/ZIPIMPORT) +                                
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/ZIPIMPOR+          
                          T.C') OPTIMIZE(&OPT) INLINE(&INLINE +                 
                          *AUTO) DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +              
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)                  
             CRTCMOD    MODULE(&LIB/SRE) OPTIMIZE(&OPT) +                       
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_SRE.C')             
             CRTCMOD    MODULE(&LIB/POSIXMODUL) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/POSIXMODULE.C')      
             CRTCMOD    MODULE(&LIB/BINASCII) OPTIMIZE(&OPT) +                  
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/BINASCII.C')         
             CRTCMOD    MODULE(&LIB/CSTRINGIO) OPTIMIZE(&OPT) +                 
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/CSTRINGIO.C')        
             CRTCMOD    MODULE(&LIB/CPICKLE) OPTIMIZE(&OPT) +                   
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/CPICKLE.C')          
             CRTCMOD    MODULE(&LIB/ERRNOMODUL) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/ERRNOMODULE.C')      
             CRTCMOD    MODULE(&LIB/FCNTLMODUL) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/FCNTLMODULE.C')      
             CRTCMOD    MODULE(&LIB/MD5C) OPTIMIZE(&OPT) +                      
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/MD5C.C')             
             CRTCMOD    MODULE(&LIB/MD5MODULE) OPTIMIZE(&OPT) +                 
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/MD5MODULE.C')        
             CRTCMOD    MODULE(&LIB/IMAGEOP) OPTIMIZE(&OPT) +                   
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/IMAGEOP.C')          
             CRTCMOD    MODULE(&LIB/PARSERMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/PARSERMODULE.C')     
             CRTCMOD    MODULE(&LIB/PUREMODULE) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/PUREMODULE.C')       
             CRTCMOD    MODULE(&LIB/SELECTMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/SELECTMODULE.C')     
             CRTCMOD    MODULE(&LIB/SHAMODULE) OPTIMIZE(&OPT) +                 
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/SHAMODULE.C')        
             CRTCMOD    MODULE(&LIB/SIGNALMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/SIGNALMODULE.C')     
             CRTCMOD    MODULE(&LIB/SOCKETMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/SOCKETMODULE.C')     
             CRTCMOD    MODULE(&LIB/THREADMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/THREADMODULE.C')     
             CRTCMOD    MODULE(&LIB/TIMINGMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/TIMINGMODULE.C')     
           /* REQUIRED */                                                       
             CRTCMOD    MODULE(&LIB/PYTHON) OPTIMIZE(&OPT) +                    
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/PYTHON.C')           
             CRTCMOD    MODULE(&LIB/CONFIG) OPTIMIZE(&OPT) +                    
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/CONFIG.C')           
             CRTCMOD    MODULE(&LIB/MAIN) OPTIMIZE(&OPT) +                      
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/MAIN.C')             
             CRTCMOD    MODULE(&LIB/ARRAYMODUL) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/ARRAYMODULE.C')      
             CRTCMOD    MODULE(&LIB/CMATHMODUL) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/CMATHMODULE.C')      
             CRTCMOD    MODULE(&LIB/GETPATH) OPTIMIZE(&OPT) +                   
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/GETPATH.C')          
             CRTCMOD    MODULE(&LIB/MATHMODULE) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/MATHMODULE.C')       
             CRTCMOD    MODULE(&LIB/STROPMODUL) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/STROPMODULE.C')      
             CRTCMOD    MODULE(&LIB/STRUCTMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/STRUCTMODULE.C')     
                        /* IKKE BRUK *IFSIO FOR TIMEMODULE */                   
             CRTCMOD    MODULE(&LIB/TIMEMODULE) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          LOCALETYPE(*LOCALEUCS2) +                             
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/TIMEMODULE.C')       
             CRTCMOD    MODULE(&LIB/OPERATOR) OPTIMIZE(&OPT) +                  
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/OPERATOR.C')         
             CRTCMOD    MODULE(&LIB/CODECSMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_CODECSMODULE.C')    
             CRTCMOD    MODULE(&LIB/LOCALEMODU) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_LOCALEMODULE.C')    
             CRTCMOD    MODULE(&LIB/UNICODEDAT) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/UNICODEDATA.C')      
             CRTCMOD    MODULE(&LIB/GETBUILDIN) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/GETBUILDINFO.C')     
                                                                                
             CRTCMOD    MODULE(&LIB/WEAKREF) OPTIMIZE(&OPT) +                   
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_WEAKREF.C')         
             CRTCMOD    MODULE(&LIB/XREADLINES) +                               
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/XREADLIN+          
                          ESMODULE.C') OPTIMIZE(&OPT) +                         
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) +                 
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                          TGTRLS(&TGT)                                          
             CRTCMOD    MODULE(&LIB/SYMTABLEMO) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/SYMTABLEMODULE.C')   
             CRTCMOD    MODULE(&LIB/TESTCAPIMO) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_TESTCAPIMODULE.C')  
             CRTCMOD    MODULE(&LIB/GCMODULE) OPTIMIZE(&OPT) +                  
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/GCMODULE.C')         
             CRTCMOD    MODULE(&LIB/HOTSHOT) OPTIMIZE(&OPT) +                   
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/MODULES/_HOTSHOT.C')         
             CRTCMOD    MODULE(&LIB/DATETIMEMO) +                               
                          SRCSTMF('/SOURCE/PYTHON2.3/MODULES/DATETIME+          
                          MODULE.C') OPTIMIZE(&OPT) INLINE(&INLINE +            
                          *AUTO) DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +              
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)                  
             ENDPGM                                                             
