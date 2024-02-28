#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb 27 12:01:53 2024

@author: eisenbrand
"""



from fractions import Fraction



def PrettyPrint(f):
    s = '' 
    for i in range(len(f)):
        frac = f[i]
        if frac.denominator == 1:
            s = s+ str(frac.numerator) + '*x^' +  str(i)
        else:
            s = s+ '(' +  str(frac.numerator) + '/' + str(frac.denominator) + ')' + '*x^' +  str(i) 
        if i < len(f) -1:
            s = s+ '+'
    return s        
        
            
        
        

def DivisionWithRemainder(f,g):
    n = len(f) - 1
    m = len(g) -1
    if n < m:
        return [], f
    else:
        a = f[n]
        b = g[m]
        c = a/b 
        
        for i in range(len(g)):
            f[n-i] = f[n-i] - c * g[m-i] 
            
        del f[-1]
        
        q_0,r = DivisionWithRemainder(f,g)
        
        q_0.append(c)
        return q_0 , r 
    

    
    
f = [Fraction(2),Fraction(0),Fraction(0),Fraction(3),Fraction(0),Fraction(4) ] 
g = [Fraction(-1), Fraction(0), Fraction(1)]
    
q,r = DivisionWithRemainder(f,g)

print(PrettyPrint(q))

print(PrettyPrint(r)) 

