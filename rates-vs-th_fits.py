# -*- coding: utf-8 -*-
"""
Created on Nov 2021
@author: daniel

FIT OF EVENT RATE VS. TRIGGER THRESHOLD
"""

import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as sco

def expo(x,A,l,x0):
	return A*np.exp(l*(x-x0))

def line(x,a,b):
	return a*x + b


"""
# first data with a few events (directory 'blocks_0922'):
V    = np.array([          50,          100,          200,         500,       1000])  
Nev  = np.array([        1000,         1000,          500,         200,         29])
T    = np.array([   107679372,    195518808,    166785310,   581150774,  754499850])*1e-6
R    = np.array([     9.27754,      5.10948,      2.99187,    0.342424,  0.0371107])
"""

"""
# second data with more events (directory 'blocks_0923'):
V    = np.array([         200,          500,         1000,        1500,       1600,       1700,       1900]) 
Nev  = np.array([        4434,         7241,         1192,          39,        336,         40,        396])
T    = np.array([  2619835908,  23848911677,  47249345270,  4356280118,67192111834,90380949706,141446002306])
R    = np.array([     1.69209,     0.303578,    0.0252067,  0.00872304,  0.0049857,0.000431507, 0.00279258])
"""

#"""
# third data, already in block III of the faculty (directory blocks_0928) (rate at 10000 mV of tr2(2)):
V = np.array([100,200,500,1000,1500,1600]) 
Nev= np.array([1016,10312,1366,788,219,175])
T  = np.array([237586098,6077235458,7303274908,55671388106,40782411792,51916440020])
R  = np.array([4.27214,1.69666,0.186902,0.0141365,0.00534544,0.00335154])
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
x = np.arange(0,2000)
popt, pcov = sco.curve_fit(line, V,logR, p0 = [1,-0.5], sigma=slogR)
a,b = popt
sa, sb = pcov[0][0], pcov[1][1]
print('a = %.7f  s(a) = %.7f    b = %.4f  s(b) = %.4f' %(a,sa,b,sb))
y = line(x,a,b)
plt.semilogy(x,np.exp(y),'k')
plt.xlabel('$V$ (mV)')
plt.ylabel('Rate (events/s)')
header = '$y = exp(ax+b)$'
texto1 = (r'$a=$ %.7f  $s(a)$ = %.7f' %(a,sa))
texto2 = (r'$b=$ %.4f  $s(b)$ = %.4f' %(b,sb))
props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
plt.text(0.52, 0.80, header+'\n'+texto1+'\n'+texto2, transform=ax2.transAxes, fontsize=8, verticalalignment='top', bbox=props)
plt.grid()
#plt.savefig('rates-vs-th_fits.png',dpi=300)
plt.show()
#"""
