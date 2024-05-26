#include <cstdint>
#include <memory>
#include <neon/backends/backend_config.hpp>
#include <neon/backends/i_backend.hpp>
#include <neon/backends/opengl3_backend.hpp>
#include <neon/renderers/renderer.hpp>
#include <type_traits>
#include <vulkan/vulkan_core.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>

namespace neon {
void
OpenGL3Backend::init(const BackendConfig& config)
{
  m_p_renderer = std::move(config.p_renderer);

  glfwSetErrorCallback([](int error_code, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error_code, description);
  });

  if (GLFW_FALSE == glfwInit()) {
    std::abort();
  }

  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  m_p_window = glfwCreateWindow(config.window_width,
                                config.window_height,
                                config.window_title,
                                nullptr,
                                nullptr);
  if (nullptr == m_p_window) {
    std::abort();
  }

  int monitor_x, monitor_y;
  GLFWmonitor** pp_monitors = glfwGetMonitors(&monitor_x);
  if (nullptr == pp_monitors) {
    std::abort();
  }
  const GLFWvidmode* p_video_mode = glfwGetVideoMode(pp_monitors[0]);
  if (nullptr == p_video_mode) {
    std::abort();
  }
  glfwGetMonitorPos(pp_monitors[0], &monitor_x, &monitor_y);
  glfwSetWindowPos(
    m_p_window,
    monitor_x + (p_video_mode->width - config.window_width) * 0.5f,
    monitor_y + (p_video_mode->height - config.window_height) * 0.5f);

  glfwMakeContextCurrent(m_p_window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(m_p_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  glClearColor(clear_color.x * clear_color.w,
               clear_color.y * clear_color.w,
               clear_color.z * clear_color.w,
               clear_color.w);

  if (nullptr != m_p_renderer) {
    m_p_renderer->init();
  }
}

void
OpenGL3Backend::run()
{
  while (GLFW_NO_ERROR == glfwWindowShouldClose(m_p_window)) {
    glfwPollEvents();

    int display_w, display_h;
    glfwGetFramebufferSize(m_p_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT);

    if (nullptr != m_p_renderer) {
      m_p_renderer->render();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_p_window);
  }

  if (nullptr != m_p_renderer) {
    m_p_renderer->shutdown();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_p_window);
  glfwTerminate();
}

Texture
OpenGL3Backend::load_texture(const uint8_t* pixels,
                             uint32_t width,
                             uint32_t height) const
{
  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               width,
               height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               pixels);

  return Texture{
    .ds = reinterpret_cast<VkDescriptorSet>(texture),
  };
}

void
OpenGL3Backend::unload_texture(const Texture& texture) const
{
}
}