#include <cstdint>
#include <glm/ext/vector_float4.hpp>
#include <imgui.h>
#include <neon/backends/i_backend.hpp>
#include <neon/renderers/basic_raytracer.hpp>
#include <neon/renderers/renderer.hpp>
#include <vector>

namespace neon {
BasicRaytracer::BasicRaytracer(const IBackend& backend)
  : Renderer(backend)
{
}

void
BasicRaytracer::init()
{
}

void
BasicRaytracer::render()
{
  const ImVec2& work_size = ImGui::GetMainViewport()->WorkSize;

  uint32_t render_width = work_size.x * m_render_scale;
  uint32_t render_height = work_size.y * m_render_scale;

  std::vector<uint8_t> pixels(render_width * render_height * 4);

  for (float y = 0.0f; y < render_height; y++) {
    for (float x = 0.0f; x < render_width; x++) {
      float r = x / (render_width - 1);
      float g = y / (render_height - 1);
      float b = 0.0f;
      float a = 1.0f;
      set_pixel(pixels, glm::vec4(r, g, b, a), 4 * ((y * render_width) + x));
    }
  }

  backend.unload_texture(m_render_texture);

  m_render_texture =
    backend.load_texture(pixels.data(), render_width, render_height);

  ImGui::GetBackgroundDrawList()->AddImage(
    m_render_texture.ds, ImVec2(), ImGui::GetMainViewport()->WorkSize);
}

void
BasicRaytracer::shutdown()
{
  backend.unload_texture(m_render_texture);
}

void
BasicRaytracer::set_pixel(std::vector<uint8_t>& pixels,
                          const glm::vec4& color,
                          size_t index) const
{
  uint8_t ir = static_cast<uint8_t>(255.999f * color.r);
  uint8_t ig = static_cast<uint8_t>(255.999f * color.g);
  uint8_t ib = static_cast<uint8_t>(255.999f * color.b);
  uint8_t ia = static_cast<uint8_t>(255.999f * color.a);

  pixels[index + 0] = ir;
  pixels[index + 1] = ig;
  pixels[index + 2] = ib;
  pixels[index + 3] = ia;
}
}