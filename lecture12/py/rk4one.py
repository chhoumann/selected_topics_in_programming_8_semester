#/usr/bin/env python3

import ctypes
from numpy.ctypeslib import ndpointer
import numpy as np

import matplotlib.pyplot as plt
import os.path
import sys

# plt.rcParams['figure.dpi'] = 250 # For HiDPI screens

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
lib = ctypes.CDLL(libpath)
fn = "solve1"
solve = lib[fn]
solve.restype = None
solve.argtypes = [
    ctypes.c_size_t, # steps
    ctypes.c_double, # step size
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"), # t
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"), # x
]

steps = 5000
dt = 0.001
t0 = 0.
x0 = 0.
t = np.zeros(steps)
x = np.zeros(steps)
solve(steps,dt,t,x)

fig, ax = plt.subplots()
ax.plot(t, x, label='x')
ax.grid(True, linestyle='dotted')
ax.set_xlabel('time')
ax.set_ylabel('value')
ax.set_title(libname + ": " + fn);
ax.legend()
plt.show()
