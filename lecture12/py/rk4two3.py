#!/usr/bin/env python3

import ctypes
from numpy.ctypeslib import ndpointer
import numpy as np
import matplotlib.pyplot as plt
import os.path
import sys

# plt.rcParams['figure.dpi'] = 250 # HiDPI

if (len(sys.argv) < 2):
    print("Expecting a path to dynamically link library as a command line argument")
    exit(1)

libpath = sys.argv[1]

if (not os.path.isfile(libpath)):
    libpath = os.path.join(os.getcwd(), libpath)
    if (not os.path.isfile(libpath)):
        print("Could not open the rk4 library: ", libpath)
        exit(1)

libname = os.path.basename(libpath)
lib = ctypes.CDLL(os.path.join(os.getcwd(), libpath))
fn = "solve2_3"
solve = lib[fn]
solve.restype = None
solve.argtypes = [
    ctypes.c_size_t, # steps
    ctypes.c_double, # step size
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"), # t timestamps
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"), # x trajectory
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"), # y trajectory
]

steps = 500000
dt = 0.00001
t = np.zeros(steps)
x = np.zeros(steps)
y = np.zeros(steps)
x[0] = 10.;
solve(steps, dt, t, x, y)

# see plot gallery: https://matplotlib.org/3.3.4/gallery/index.html
fig, ax = plt.subplots()
ax.plot(t, x, label="x")
ax.plot(t, y, label="y")
ax.grid(True, linestyle='dotted')
ax.set_xlabel('time')
ax.set_ylabel('value')
ax.set_title(libname + ': ' + fn);
ax.legend()
plt.show()
