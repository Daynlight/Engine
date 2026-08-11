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

  MOCK_METHOD(const CW::Renderer::WindowData*, getWindowData, (), (override));
  MOCK_METHOD(const CW::Renderer::InputData*, getInputData, (), (override));
  // MOCK_METHOD(void, setKeyboardBind, (const std::string& action, char key), (override));

  MOCK_METHOD(void, createWindow, (), (override));
  // MOCK_METHOD(void, windowLessRenderer, (), (override));
  MOCK_METHOD(APIWindow*, getWindow, (), (override));
  void createRenderer() override {};

  MOCK_METHOD(void, beginFrame, (), (override));
  MOCK_METHOD(void, swapBuffer, (), (override));
  MOCK_METHOD(void, windowEvents, (), (override));

  MOCK_METHOD(void, setWindowMode, (CW::Renderer::WindowMode mode), (override));
  MOCK_METHOD(void, setWindowTitle, (const std::string& title), (override));
  MOCK_METHOD(void, setIcon, (const std::string& path), (override));
  MOCK_METHOD(void, setVsync, (bool vsync), (override));
  MOCK_METHOD(void, minimize, (bool minimize), (override));
  MOCK_METHOD(void, maximize, (bool maximize), (override));
  MOCK_METHOD(void, setPosition, (int x, int y), (override));
  MOCK_METHOD(void, setSize, (int width, int height), (override));
  MOCK_METHOD(void, setCursorVisibility, (bool visible), (override));
  MOCK_METHOD(void, setCursorOn, (bool on), (override));
  MOCK_METHOD(void, close, (), (override));
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
