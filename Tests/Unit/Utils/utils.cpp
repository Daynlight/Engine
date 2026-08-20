// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include "random"


#define private public
#define protected public

#include "Renderer.h"
#include "Utils/utils.h"

#undef private
#undef protected



TEST(UtilsHash, HandlesInitialization){
  const unsigned int seed = 2123123;
  const size_t random_numbers_to_test = 25;
  const float min = -200.0f;
  const float max = 200.0f;

  std::mt19937 gen(seed);
  std::uniform_real_distribution<float> dist(min, max);

  std::vector<float> init_values = {0.0f, static_cast<float>(0x45d9f3b), 16.0f};
  init_values.reserve(random_numbers_to_test);
  
  for (size_t i = 0; i < random_numbers_to_test; ++i) {
    init_values.emplace_back(dist(gen));
  };

  for (size_t i = 0; i < init_values.size(); ++i) {
    float value = init_values[i];
    
    uint32_t hashed_value1 = Engine::Utils::hash(value);
    EXPECT_NE(value, hashed_value1);
    
    uint32_t hashed_value2 = Engine::Utils::hash(value);
    EXPECT_EQ(hashed_value1, hashed_value2);
  };
};

TEST(UtilsUploadTypedBuffer, HandlesInitialization){
  CW::Renderer::Renderer renderer;
  CW::Renderer::Mesh mesh;

  const size_t random_tests = 25;
  const unsigned int seed = 2123123;
  const float min = -200.0f;
  const float max = 200.0f;

  std::mt19937 gen(seed);
  std::uniform_real_distribution<float> dist(min, max);

  std::vector<glm::vec3> data = { glm::vec3(1.0f) };

  for(size_t i = 0; i < random_tests; i++){
    float rand_val = dist(gen);
    data.emplace_back(glm::vec3(rand_val));
  };

  const unsigned int dimensions = 3;
  const unsigned int layout = 0;
  const GLenum type = GL_FLOAT;

  const auto* bytePtr = reinterpret_cast<const uint8_t*>(&data);
  std::vector<uint8_t> buffer(bytePtr, bytePtr + sizeof(glm::vec3) * data.size());
  
  Engine::Utils::uploadTypedBuffer<float>(mesh, buffer, dimensions, layout, GL_FLOAT);
  
  auto& data_reg = mesh.dataRegister;
  auto it = data_reg.find(layout);
  EXPECT_NE(it, data_reg.end());
  if(it == data_reg.end()) return;

  CW::Renderer::MeshData test_data = it->second;
  EXPECT_EQ(test_data.dimension, dimensions);
  EXPECT_EQ(test_data.type, type);
  EXPECT_EQ(test_data.size_of_element, sizeof(float));
  EXPECT_EQ(test_data.size, sizeof(float) * dimensions * data.size());
};

TEST(UtilsuploadBufferByType, HandlesInitialization){
  CW::Renderer::Renderer renderer;

  const size_t random_tests = 25;
  const unsigned int seed = 2123123;
  const float min = -200.0f;
  const float max = 200.0f;

  std::mt19937 gen(seed);
  std::uniform_real_distribution<float> dist(min, max);

  std::vector<glm::vec3> data = { glm::vec3(1.0f) };

  for(size_t i = 0; i < random_tests; i++){
    float rand_val = dist(gen);
    data.emplace_back(glm::vec3(rand_val));
  };

  const unsigned int dimensions = 3;
  const unsigned int layout = 0;
  std::vector<GLenum> types = {GL_FLOAT, GL_UNSIGNED_INT};

  for(GLenum type : types){
    CW::Renderer::Mesh mesh;

    const auto* bytePtr = reinterpret_cast<const uint8_t*>(&data);
    std::vector<uint8_t> buffer(bytePtr, bytePtr + sizeof(glm::vec3) * data.size());
    
    Engine::Utils::uploadBufferByType(mesh, type, buffer, dimensions, layout);
    
    auto& data_reg = mesh.dataRegister;
    auto it = data_reg.find(layout);
    EXPECT_NE(it, data_reg.end());
    if(it == data_reg.end()) return;
    
    CW::Renderer::MeshData test_data = it->second;
    EXPECT_EQ(test_data.dimension, dimensions);
    EXPECT_EQ(test_data.type, type);
    EXPECT_EQ(test_data.size_of_element, sizeof(float));
    EXPECT_EQ(test_data.size, sizeof(float) * dimensions * data.size());
  };
};