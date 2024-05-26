#pragma once

#include <cstdint>
#include <neon/backends/i_backend.hpp>
#include <neon/renderers/renderer.hpp>

namespace neon {
class IBackend;

class BasicRaytracer : public Renderer
{
public:
  BasicRaytracer(const IBackend& backend);

  void init() override;
  void render() override;
  void shutdown() override;

private:
  uint32_t image_width = 256;
  uint32_t image_height = 256;

  Texture image_texture;
};
}