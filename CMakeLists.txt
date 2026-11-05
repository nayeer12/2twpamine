cmake_minimum_required(VERSION 3.20)
project(ytclient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options(/W3 /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
endif()

# ── JNI / JVMTI ──────────────────────────────────────────────
find_package(JNI REQUIRED)

if(NOT JNI_FOUND)
    message(FATAL_ERROR "JNI not found. Set JAVA_HOME or JDK_DIR.")
endif()

# ── FetchContent: MinHook + Dear ImGui ───────────────────────
include(FetchContent)

FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(minhook)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.5
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# ── OpenGL ───────────────────────────────────────────────────
find_package(OpenGL REQUIRED)

# ── ytclient DLL ─────────────────────────────────────────────
set(YTCLIENT_SOURCES
    src/DllMain.cpp
    src/core/Client.cpp
    src/core/JVM.cpp
    src/core/Hooks.cpp
    src/modules/ModuleManager.cpp
    src/modules/Speed.cpp
    src/modules/Fly.cpp
    src/modules/NoFall.cpp
    src/modules/Sprint.cpp
    src/modules/KillAura.cpp
    src/modules/Reach.cpp
    src/modules/FullBright.cpp
    src/packet/PacketManager.cpp
    src/packet/TickSync.cpp
    src/menu/Menu.cpp
    src/menu/Theme.cpp
)

add_library(ytclient SHARED ${YTCLIENT_SOURCES})

target_include_directories(ytclient PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${JNI_INCLUDE_DIRS}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(ytclient PRIVATE
    minhook
    imgui
    ${JNI_LIBRARIES}
    OpenGL::GL
    opengl32
    gdi32
    user32
    dwmapi
)

set_target_properties(ytclient PROPERTIES
    OUTPUT_NAME ytclient
    SUFFIX .dll
)
