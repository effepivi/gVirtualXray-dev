Download the last version

from SVN:
```bash
$ svn checkout https://svn.code.sf.net/p/gvirtualxray/code/trunk gvirtualxray-code
```

from the web:
```bash
$ wget ...
```

CMake 3.12 required

```bash
$ mkdir bin
$ cd bin
```

```bash
$ cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_INSTALL_PREFIX:STRING=$PWD/install \
    -DBUILD_WRAPPER_CSHARP:BOOL=ON \
    -DBUILD_WRAPPER_JAVA:BOOL=ON \
    -DBUILD_WRAPPER_OCTAVE:BOOL=ON \
    -DBUILD_WRAPPER_PERL:BOOL=ON \
    -DBUILD_WRAPPER_PYTHON3:BOOL=ON \
    -DBUILD_WRAPPER_R:BOOL=ON \
    -DBUILD_WRAPPER_RUBY:BOOL=ON \
    -DBUILD_WRAPPER_TCL:BOOL=ON \
    -S ../gvirtualxray-code \
    -B $PWD
```

```bash
$ cmake-gui -S ../gvirtualxray-code -B $PWD
```

```bash
$ ccmake -S ../gvirtualxray-code -B $PWD
```

```bash
$ make -j8
```