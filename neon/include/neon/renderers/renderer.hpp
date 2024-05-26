#pragma once

namespace neon {
class IBackend;

class Renderer
{
public:
  Renderer(const IBackend& backend);

  virtual void init() = 0;
  virtual void render() = 0;
  virtual void shutdown() = 0;

protected:
  const IBackend& backend;
};
}