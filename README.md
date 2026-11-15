# rren

Software Renderer in C based on [Computer Graphics from scratch](https://www.gabrielgambetta.com/computer-graphics-from-scratch/). Requires SDL3.

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
./build/rren
```

## Development

Configure `CMake` to include debug info:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```
This also enables address sanitizer and UB sanitizer. You may need to install `libasan` and `libubsan`.

Format everything with `clang-format`:
```bash
./format.sh
```