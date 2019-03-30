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
