# Open-Stream 

Open source low latency stream solution. 

-----
### Project structure

- root folder: contains the GUI application. 
- `openstreamhost` folder: contains the streaming **host** application. Includes `git` submodules. 

### Building

#### Windows build requirements
- Windows 10 64 bits.
- [`Msys2`](https://www.msys2.org/) development platform. 
``` 
 mingw-w64-x86_64-openssl mingw-w64-x86_64-cmake mingw-w64-x86_64-toolchain mingw-w64-x86_64-ffmpeg mingw-w64-x86_64-boost
``` 
- `Qt SDK` for `Msys2`
  - [Qt packages](https://wiki.qt.io/MSYS2#Obtain_Pre-Built_Qt_.26_QtCreator_binary_files_and_Use_instantly_without_Building.2FCompiling)
    
#### Build the project using QtCreator: 
- Open the project using the `CMake` support and selecting the `CMakeList` file in root folder.
- Select building kit `MinGW-w64 64bit (MSYS2)`
