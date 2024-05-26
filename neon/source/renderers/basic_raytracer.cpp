#include <cstdint>
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
  std::vector<uint8_t> pixels(image_width * image_height * 4);

  for (double y = 0; y < image_height; y++) {
    for (double x = 0; x < image_width; x++) {
      double r = x / (image_width - 1);
      double g = y / (image_height - 1);
      double b = 0.0;
      double a = 1.0;
      int ir = static_cast<int>(255.999 * r);
      int ig = static_cast<int>(255.999 * g);
      int ib = static_cast<int>(255.999 * b);
      int ia = static_cast<int>(255.999 * a);
      pixels[4 * (y * image_width + x) + 0] = ir;
      pixels[4 * (y * image_width + x) + 1] = ig;
      pixels[4 * (y * image_width + x) + 2] = ib;
      pixels[4 * (y * image_width + x) + 3] = ia;
    }
  }

  image_texture =
    backend.load_texture(pixels.data(), image_width, image_height);
}

void
BasicRaytracer::render()
{
  ImGui::Image(image_texture.ds, ImVec2(image_width, image_height));
}

void
BasicRaytracer::shutdown()
{
  backend.unload_texture(image_texture);
}
}