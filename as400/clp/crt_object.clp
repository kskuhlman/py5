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
