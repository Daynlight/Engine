#pragma once
#include "Renderer.h"

#include "Camera/Camera.h"
#include "Resources/Resources.h"



namespace UW{
struct SDFObject{
  alignas(16) int type;
  alignas(16) glm::vec3 position;
  alignas(16) float size;

  SDFObject(int type, glm::vec3 position, float size);
};



class SDF{
private:
  CW::Renderer::GPUStore buffer;
  bool is_compiled = false;
  std::vector<SDFObject> data;

public:
  SDF();
  ~SDF();

  void render(CW::Renderer::Framebuffer& fbo, UW::Camera& camera, CW::Renderer::Renderer& window);

  void compile();
  void destroy();
  void bind(GLuint socket);
  void unbind();

  SDFObject& operator[](unsigned int index);
  SDFObject get(unsigned int index) const;

  void clear();
  void erase(unsigned int index);
  unsigned int size() const;

  void emplace_back(SDFObject sdf);
  void emplace_back(std::initializer_list<SDFObject> sdfs);
};
};
