# OpenDSS
Virtual music center for DOS, Windows and Linux. Inspired by Digital Sound System DOS version from Sergey Sapelin.

Uses [CroconTUI](https://github.com/CroconTUI/crocon-lib) framework and [OpenDSE](https://github.com/tinelix/OpenDSE) library for WAV support.

## Building

### Supported targets

* Microsoft Visual C++ (Windows)
    * `vc2` - Microsoft Visual C++ 2.0 - 2003
    * `vc8` - Microsoft Visual Studio 2005 - 2013
    * `vc14` - Microsoft Visual Studio 2015 - 2025
* GNU Compiler Collection (Linux)

### Windows 

To build the program in Windows, download the source code of the required libraries and run the commands below:
    
```bat
REM Set enviroment variables for CroconTUI and OpenDSE include directories...
set CROCON_ROOT=[CroconTUI root]
set OPENDSE_ROOT=[OpenDSE root]

REM Build...
cd [OpenDSS root]\proj\msvc
nmake -f [MSVC target].mak
```

### Linux

To build the program on a Linux or UNIX-like operating system, download the source code of the required libraries and run the commands below:

```sh
# Set enviroment variables for CroconTUI and OpenDSE include directories...
export CROCON_ROOT=[CroconTUI path]
export OPENDSE_ROOT=[OpenDSE path]

# Build...
cd [OpenDSS root]/proj/gcc
make
```

## Running

```sh
./opendss [WAV file real or relative path]
```
