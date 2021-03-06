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
