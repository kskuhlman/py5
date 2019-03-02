             PGM                                                                
             DLTF       FILE(PYTHON23/LIB)                                      
             MONMSG     MSGID(CPF0000)                                          
             CRTSAVF    FILE(PYTHON23/LIB)                                      
             SAV        DEV('/QSYS.LIB/PYTHON23.LIB/LIB.FILE') +                
                          OBJ(('/USR/PYTHON2.3/LIB/*') +                        
                          ('/USR/PYTHON2.3/LIB/*.PYC' *OMIT) +                  
                          ('/USR/PYTHON2.3/LIB/HOTSHOT/*.PYC' +                 
                          *OMIT) +                                              
                          ('/USR/PYTHON2.3/LIB/COMPILER/*.PYC' +                
                          *OMIT) +                                              
                          ('/USR/PYTHON2.3/LIB/ENCODINGS/*.PYC' +               
                          *OMIT) +                                              
                          ('/USR/PYTHON2.3/LIB/LOGGING/*.PYC' +                 
                          *OMIT) +                                              
                          ('/USR/PYTHON2.3/LIB/DISTUTILS/*.PYC' +               
                          *OMIT) ('/USR/PYTHON2.3/LIB/XML/*.PYC' +              
                          *OMIT) +                                              
                          ('/USR/PYTHON2.3/LIB/EMAIL/*.PYC' +                   
                          *OMIT) ('/USR/PYTHON2.3/LIB/TEST/*.PYC' +             
                          *OMIT) +                                              
                          ('/USR/PYTHON2.3/LIB/TEST/OUTPUT' +                   
                          *OMIT) +                                              
                          ) USEOPTBLK(*NO) TGTRLS(V4R4M0) +                     
                          DTACPR(*YES)                                          
             ENDPGM                                                             
