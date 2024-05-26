#pragma once

#include <cstdint>
#include <memory>
#include <vulkan/vulkan_core.h>

namespace neon {
class Renderer;
struct BackendConfig;

struct Texture
{
  VkDescriptorSet ds = nullptr;

  VkImageView vk_image_view = nullptr;
  VkImage vk_image = nullptr;
  VkDeviceMemory vk_image_memory = nullptr;
  VkSampler vk_sampler = nullptr;
  VkBuffer vk_upload_buffer = nullptr;
  VkDeviceMemory vk_upload_buffer_memory = nullptr;
};

class IBackend
{
public:
  virtual void init(const BackendConfig& config) = 0;
  virtual void run() = 0;

  virtual Texture load_texture(const uint8_t* pixels,
                               uint32_t width,
                               uint32_t height) const = 0;
  virtual void unload_texture(const Texture& texture) const = 0;

protected:
  std::unique_ptr<Renderer> m_p_renderer;
};
}