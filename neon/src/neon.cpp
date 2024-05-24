#include "contexts/opengl3_context.hpp"
#include "contexts/vulkan_context.hpp"

int
main()
{
  return opengl3_init();
  return vulkan_init();
}