"""Append site specific modules 

****************************************************************
* This module is automatically imported during initialization via site.py. *
****************************************************************
"""
import __builtin__

#TODO: KSK: Is this really necessary? XXX Remove?
# for __ILEC400__ put File400 in builtins
try:
    import file400
    import __builtin__
    __builtin__.File400 = file400.File400
except:
    pass
