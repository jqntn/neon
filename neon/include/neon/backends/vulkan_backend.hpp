#pragma once

#include <cstdint>
#include <neon/backends/i_backend.hpp>

struct GLFWwindow;

namespace neon {
struct BackendConfig;

class VulkanBackend : public IBackend
{
public:
  void init(const BackendConfig& config) override;
  void run() override;

  Texture load_texture(const uint8_t* pixels,
                       uint32_t width,
                       uint32_t height) const override;
  void unload_texture(const Texture& texture) const override;

private:
  GLFWwindow* m_p_window = nullptr;
};
}