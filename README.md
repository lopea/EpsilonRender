# Epsilon Renderer
## A tiny little vulkan renderer.
### Setup
#### Prerequisites
- Windows or Linux (Mac untested)
- Cmake (3.19 minimum)
- Vulkan 1.2 API 
- MSVC C++ 2019/GCC++/Clang 

#### Git Submodules
Before compiling the project, you must set up essential libraries through git.
To do that you must initialize the submodules inside this project. Enter the following commands through the git bash/terminal on the root project folder.
```bash
$ git submodule init
$ git submodule update --init
```
This will clone libraries necessary for this project and add them to the `lib/` folder.
Cmake will automatically compile and link the libraries to the renderer.

##### Libraries Installed:
- [GLFW](https://github.com/glfw/glfw)
- [Assimp](https://github.com/assimp/assimp)
- [Imgui](https://github.com/ocornut/imgui/tree/docking) (docking branch)

#### Compilation
In order to compile the program, you will need to use CMake commands to do so,

```bash
$ cmake --target EpsilonRenderer --build <binary directory>
```

The program executable will be located at `<binary directory>`.