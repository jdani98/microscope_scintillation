#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 25 12:52:16 2021
@author: daniel

PROGRAM TO OBTAIN THE DISTRIBUTION OF THE CHARGES AND ISOLATE THE MAXIMA
"""

import matplotlib.pyplot as plt
from picocodes_modules.picoReader import ps3000Reader
from picocodes_modules.auto_plots import histo_stats

### CONFIG
fname = 'DATA/blocks_11/block_1000_50_7_770V.txt'
channel = 'D'
max_th = 40000
min_th = 2000


dataobj = ps3000Reader(fname)

DATA = dataobj.read_txt2()

charges = []

#print(DATA)
for iev,event in DATA.items():
    #print(event)
    charge = -sum(event[channel])
    charges.append(charge)
    if charge > max_th:
        print('max',iev,charge)
    if charge < min_th:
        print('min',iev,charge)

print('Nev=',iev)

plt.close('all')
fig, ax = plt.subplots(1)
histogram = histo_stats((charges,))
tN, tbines, tpatches = histogram.plot_histo(ax,bins=(20,))