# Python Binding Examples using CTypes

The project demonstrates how to create a dynamically linked library (DLL) out of C code and call it from Python programs.

The C code contains a sample solver for ordinary differential equations (ODE) using Runge-Kutta 4th degree integrator. 
The math details are not important here.
The solution is designed in such a way that Python script allocates the memory for variable trajectories, passes them for heavy computations in the DLL and then plots the resulting trajectories.

See [tutorial on foreign function interface with CTypes](https://docs.python.org/3/library/ctypes.html) for more.

## Dependencies

* Recommended: [Clion IDE](https://www.jetbrains.com/clion/)
* CMake build generator
* C/C++ compiler: GCC version 10 or later, or Visual Studio compiler.
* Python version 3 with `numpy` and `matplotlib` modules. 

### Linux
Debian-based:
```shell
sudo apt install cmake gcc g++ python3 python3-numpy python3-matplotlib
sudo apt install libqt5charts5-dev # for Qt5
```
Pacman-based:
```shell
pacman -S python3 python3-numpy python3-matplotlib
pacman -S libqt5charts # for Qt5
```

For others please consult with your package manager (search for python, numpy, matplotlib, qt5 charts). 
`numpy` and `matplotlib` can also be installed using python package manager `pip`.

In case of running issues, the dependent libraries can be inspected by running `ldd test_qt5`.

### Windows

It is recommended to use the toolchain and everything from MSYS2 distribution: it has the toolchain (compiler, cmake, ninja), libraries (qt5) and python.

On the MinGW64/MSYS2 command prompt install the following tools:
```shell
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-gcc # the toolchain
pacman -S mingw-w64-x86_64-python mingw-w64-x86_64-python-numpy mingw-w64-x86_64-python-matplotlib # python
pacman -S mingw-w64-x86_64-qt5-charts # Qt5 charts
```
Then make sure that Clion is configured to use MinGW toolchain and not the built-in one: in the Settings/Build/Toolchains press plus icon, choose `MinGW` and then Select the `c:\msys64\mingw64` as the toolset directory, then Clion should find the corresponding compilers.

When running from Clion command prompt, python assumes the local environment in the project directory `venv`, therefore `numpy` and `matplot` may need to be installed there:
```shell
pip install numpy matplot
```

## Running

### From Clion

* In `Settings`/`Build`/`CMake` add `Release` build.
* Regenerate `CMake` (tab at the bottom).
* Select `All CTest | Release` build.
* Press `Run` button, you should see a series of new windows popping up showing plots (close them to complete the test).

### From Command line

The following compiles the library, runs Python scripts and produces four plot windows:
```shell
cmake -B build
cmake --build build --config Release
(cd build ; ctest -C Release --output-on-failure)
```
Individual Python scripts need to be executed in the directory where the `rk4` library is built, normally it is in `build` directory, but VisualStudio may put them into `build/Release`, thus the following can be used to run:
```shell
python py/rk4one.py build/src/librk4.dll
python py/rk4two1.py build/src/librk4.dll
python py/rk4two2.py build/src/librk4.dll
python py/rk4two3.py build/src/librk4.dll
```

## Library Diagnostics

### Linux

The content of an executable object file can be examined using the following utilities:
* `file` identifies the format and the platform (both the program and the library should target the same).
* `ldd` prints the dynamic library dependencies, which need to be resolved during program startup.
* `objdump -CT` prints the dynamic symbols (functions and variables) the library exports.
* `nm -CD` prints the dynamic symbols the library exports (same as objdump)

### Windows

It is recommended to add MSYS2/MingW64 tools and libraries to the PATH environment variable:
* `c:/msys64/mingw64/bin` -- common utilities and libraries
* `c:/msys64/mingw64/lib` -- extra libraries
* Click Start, enter "environment", choose "Environment Variables" and add the above to your `Path`.

If your program exits with code -1073741511 (0xC0000139), it means that it is missing shared libraries. 

On Windows `file` and `ldd` utilities are available at MinGW64/MSYS2 prompt (also included in "Git Bash Here" prompt when right-clicked inside folder), alternatively use `dumpbin.exe` from your Visual Studio installation:
* `dumpbin /dependents` prints the dynamic library dependencies of the executable object.
* `dumpbin /exports` prints the dynamic symbols exported by the library.

## Exercise

The goal is learn how to work library APIs: expose C API, use C++ (`Qt`) API, use the C API from Python code. 

1. Convert C code in [test_rk4.c](src/test_rk4.c) into C++ code in [test_rk4.cpp](src/test_rk4.cpp).
   * Copy [test_rk4.c](src/test_rk4.c) into [test_rk4.cpp](`src/test_rk4.cpp`) file and add CMake target `test_rk4_cpp` for it.
   * Get rid of C headers from [test_rk4.cpp](`src/test_rk4.cpp`) and use C++ instead when needed.
   * Replace C arrays, `memset` and `malloc` calls with C++ code.
   * Fix printing. Use [std::setprecision](https://en.cppreference.com/w/cpp/io/manip/setprecision) where needed. 
   * Compare the outputs from `test_rk4` and `test_rk4_cpp`. 

2. Qt Graphical Library Exercise. Create a C++ class with two methods to display two kinds of plots for a given data series:
   * Scatter plot (dots at x,y coordinates), and
   * Line plot (x,y coordinates connected with lines).
   * Plan:
     * Create [test_plot.cpp](src/test_plot.cpp) "test" file (simple `main()` will do, no `doctest` this time) which has the following functionality: 
       * Creates two indexed random number series (choose your design of indices and values, pick your own function instead of random).
       * Includes [plot.hpp](include/plot.hpp). 
       * Creates an instance `plot` of a class `Plot`.
       * Calls `plot.scatter(...)` and passes the first number series.
       * Similarly, calls `plot.lines(...)` and passes the second number series.
       * Calls `plot.process(...)` to render a graphical window with plot and process its events.
     * Define a simple class `Plot` in `plot.hpp` header file with the corresponding methods `scatter` and `lines`.
     * In [plot.cpp](src/plot.cpp) implement the class `Plot` member functions.
     * See [test_q5.cpp](src/test_qt5.cpp) for example code you can borrow.
     * Encapsulate and hide the `Qt` library details in [plot.cpp](src/plot.cpp) (header ([plot.hpp](include/plot.hpp) should be as simple as possible and not contain any `Qt` details). Use Pimpl idiom.
     * Note that `Qt` library may crash if a `Qt` [window](https://doc.qt.io/qt-6/qwindow.html) or a [widget](https://doc.qt.io/qt-6/qchart.html) is created before [QApplication](https://doc.qt.io/qt-6/qapplication.html) is initialized, so make sure it is created first.
     * Avoid copy-pasting, use DRY (Don't Repeat Yourself) principle and reuse as much of your own code as possible.

3. Python Exercise. Walk through scripts in [py](py/) directory ([rk4one.py](py/rk4one.py), [rk4two1.py](py/rk4two1.py),...) and C/C++ code. Examine the following:
   * What is being executed in Python and what in C/C++ code?
   * Where does the data come from?
   * Who is responsible for the memory allocation?
