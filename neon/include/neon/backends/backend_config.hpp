#pragma once

#include <memory>

namespace neon {
class Renderer;

struct BackendConfig
{
  int window_width = 1280;
  int window_height = 720;
  const char* window_title = "window_title";
  std::unique_ptr<Renderer>&& p_renderer;
};
}