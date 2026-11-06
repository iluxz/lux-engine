# lux Engine

A from-scratch C++ game engine with 2D/3D rendering, physics, audio, scripting, and director graph.

## Features (v0.2)

- Cross-platform windowing (GLFW)
- OpenGL 4.6 renderer
- Shader system
- Camera (2D orthographic + 3D perspective)
- Mesh primitives (cube, sphere, plane, quad)
- Texture loading (stb_image)
- Input system (keyboard, mouse, gamepad)
- ECS (Entity Component System)
- Application framework

## Building

### Prerequisites
- CMake 3.20+
- C++20 compiler (MSVC, GCC, or Clang)
- OpenGL 4.6 capable GPU

### Build
```bash
cmake -B build -S .
cmake --build build --config Release
./build/Release/luxengine_app
```

## Roadmap

- [x] Window & OpenGL context
- [x] Shader system
- [x] Camera (2D/3D)
- [x] Mesh primitives
- [x] Texture loading
- [x] Input system
- [x] ECS
- [ ] Physics (Box2D/Bullet)
- [ ] Audio (miniaudio)
- [ ] Scene management
- [ ] Asset pipeline
- [ ] Scripting (Lua)
- [ ] Director graph (visual scripting)
- [ ] Editor (Dear ImGui)
- [ ] Vulkan backend
- [ ] DirectX backend

## License

MIT