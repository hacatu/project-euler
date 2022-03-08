from sympy import *

cos_table = [None]*91

"""
cos(x/2) = sqrt((1+cos(x))/2)

cos(2x) = 2*cos(x)^2-1

cos(3x) = 4*cos(x)^3-3*cos(x)
"""

def double_angle(cosx):
    return 2*cosx**2 - 1

def triple_angle(cosx):
    return 4*cosx**3 - 3*cosx

def bisect_angle(cosx):
    return sqrt((1 + cosx)/2)

cos_table[0] = sqrt(1)
cos_table[15] = (sqrt(2) + sqrt(6))/4
cos_table[30] = sqrt(3)/2
cos_table[36] = (1 + sqrt(5))/4
cos_table[45] = sqrt(2)/2
cos_table[60] = sqrt(1)/2
cos_table[75] = (-sqrt(2) + sqrt(6))/4
cos_table[90] = sqrt(0)

