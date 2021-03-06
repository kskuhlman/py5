             PGM        PARM(&LIB &OPT &INLINE &DBG &TGT)                       
             DCL        VAR(&LIB) TYPE(*CHAR) LEN(10)                           
             DCL        VAR(&OPT) TYPE(*CHAR) LEN(2)                            
             DCL        VAR(&INLINE) TYPE(*CHAR) LEN(4)                         
             DCL        VAR(&DBG) TYPE(*CHAR) LEN(10)                           
             DCL        VAR(&TGT) TYPE(*CHAR) LEN(10)                           
             CHGENVVAR  ENVVAR(INCLUDE) +                                       
                          VALUE('/source/PYTHON2.3/as400:/source/pyth+          
                          on2.3:/source/PYTHON2.3/include:/source/pyt  +        
                          hon2.3/python:/qibm/proddata/ilec:/qibm/pro  +        
                          ddata/ilec/include:/qibm/include:/qibm/incl+          
                          ude/sys')                                             
                                                                                
             CRTCMOD    MODULE(&LIB/FILE400MOD) OPTIMIZE(&OPT) +                
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/FILE400MODULE.C')      
          /* ALWAYS COMPILE THIS WITH NO OPTIMIZE */                            
             CRTCMOD    MODULE(&LIB/ZOS400) OPTIMIZE(10) +                      
                          DBGVIEW(&DBG) TGTRLS(&TGT) +                          
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/_OS400.C'     )        
          /* ALWAYS COMPILE THIS WITH NO OPTIMIZE */                            
             CRTCMOD    MODULE(&LIB/AS400MISC) OPTIMIZE(10) +                   
                          DBGVIEW(&DBG) TGTRLS(&TGT) +                          
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/AS400MISC.C')          
             CRTCMOD    MODULE(&LIB/DYNLOAD_AS) +                               
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/DYNLOAD_+            
                          AS400.C') OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +      
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                          DBGVIEW(&DBG) TGTRLS(&TGT)                            
             CRTCMOD    MODULE(&LIB/PYTHONCMD) OPTIMIZE(&OPT) +                 
                          INLINE(&INLINE *AUTO) DBGVIEW(&DBG) TGTRLS(&TGT) +    
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                        SRCSTMF('/SOURCE/PYTHON2.3/AS400/PYTHONCMD.C')          
             CRTCMOD    MODULE(&LIB/ZDB) +                                      
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/_DB.C') +            
                          OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +                
                          DBGVIEW(&DBG) SYSIFCOPT(*IFSIO) +                     
                          LOCALETYPE(*LOCALEUCS2) TGTRLS(&TGT)                                                                 
             CRTCMOD    MODULE(&LIB/SGMLMODULE) +                               
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/SGMLMODULE.C') +      
                          OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +                
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                          DBGVIEW(&DBG) TGTRLS(&TGT)                            
             CRTCMOD    MODULE(&LIB/ZDATE) +                                    
                          SRCSTMF('/SOURCE/PYTHON2.3/AS400/_DATE.C') +           
                          OPTIMIZE(&OPT) INLINE(&INLINE *AUTO) +                
                          SYSIFCOPT(*IFSIO) LOCALETYPE(*LOCALEUCS2) +           
                          DBGVIEW(&DBG) TGTRLS(&TGT)                                       
             ENDPGM                                                             
