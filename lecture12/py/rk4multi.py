#!/usr/bin/env python3

import ctypes
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
lib = ctypes.CDLL(libpath)
fn = "solve_trig" # the name of the function to lookup in the library
solve = lib[fn]
solve.restype = None
solve.argtypes = [
    ctypes.c_size_t, # steps
    ctypes.c_double, # dt
    np.ctypeslib.ndpointer(ctypes.c_double, ndim=2, flags="C_CONTIGUOUS")
]

vars = 3
steps = 5000
dt = 0.001
tr = np.empty(shape=(steps, vars), dtype=np.float64)
tr[0]=(0.,0.,1.) # initial state
solve(steps, dt, tr)

fig, ax = plt.subplots()
ax.plot(tr[:,0], tr[:,1])
ax.plot(tr[:,0], tr[:,2])
ax.grid(True, linestyle='dotted')
ax.tick_params(labelsize='medium', width=1)
ax.set_title(libname + ": " + fn);
plt.show()
