# -*- coding: utf-8 -*-
"""
Created on Nov 2021
@author: daniel

FIT OF EVENT RATE VS. SCINTILLATOR FEED VOLTAGE
"""

import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as sco

def expo(x,A,l,x0):
	return A*np.exp(l*(x-x0))

def line(x,a,b):
	return a*x + b


#"""
# Data from NOVEMBER:
V    = np.array([         770,          780,          800,         820,        900])
Nev  = np.array([        1960,         3892,         3752,        3667,       1663])
T    = np.array([252105951892, 334381781888, 169956793137, 82846479017, 1797973592])
R    = np.array([  0.00777054,    0.0116364,    0.0220703,   0.0442505,   0.924374])
#"""

sR   = np.sqrt(Nev)/(T*1e-6)
print('sR=',sR)
slogR = sR/R

"""
## PLOTS IN LINEAR SCALE
fig1, ax1 = plt.subplots()
plt.errorbar(V,R,sR,fmt='ro',ecolor='cyan',barsabove=True)
x = np.arange(700,1000)
popt, pcov = sco.curve_fit(expo, V,R, p0=(1.,-0.5,900), sigma=sR)
A,l,x0 = popt
print('A=',A,' l=',l,' x0=',x0)
y = expo(x,A,l,x0)
plt.plot(x,y,'k')
plt.xlabel('$V$ (V)')
plt.ylabel('Rate (events/s)')
texto1 = (r'$A=$ %.5f' %A)
texto2 = (r'$\lambda=$ %.5f' %l)
texto3 = (r'$x_0=$ %.5f' %x0)
props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
plt.text(0.78, 0.40, texto1+'\n'+texto2+'\n'+texto3, transform=ax1.transAxes, fontsize=8, verticalalignment='top', bbox=props)
plt.grid()
plt.show()
"""

#"""
## PLOTS IN SEMI-LOGARITMIC SCALE
plt.close('all')
fig2, ax2 = plt.subplots()
logR = np.log(R)
#plt.semilogy(V,R,marker='o',linestyle='None')
plt.errorbar(V,R,yerr=sR, marker='o',linestyle='None')
ax2.set_yscale('log')
x = np.arange(700,1000)
popt, pcov = sco.curve_fit(line, V,logR, p0 = [1,-0.5], sigma=slogR)
a,b = popt
sa, sb = pcov[0][0], pcov[1][1]
print('a = %.7f  s(a) = %.7f    b = %.4f  s(b) = %.4f' %(a,sa,b,sb))
y = line(x,a,b)
plt.semilogy(x,np.exp(y),'k')
plt.xlabel('$V$ (V)')
plt.ylabel('Rate (events/s)')
#plt.xlim(700,1000)
header = '$y = exp(ax+b)$'
texto1 = (r'$a=$ %.7f  $s(a)$ = %.7f' %(a,sa))
texto2 = (r'$b=$ %.4f  $s(b)$ = %.4f' %(b,sb))
props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
plt.text(0.52, 0.20, header+'\n'+texto1+'\n'+texto2, transform=ax2.transAxes, fontsize=8, verticalalignment='top', bbox=props)
plt.grid()
#plt.savefig('rates-vs-voltage_fits.png',dpi=300)
plt.show()
#"""
