cmake_minimum_required(VERSION 3.21)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "$<$<NOT:$<CONFIG:Debug>>:MultiThreaded>")

project(neon LANGUAGES CXX)

# Dependencies Begin
find_package(OpenGL REQUIRED)
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(glfw          URL "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz")
FetchContent_Declare(imgui         URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.90.6.tar.gz")
FetchContent_Declare(stb           URL "https://github.com/nothings/stb/archive/refs/heads/master.tar.gz")
FetchContent_Declare(glm           URL "https://github.com/g-truc/glm/archive/refs/tags/1.0.1.tar.gz")
FetchContent_Declare(volk          URL "https://github.com/zeux/volk/archive/refs/tags/1.3.270.tar.gz")
FetchContent_Declare(vulkanh       URL "https://github.com/KhronosGroup/Vulkan-Headers/archive/refs/tags/v1.3.285.tar.gz")
FetchContent_Declare(vulkanhpp     URL "https://github.com/KhronosGroup/Vulkan-Hpp/archive/refs/tags/v1.3.285.tar.gz")
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(volk)
FetchContent_MakeAvailable(vulkanh)
FetchContent_GetProperties(vulkanhpp)
FetchContent_Populate(vulkanhpp)
# Dependencies End

add_executable(neon)

set_property(TARGET neon PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE neon_source "source/*.cpp")
file(GLOB imgui_source "${imgui_SOURCE_DIR}/*.cpp")
target_sources(neon PRIVATE
  "${neon_source}"
  "${imgui_source}"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp" "source/renderers/renderer.cpp")

target_include_directories(neon PRIVATE
  "include"
  "${glfw_SOURCE_DIR}"
  "${imgui_SOURCE_DIR}"
  "${imgui_SOURCE_DIR}/backends"
  "${stb_SOURCE_DIR}"
  "${vulkanhpp_SOURCE_DIR}/include")

target_link_libraries(neon PRIVATE
  OpenGL::GL
  glfw
  glm::glm
  volk::volk
  Vulkan::Headers)
target_link_libraries(volk PRIVATE
  Vulkan::Headers)

if(MSVC)
  target_link_options(neon PRIVATE "$<$<NOT:$<CONFIG:Debug>>:LINKER:/SUBSYSTEM:WINDOWS,/ENTRY:mainCRTStartup>")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  set(VK_PLATFORM VK_USE_PLATFORM_WIN32_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(VK_PLATFORM VK_USE_PLATFORM_XLIB_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(VK_PLATFORM VK_USE_PLATFORM_MACOS_MVK)
endif()

target_compile_definitions(neon PRIVATE VK_NO_PROTOTYPES ${VK_PLATFORM})
target_compile_definitions(volk PRIVATE VK_NO_PROTOTYPES ${VK_PLATFORM})