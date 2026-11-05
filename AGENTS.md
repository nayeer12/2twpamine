# ytclient — Minecraft 1.21.11 Utility Mod (C++)

## Build

Requires: CMake 3.20+, MSVC (Visual Studio 2022), JDK 21 (JAVA_HOME set).

```
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Output: `build/Release/ytclient.dll`

## Keybind

- **Right Shift**: Toggle menu

## Architecture

- **Injection**: DLL injected into Minecraft Java process; MinHook hooks `wglSwapBuffers`
- **JVM access**: `JNI_GetCreatedJavaVMs` from `jvm.dll`; JVMTI via `GetEnv`
- **Tick sync**: JVMTI `SetBreakpoint` on `MinecraftClient.tick()` at location 0; all module logic + packet flush runs at tick start
- **Packets**: Only existing game packets via `ClientPlayNetworkHandler.sendPacket()`; queued in `PacketManager`, flushed at tick start
- **Menu**: ImGui (OpenGL3 + Win32 backend), amber-accented dark theme

## Mappings

All Minecraft class/method/field names are in `src/core/Mappings.h` using Fabric intermediary names for 1.21.11. If a mapping is wrong, update the constant in that file. No runtime remapping.

## Adding a module

1. Create `src/modules/MyModule.h` and `MyModule.cpp`
2. Inherit from `Module`, call base ctor with name/desc/category
3. Add settings via `addFloat/addBool/addInt` in constructor
4. Override `onEnable/onDisable/onTick/onRender` as needed
5. Register in `src/core/Client.cpp` → `initModules()`: `mgr.add<MyModule>()`
6. Add `.cpp` to `CMakeLists.txt` → `YTCLIENT_SOURCES`
