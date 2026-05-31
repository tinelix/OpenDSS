# OpenDSS
Virtual music center for DOS, Windows and Linux. Inspired by Digital Sound System DOS version from Sergey Sapelin.

Uses [CroconTUI](https://github.com/CroconTUI/crocon-lib) and [OpenDSE](https://github.com/tinelix/OpenDSE) libraries for WAV support and TUI implementation.

## Building

### Supported targets

* Microsoft Visual C++ (Windows)
    * `vc2` - Microsoft Visual C++ 2.0 - 2003
    * `vc8` - Microsoft Visual Studio 2005 - 2013
    * `vc14` - Microsoft Visual Studio 2015 - 2025
* GNU Compiler Collection (Linux)

### Windows 

To build the program in Windows, use the commands below:
    
```bat
set CROCON_ROOT=[CroconTUI root]
set OPENDSE_ROOT=[OpenDSE root]
cd [OpenDSS root]\proj\msvc
nmake -f [MSVC target].mak
```

### Linux

To build the program in Linux origin UNIX-like operating system, use the commands below:

```sh
export CROCON_ROOT=[CroconTUI path]
export OPENDSE_ROOT=[OpenDSE path]
cd [OpenDSS root]/proj/gcc
make
```

## Running

```sh
./opendss [WAV file real or relative path]
```