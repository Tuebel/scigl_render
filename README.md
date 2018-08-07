# scigl_render
Library to simplify rendering objects via OpenGL. The intendet use case is 
scientific (e.g. probabalistic filtering).

# dependencies
This package is built with cmake but comes with a package.xml for 
[colcon](https://colcon.readthedocs.io/en/latest/index.html) as it will be used
in ROS2.
It requires the following packages:

- gl3w (https://github.com/skaslev/gl3w)
- glfw3 (http://www.glfw.org/)
- assimp (http://www.assimp.org/)
- glm (https://glm.g-truc.net/)

All the packages require CMake and can be installed via:
```
cmake .
make
(sudo) make install
``` 

Of course a OpenGL driver is required (at least 4.2 since I am using 
glTexStorage2D but it could be replaced with the older glTexImage2D), see
https://www.khronos.org/opengl/wiki/Texture_Storage for more information).

I also built it with CMake. Since the package searches for the ament_cmake
package I suppose you need to have at least ament_cmake installed.
