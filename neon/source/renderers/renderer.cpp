#include <neon/renderers/renderer.hpp>

namespace neon {
Renderer::Renderer(const IBackend& backend)
  : backend(backend)
{
}
}