#pragma once

#include <cstdint>
#include <glm/ext/vector_float4.hpp>
#include <neon/backends/i_backend.hpp>
#include <neon/renderers/renderer.hpp>
#include <vector>

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
  void set_pixel(std::vector<uint8_t>& pixels,
                 const glm::vec4& color,
                 size_t index) const;

  float m_render_scale = 1.0f;

  Texture m_render_texture;
};
}