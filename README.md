# [![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&pause=1000&random=false&width=600&lines=Rigid+Body+Engine)](https://git.io/typing-svg)
This repository is a simple rigid body engine written in OpenGL and configured using CMake. It supports cross-platform builds. 

# Building
To configure and build the project, do the following from the root directory of the repository:
```bash
mkdir build
cd build
cmake ..
```
This will configure the CMake and will generate platform-specific build files. To build, execute the following from within the build directory:
```bash
cd .. && cmake build
```
Or build using whatever generated build system CMake produced.

# Third Party Library Support
By default, the CMake project will automatically link with GLAD, GLFW, and OpenGL. There is optional built-in support for a few other ubiquitous libraries.

### GLAD
GLAD loader files will be generated based on the CMake cache variables set in [CMakeLists.txt](https://github.com/De-Par/3D_PHYSICS_ENGINE/blob/main/CMakeLists.txt). Setting these variables allows one to specify the parameters for the loader similar to what the corresponding [webservice](https://glad.dav1d.de/) allows you to do.

### GLFW
GLFW is linked automatically against the main project. This works without requiring any additional dependencies to be installed on Windows, but Linux may require some additional steps depending on your environment:
1. Make sure you have a working compiler (GCC / Clang) and build system, and that they're accessible to CMake. Stable versions of GCC (note: this project supports C++17 by default) and Make can be pulled down through:
   
   ```bash
    sudo apt-get install build-essential
    ```
2. X11 may need to be installed for OpenGL window creation:
   
   ```bash
    sudo apt-get install libx11-dev
    ```
3. RandR headers (dependency of X11) may need to be installed as well:
   
    ```bash
    sudo apt-get install xorg-dev libglu1-mesa-dev
    ```
# Demo

![image](https://github.com/De-Par/3D_PHYSICS_ENGINE/blob/main/images/demonstartion.png)
