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



//// ======================= /////
//// ======== Mocks ======== /////
//// ======================= /////
namespace Mock{

class Renderer : public CW::Renderer::Renderer {
public:
  Renderer() {}

  // MOCK_METHOD(const CW::Renderer::WindowData*, getWindowData, (), (override));
  // MOCK_METHOD(const CW::Renderer::InputData*, getInputData, (), (override));
  // MOCK_METHOD(void, setKeyboardBind, (const std::string& action, char key), (override));

  // MOCK_METHOD(void, createWindow, (), (override));
  // MOCK_METHOD(void, windowLessRenderer, (), (override));
  // MOCK_METHOD(APIWindow*, getWindow, (), (override));
  // void createRenderer() override {};

  // MOCK_METHOD(void, beginFrame, (), (override));
  // MOCK_METHOD(void, swapBuffer, (), (override));
  // MOCK_METHOD(void, windowEvents, (), (override));

  // MOCK_METHOD(void, setWindowMode, (CW::Renderer::WindowMode mode), (override));
  // MOCK_METHOD(void, setWindowTitle, (const std::string& title), (override));
  // MOCK_METHOD(void, setIcon, (const std::string& path), (override));
  // MOCK_METHOD(void, setVsync, (bool vsync), (override));
  // MOCK_METHOD(void, minimize, (bool minimize), (override));
  // MOCK_METHOD(void, maximize, (bool maximize), (override));
  // MOCK_METHOD(void, setPosition, (int x, int y), (override));
  // MOCK_METHOD(void, setSize, (int width, int height), (override));
  // MOCK_METHOD(void, setCursorVisibility, (bool visible), (override));
  // MOCK_METHOD(void, setCursorOn, (bool on), (override));
  // MOCK_METHOD(void, close, (), (override));
};
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
  Mock::Renderer* renderer = nullptr;

  Engine::Core::Camera camera(renderer);

  EXPECT_EQ(camera.renderer, nullptr);
  EXPECT_GE(camera.fov, 0);
  EXPECT_GE(camera.ortho_size, 0);
  EXPECT_GE(camera.sensitivity, 0);
  EXPECT_GE(camera.velocity, 0);
};

TEST(CameraDefaultParamConstructors, HandlesInitialization) {
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.1f, -2.0f, 5.0f};
  glm::vec3 init_dir = glm::normalize(glm::vec3(0.2f, 0.5f, -0.3f));
  
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

TEST(CameraCopyConstructor, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.1f, -2.0f, 5.0f};
  glm::vec3 init_dir = glm::normalize(glm::vec3(0.2f, 0.5f, -0.3f));

  Engine::Core::Camera init_camera(&renderer, init_pos, init_dir);

  Engine::Core::Camera construct_move_camera(init_camera);
  
  EXPECT_NE(construct_move_camera.renderer, nullptr);
  EXPECT_EQ(init_camera.position, construct_move_camera.position);
  EXPECT_EQ(init_camera.direction, construct_move_camera.direction);
  EXPECT_EQ(init_camera.orientation, construct_move_camera.orientation);
  EXPECT_EQ(init_camera.fov, construct_move_camera.fov);
  EXPECT_EQ(init_camera.ortho_size, construct_move_camera.ortho_size);
  EXPECT_EQ(init_camera.default_movemement_on, construct_move_camera.default_movemement_on);
  EXPECT_EQ(init_camera.sensitivity, construct_move_camera.sensitivity);
  EXPECT_EQ(init_camera.velocity, construct_move_camera.velocity);
  EXPECT_EQ(init_camera.mouse_is_active, construct_move_camera.mouse_is_active);
  EXPECT_EQ(init_camera.cursor_lock, construct_move_camera.cursor_lock);

  Engine::Core::Camera construct_copy_assign_camera = init_camera;
  
  EXPECT_NE(construct_copy_assign_camera.renderer, nullptr);
  EXPECT_EQ(init_camera.position, construct_copy_assign_camera.position);
  EXPECT_EQ(init_camera.direction, construct_copy_assign_camera.direction);
  EXPECT_EQ(init_camera.orientation, construct_copy_assign_camera.orientation);
  EXPECT_EQ(init_camera.fov, construct_copy_assign_camera.fov);
  EXPECT_EQ(init_camera.ortho_size, construct_copy_assign_camera.ortho_size);
  EXPECT_EQ(init_camera.default_movemement_on, construct_copy_assign_camera.default_movemement_on);
  EXPECT_EQ(init_camera.sensitivity, construct_copy_assign_camera.sensitivity);
  EXPECT_EQ(init_camera.velocity, construct_copy_assign_camera.velocity);
  EXPECT_EQ(init_camera.mouse_is_active, construct_copy_assign_camera.mouse_is_active);
  EXPECT_EQ(init_camera.cursor_lock, construct_copy_assign_camera.cursor_lock);

  Engine::Core::Camera construct_copy_self_camera(init_camera);
  Engine::Core::Camera* org_camera_ptr = &construct_copy_self_camera;
  construct_copy_self_camera = construct_copy_self_camera;
  Engine::Core::Camera* new_camera_ptr = &construct_copy_self_camera;
  
  EXPECT_EQ(org_camera_ptr, new_camera_ptr);
  EXPECT_NE(construct_copy_self_camera.renderer, nullptr);
  EXPECT_EQ(init_camera.position, construct_copy_self_camera.position);
  EXPECT_EQ(init_camera.direction, construct_copy_self_camera.direction);
  EXPECT_EQ(init_camera.orientation, construct_copy_self_camera.orientation);
  EXPECT_EQ(init_camera.fov, construct_copy_self_camera.fov);
  EXPECT_EQ(init_camera.ortho_size, construct_copy_self_camera.ortho_size);
  EXPECT_EQ(init_camera.default_movemement_on, construct_copy_self_camera.default_movemement_on);
  EXPECT_EQ(init_camera.sensitivity, construct_copy_self_camera.sensitivity);
  EXPECT_EQ(init_camera.velocity, construct_copy_self_camera.velocity);
  EXPECT_EQ(init_camera.mouse_is_active, construct_copy_self_camera.mouse_is_active);
  EXPECT_EQ(init_camera.cursor_lock, construct_copy_self_camera.cursor_lock);
};

