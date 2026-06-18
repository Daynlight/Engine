#pragma once
#include "Renderer.h"

#include <random>

#include "Resources/Resources.h"
#include "SDF.h"
#include "config.h"


namespace UW{
class Meduse{
private:
  CW::Renderer::Uniform sdf_uniform;
  glm::vec3 position = {153.0f, 28.0f, -116.0f};
  glm::vec3 rotation = {0.2f, 0.707f, 0.0f};
  glm::vec3 scale = {0.5f, 0.5f, 0.5f};
  UW::SDF sdf;

public:
  Meduse(): sdf("SDF"){};
  ~Meduse(){};

  void render(CW::Renderer::Framebuffer& fbo, UW::Camera& camera, CW::Renderer::Renderer& window){
    glm::vec3 pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMat = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 preRotate = glm::translate(glm::mat4(1.0f), -pivotOffset);
    glm::mat4 postRotate = glm::translate(glm::mat4(1.0f), pivotOffset);
    glm::mat4 model = translationMat * postRotate * rotationMat * preRotate * scaleMat;

    sdf_uniform["model"]->set<glm::mat4>(model);

    sdf_uniform["material_id"]->set<int>(Resources::get().materials.translate_material("SDF"));
    sdf.render(fbo, camera, window, &sdf_uniform);
  };

  void setPos(glm::vec3 position){
    this->position = position;
  };

  void genRandom(int i, glm::vec3 position_min, glm::vec3 position_max, glm::vec3 center){
    unsigned int seed = UW::Config::SEED + i * 213123123;
    std::mt19937 gen(seed);
    
    std::uniform_real_distribution<float> randomX(position_min.x, position_max.x);
    std::uniform_real_distribution<float> randomY(position_min.y, position_max.y);
    std::uniform_real_distribution<float> randomZ(position_min.z, position_max.z);

    setPos(glm::vec3(randomX(gen), randomY(gen), randomZ(gen)) + center);
  };
};
};