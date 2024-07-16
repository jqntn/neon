#include <cstdlib>
#include <memory>
#include <neon/backends/backend_config.hpp>
#include <neon/backends/opengl3_backend.hpp>
#include <neon/backends/vulkan_backend.hpp>
#include <neon/renderers/basic_raytracer.hpp>

int
main()
{
  // neon::OpenGL3Backend backend;
  neon::VulkanBackend backend;

  neon::BackendConfig config{
    .window_width = 1280,
    .window_height = 720,
    .window_title = "basic_raytracer",
    .p_renderer = std::make_unique<neon::BasicRaytracer>(backend),
  };

  backend.init(config);
  backend.run();

  return EXIT_SUCCESS;
}