TEST(CameraMoveConstructor, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.1f, -2.0f, 5.0f};
  glm::vec3 init_dir = glm::normalize(glm::vec3(0.2f, 0.5f, -0.3f));

  Engine::Core::Camera init_org_camera(&renderer, init_pos, init_dir);
  
  Engine::Core::Camera init_constructor_move_camera(init_org_camera);
  Engine::Core::Camera construct_move_camera(std::move(init_constructor_move_camera));
  
  EXPECT_NE(init_constructor_move_camera.renderer, nullptr);
  EXPECT_NE(init_org_camera.renderer, nullptr);
  EXPECT_EQ(init_org_camera.position, construct_move_camera.position);
  EXPECT_EQ(init_org_camera.direction, construct_move_camera.direction);
  EXPECT_EQ(init_org_camera.orientation, construct_move_camera.orientation);
  EXPECT_EQ(init_org_camera.fov, construct_move_camera.fov);
  EXPECT_EQ(init_org_camera.ortho_size, construct_move_camera.ortho_size);
  EXPECT_EQ(init_org_camera.default_movemement_on, construct_move_camera.default_movemement_on);
  EXPECT_EQ(init_org_camera.sensitivity, construct_move_camera.sensitivity);
  EXPECT_EQ(init_org_camera.velocity, construct_move_camera.velocity);
  EXPECT_EQ(init_org_camera.mouse_is_active, construct_move_camera.mouse_is_active);
  EXPECT_EQ(init_org_camera.cursor_lock, construct_move_camera.cursor_lock);

  Engine::Core::Camera init_construct_move_assign_camera(init_org_camera);
  Engine::Core::Camera construct_move_assign_camera = std::move(init_construct_move_assign_camera);
  
  EXPECT_NE(init_construct_move_assign_camera.renderer, nullptr);
  EXPECT_NE(construct_move_assign_camera.renderer, nullptr);
  EXPECT_EQ(init_org_camera.position, construct_move_assign_camera.position);
  EXPECT_EQ(init_org_camera.direction, construct_move_assign_camera.direction);
  EXPECT_EQ(init_org_camera.orientation, construct_move_assign_camera.orientation);
  EXPECT_EQ(init_org_camera.fov, construct_move_assign_camera.fov);
  EXPECT_EQ(init_org_camera.ortho_size, construct_move_assign_camera.ortho_size);
  EXPECT_EQ(init_org_camera.default_movemement_on, construct_move_assign_camera.default_movemement_on);
  EXPECT_EQ(init_org_camera.sensitivity, construct_move_assign_camera.sensitivity);
  EXPECT_EQ(init_org_camera.velocity, construct_move_assign_camera.velocity);
  EXPECT_EQ(init_org_camera.mouse_is_active, construct_move_assign_camera.mouse_is_active);
  EXPECT_EQ(init_org_camera.cursor_lock, construct_move_assign_camera.cursor_lock);

  Engine::Core::Camera init_construct_move_self_camera(init_org_camera);
  Engine::Core::Camera construct_move_self_camera(std::move(init_construct_move_self_camera));
  Engine::Core::Camera* org_camera_ptr = &construct_move_self_camera;
  construct_move_self_camera = std::move(init_construct_move_self_camera);
  Engine::Core::Camera* new_camera_ptr = &construct_move_self_camera;
  
  EXPECT_EQ(org_camera_ptr, new_camera_ptr);
  EXPECT_NE(init_construct_move_self_camera.renderer, nullptr);
  EXPECT_NE(construct_move_self_camera.renderer, nullptr);
  EXPECT_EQ(init_org_camera.position, construct_move_self_camera.position);
  EXPECT_EQ(init_org_camera.direction, construct_move_self_camera.direction);
  EXPECT_EQ(init_org_camera.orientation, construct_move_self_camera.orientation);
  EXPECT_EQ(init_org_camera.fov, construct_move_self_camera.fov);
  EXPECT_EQ(init_org_camera.ortho_size, construct_move_self_camera.ortho_size);
  EXPECT_EQ(init_org_camera.default_movemement_on, construct_move_self_camera.default_movemement_on);
  EXPECT_EQ(init_org_camera.sensitivity, construct_move_self_camera.sensitivity);
  EXPECT_EQ(init_org_camera.velocity, construct_move_self_camera.velocity);
  EXPECT_EQ(init_org_camera.mouse_is_active, construct_move_self_camera.mouse_is_active);
  EXPECT_EQ(init_org_camera.cursor_lock, construct_move_self_camera.cursor_lock);
};



//// ==================== ////
//// ==== Projection ==== ////
//// ==================== ////