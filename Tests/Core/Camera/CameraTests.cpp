// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define private public
#define protected public

#include "Core/Camera/Camera.h"

#undef private
#undef protected



class MockRenderer : public CW::Renderer::Renderer {
public:
  MockRenderer() {}

  const CW::Renderer::WindowData* getWindowData() override { return nullptr; };
  const CW::Renderer::InputData* getInputData() override { return nullptr; };
  // void setKeyboardBind(const std::string& action, char key) override {};

  // void windowLessRenderer() override {};
  void createWindow() override {};
  APIWindow* getWindow() override { return nullptr; };
  void createRenderer() override {};

  void beginFrame() override {};
  void swapBuffer() override {};
  void windowEvents() override {};

  void setWindowMode(CW::Renderer::WindowMode mode) override { };
  void setWindowTitle(const std::string& title) override { };
  void setIcon(const std::string& path) override { };
  void setVsync(bool vsync) override { };
  void minimize(bool minimize) override { };
  void maximize(bool maximize) override { };
  void setPosition(int x, int y) override { };
  void setSize(int width, int height) override { };
  void setCursorVisibility(bool visible) override { };
  void setCursorOn(bool on) override { };
  void close() override { };
};



//// ==================== ////
//// === Constructors === ////
//// ==================== ////
TEST(CameraDefaultConstructors, HandlesInitialization) {
  Engine::Core::Camera camera;

  EXPECT_EQ(camera.renderer, nullptr);
  EXPECT_GE(camera.fov, 0);
  EXPECT_GE(camera.ortho_size, 0);
  EXPECT_GE(camera.sensitivity, 0);
  EXPECT_GE(camera.velocity, 0);
};



TEST(CameraDefaultConstructorsWithNullptr, HandlesInitialization) {
  MockRenderer* renderer = nullptr;
  Engine::Core::Camera camera(renderer);

  EXPECT_EQ(camera.renderer, nullptr);
  EXPECT_GE(camera.fov, 0);
  EXPECT_GE(camera.ortho_size, 0);
  EXPECT_GE(camera.sensitivity, 0);
  EXPECT_GE(camera.velocity, 0);
};



TEST(CameraDefaultParamConstructors, HandlesInitialization) {
  glm::vec3 init_pos = {0.1f, -2.0f, 5.0f};
  glm::vec3 init_dir = glm::normalize(glm::vec3(0.2f, 0.5f, -0.3f));
  
  MockRenderer renderer;
  Engine::Core::Camera camera(&renderer, init_pos, init_dir);

  EXPECT_NE(camera.renderer, nullptr);

  glm::vec3 actual_pos = camera.getPosition();
  EXPECT_NEAR(actual_pos.x, init_pos.x, 0.0001f);
  EXPECT_NEAR(actual_pos.y, init_pos.y, 0.0001f);
  EXPECT_NEAR(actual_pos.z, init_pos.z, 0.0001f);

  glm::vec3 actual_dir = camera.getDirection();
  EXPECT_NEAR(actual_dir.x, init_dir.x, 0.0001f);
  EXPECT_NEAR(actual_dir.y, init_dir.y, 0.0001f);
  EXPECT_NEAR(actual_dir.z, init_dir.z, 0.0001f);

  EXPECT_GE(camera.fov, 0);
  EXPECT_GE(camera.ortho_size, 0);
  EXPECT_GE(camera.sensitivity, 0);
  EXPECT_GE(camera.velocity, 0);
};
