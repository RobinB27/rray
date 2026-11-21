# rray

Realtime Software Raytracer in C.

Move around the scene with Arrow keys or `WASD` and look around using the mouse. `ESC` quits the renderer.

## Build from source

Install [SDL3](https://libsdl.org/). Build with `CMake`. First run:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```
to configure `CMake`. Then build with:
```bash
cmake --build build
```
If the build was successful, run the renderer with
```bash
./build/rray
```

## Development

Configure `CMake` to include debug info & sanitization:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```
You may need to install `libasan` and `libubsan`.

Format everything with `clang-format`:
```bash
./format.sh
```

## References
- Fundamentals of Computer Graphics, 5th Edition
- [Computer Graphics From Scratch](https://www.gabrielgambetta.com/computer-graphics-from-scratch/)
- [SDL3 Wiki](https://wiki.libsdl.org/SDL3/FrontPage)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)