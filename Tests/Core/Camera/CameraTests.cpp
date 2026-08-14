// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"

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

  Engine::Core::Camera construct_copy_camera(init_camera);
  
  EXPECT_NE(construct_copy_camera.renderer, nullptr);
  EXPECT_EQ(init_camera.position, construct_copy_camera.position);
  EXPECT_EQ(init_camera.direction, construct_copy_camera.direction);
  EXPECT_EQ(init_camera.orientation, construct_copy_camera.orientation);
  EXPECT_EQ(init_camera.fov, construct_copy_camera.fov);
  EXPECT_EQ(init_camera.ortho_size, construct_copy_camera.ortho_size);
  EXPECT_EQ(init_camera.transform_mat_ready, construct_copy_camera.transform_mat_ready);
  EXPECT_EQ(init_camera.transform_mat, construct_copy_camera.transform_mat);
  EXPECT_EQ(init_camera.view_mat_ready, construct_copy_camera.view_mat_ready);
  EXPECT_EQ(init_camera.view_mat, construct_copy_camera.view_mat);
  EXPECT_EQ(init_camera.last_aspect_ratio_orthogonal, construct_copy_camera.last_aspect_ratio_orthogonal);
  EXPECT_EQ(init_camera.last_aspect_ratio_perspective, construct_copy_camera.last_aspect_ratio_perspective);
  EXPECT_EQ(init_camera.perspective_near_plane, construct_copy_camera.perspective_near_plane);
  EXPECT_EQ(init_camera.orthogonal_near_plane, construct_copy_camera.orthogonal_near_plane);
  EXPECT_EQ(init_camera.perspective_far_plane, construct_copy_camera.perspective_far_plane);
  EXPECT_EQ(init_camera.orthogonal_far_plane, construct_copy_camera.orthogonal_far_plane);
  EXPECT_EQ(init_camera.perspective_mat_ready, construct_copy_camera.perspective_mat_ready);
  EXPECT_EQ(init_camera.orthogonal_mat_ready, construct_copy_camera.orthogonal_mat_ready);
  EXPECT_EQ(init_camera.perspective_mat, construct_copy_camera.perspective_mat);
  EXPECT_EQ(init_camera.orthogonal_mat, construct_copy_camera.orthogonal_mat);
  EXPECT_EQ(init_camera.default_movemement_on, construct_copy_camera.default_movemement_on);
  EXPECT_EQ(init_camera.sensitivity, construct_copy_camera.sensitivity);
  EXPECT_EQ(init_camera.velocity, construct_copy_camera.velocity);
  EXPECT_EQ(init_camera.mouse_is_active, construct_copy_camera.mouse_is_active);
  EXPECT_EQ(init_camera.cursor_lock, construct_copy_camera.cursor_lock);

  Engine::Core::Camera construct_copy_assign_camera = init_camera;
  
  EXPECT_NE(construct_copy_assign_camera.renderer, nullptr);
  EXPECT_EQ(init_camera.position, construct_copy_assign_camera.position);
  EXPECT_EQ(init_camera.direction, construct_copy_assign_camera.direction);
  EXPECT_EQ(init_camera.orientation, construct_copy_assign_camera.orientation);
  EXPECT_EQ(init_camera.fov, construct_copy_assign_camera.fov);
  EXPECT_EQ(init_camera.ortho_size, construct_copy_assign_camera.ortho_size);
  EXPECT_EQ(init_camera.transform_mat_ready, construct_copy_assign_camera.transform_mat_ready);
  EXPECT_EQ(init_camera.transform_mat, construct_copy_assign_camera.transform_mat);
  EXPECT_EQ(init_camera.view_mat_ready, construct_copy_assign_camera.view_mat_ready);
  EXPECT_EQ(init_camera.view_mat, construct_copy_assign_camera.view_mat);
  EXPECT_EQ(init_camera.last_aspect_ratio_orthogonal, construct_copy_assign_camera.last_aspect_ratio_orthogonal);
  EXPECT_EQ(init_camera.last_aspect_ratio_perspective, construct_copy_assign_camera.last_aspect_ratio_perspective);
  EXPECT_EQ(init_camera.perspective_near_plane, construct_copy_assign_camera.perspective_near_plane);
  EXPECT_EQ(init_camera.orthogonal_near_plane, construct_copy_assign_camera.orthogonal_near_plane);
  EXPECT_EQ(init_camera.perspective_far_plane, construct_copy_assign_camera.perspective_far_plane);
  EXPECT_EQ(init_camera.orthogonal_far_plane, construct_copy_assign_camera.orthogonal_far_plane);
  EXPECT_EQ(init_camera.perspective_mat_ready, construct_copy_assign_camera.perspective_mat_ready);
  EXPECT_EQ(init_camera.orthogonal_mat_ready, construct_copy_assign_camera.orthogonal_mat_ready);
  EXPECT_EQ(init_camera.perspective_mat, construct_copy_assign_camera.perspective_mat);
  EXPECT_EQ(init_camera.orthogonal_mat, construct_copy_assign_camera.orthogonal_mat);
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
  EXPECT_EQ(init_camera.transform_mat_ready, construct_copy_self_camera.transform_mat_ready);
  EXPECT_EQ(init_camera.transform_mat, construct_copy_self_camera.transform_mat);
  EXPECT_EQ(init_camera.view_mat_ready, construct_copy_self_camera.view_mat_ready);
  EXPECT_EQ(init_camera.view_mat, construct_copy_self_camera.view_mat);
  EXPECT_EQ(init_camera.last_aspect_ratio_orthogonal, construct_copy_self_camera.last_aspect_ratio_orthogonal);
  EXPECT_EQ(init_camera.last_aspect_ratio_perspective, construct_copy_self_camera.last_aspect_ratio_perspective);
  EXPECT_EQ(init_camera.perspective_near_plane, construct_copy_self_camera.perspective_near_plane);
  EXPECT_EQ(init_camera.orthogonal_near_plane, construct_copy_self_camera.orthogonal_near_plane);
  EXPECT_EQ(init_camera.perspective_far_plane, construct_copy_self_camera.perspective_far_plane);
  EXPECT_EQ(init_camera.orthogonal_far_plane, construct_copy_self_camera.orthogonal_far_plane);
  EXPECT_EQ(init_camera.perspective_mat_ready, construct_copy_self_camera.perspective_mat_ready);
  EXPECT_EQ(init_camera.orthogonal_mat_ready, construct_copy_self_camera.orthogonal_mat_ready);
  EXPECT_EQ(init_camera.perspective_mat, construct_copy_self_camera.perspective_mat);
  EXPECT_EQ(init_camera.orthogonal_mat, construct_copy_self_camera.orthogonal_mat);
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
  EXPECT_EQ(init_org_camera.transform_mat_ready, construct_move_camera.transform_mat_ready);
  EXPECT_EQ(init_org_camera.transform_mat, construct_move_camera.transform_mat);
  EXPECT_EQ(init_org_camera.view_mat_ready, construct_move_camera.view_mat_ready);
  EXPECT_EQ(init_org_camera.view_mat, construct_move_camera.view_mat);
  EXPECT_EQ(init_org_camera.last_aspect_ratio_orthogonal, construct_move_camera.last_aspect_ratio_orthogonal);
  EXPECT_EQ(init_org_camera.last_aspect_ratio_perspective, construct_move_camera.last_aspect_ratio_perspective);
  EXPECT_EQ(init_org_camera.perspective_near_plane, construct_move_camera.perspective_near_plane);
  EXPECT_EQ(init_org_camera.orthogonal_near_plane, construct_move_camera.orthogonal_near_plane);
  EXPECT_EQ(init_org_camera.perspective_far_plane, construct_move_camera.perspective_far_plane);
  EXPECT_EQ(init_org_camera.orthogonal_far_plane, construct_move_camera.orthogonal_far_plane);
  EXPECT_EQ(init_org_camera.perspective_mat_ready, construct_move_camera.perspective_mat_ready);
  EXPECT_EQ(init_org_camera.orthogonal_mat_ready, construct_move_camera.orthogonal_mat_ready);
  EXPECT_EQ(init_org_camera.perspective_mat, construct_move_camera.perspective_mat);
  EXPECT_EQ(init_org_camera.orthogonal_mat, construct_move_camera.orthogonal_mat);
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
  EXPECT_EQ(init_org_camera.transform_mat_ready, construct_move_assign_camera.transform_mat_ready);
  EXPECT_EQ(init_org_camera.transform_mat, construct_move_assign_camera.transform_mat);
  EXPECT_EQ(init_org_camera.view_mat_ready, construct_move_assign_camera.view_mat_ready);
  EXPECT_EQ(init_org_camera.view_mat, construct_move_assign_camera.view_mat);
  EXPECT_EQ(init_org_camera.last_aspect_ratio_orthogonal, construct_move_assign_camera.last_aspect_ratio_orthogonal);
  EXPECT_EQ(init_org_camera.last_aspect_ratio_perspective, construct_move_assign_camera.last_aspect_ratio_perspective);
  EXPECT_EQ(init_org_camera.perspective_near_plane, construct_move_assign_camera.perspective_near_plane);
  EXPECT_EQ(init_org_camera.orthogonal_near_plane, construct_move_assign_camera.orthogonal_near_plane);
  EXPECT_EQ(init_org_camera.perspective_far_plane, construct_move_assign_camera.perspective_far_plane);
  EXPECT_EQ(init_org_camera.orthogonal_far_plane, construct_move_assign_camera.orthogonal_far_plane);
  EXPECT_EQ(init_org_camera.perspective_mat_ready, construct_move_assign_camera.perspective_mat_ready);
  EXPECT_EQ(init_org_camera.orthogonal_mat_ready, construct_move_assign_camera.orthogonal_mat_ready);
  EXPECT_EQ(init_org_camera.perspective_mat, construct_move_assign_camera.perspective_mat);
  EXPECT_EQ(init_org_camera.orthogonal_mat, construct_move_assign_camera.orthogonal_mat);
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
  EXPECT_EQ(init_org_camera.transform_mat_ready, construct_move_self_camera.transform_mat_ready);
  EXPECT_EQ(init_org_camera.transform_mat, construct_move_self_camera.transform_mat);
  EXPECT_EQ(init_org_camera.view_mat_ready, construct_move_self_camera.view_mat_ready);
  EXPECT_EQ(init_org_camera.view_mat, construct_move_self_camera.view_mat);
  EXPECT_EQ(init_org_camera.last_aspect_ratio_orthogonal, construct_move_self_camera.last_aspect_ratio_orthogonal);
  EXPECT_EQ(init_org_camera.last_aspect_ratio_perspective, construct_move_self_camera.last_aspect_ratio_perspective);
  EXPECT_EQ(init_org_camera.perspective_near_plane, construct_move_self_camera.perspective_near_plane);
  EXPECT_EQ(init_org_camera.orthogonal_near_plane, construct_move_self_camera.orthogonal_near_plane);
  EXPECT_EQ(init_org_camera.perspective_far_plane, construct_move_self_camera.perspective_far_plane);
  EXPECT_EQ(init_org_camera.orthogonal_far_plane, construct_move_self_camera.orthogonal_far_plane);
  EXPECT_EQ(init_org_camera.perspective_mat_ready, construct_move_self_camera.perspective_mat_ready);
  EXPECT_EQ(init_org_camera.orthogonal_mat_ready, construct_move_self_camera.orthogonal_mat_ready);
  EXPECT_EQ(init_org_camera.perspective_mat, construct_move_self_camera.perspective_mat);
  EXPECT_EQ(init_org_camera.orthogonal_mat, construct_move_self_camera.orthogonal_mat);
  EXPECT_EQ(init_org_camera.default_movemement_on, construct_move_self_camera.default_movemement_on);
  EXPECT_EQ(init_org_camera.sensitivity, construct_move_self_camera.sensitivity);
  EXPECT_EQ(init_org_camera.velocity, construct_move_self_camera.velocity);
  EXPECT_EQ(init_org_camera.mouse_is_active, construct_move_self_camera.mouse_is_active);
  EXPECT_EQ(init_org_camera.cursor_lock, construct_move_self_camera.cursor_lock);
};



//// ==================== ////
//// ==== Projection ==== ////
//// ==================== ////
TEST(CameraPerspectiveProjection, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 init_dir = {0.0f, 0.0f, 1.0f};
  float init_fov = 60.0f;
  float init_near_plane = Engine::Config::CAMERA_NEAR_PLANE;
  float init_far_plane = Engine::Config::CAMERA_FAR_PLANE;

  
  Engine::Core::Camera camera(&renderer, init_pos, init_dir);
  camera.setFov(init_fov);
  camera.setNearPlane(init_near_plane);
  camera.setFarPlane(init_far_plane);
  
  float aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  glm::mat4 expected_mat = glm::perspective(glm::radians(init_fov), aspectRatio, init_near_plane, init_far_plane);
  
  glm::vec4 testing_point1 = glm::vec4(-3.0f, 2.0f, -10.0f, 1.0f); 
  glm::vec4 transformed_point1 = camera.perspective_projection() * testing_point1;
  glm::vec4 expected1 = expected_mat * testing_point1;
  
  EXPECT_NEAR(transformed_point1.w, 10.0f, 0.001f);
  EXPECT_NEAR(transformed_point1.x, expected1.x, 0.001f);
  EXPECT_NEAR(transformed_point1.y, expected1.y, 0.001f);
  EXPECT_NEAR(transformed_point1.z, expected1.z, 0.001f);

  glm::vec3 ndc1 = glm::vec3(transformed_point1) / transformed_point1.w;
  
  EXPECT_GE(ndc1.x, -1.0f);
  EXPECT_LE(ndc1.x, 1.0f);
  EXPECT_GE(ndc1.y, -1.0f);
  EXPECT_LE(ndc1.y, 1.0f);
  EXPECT_GE(ndc1.z, -1.0f);
  EXPECT_LE(ndc1.z, 1.0f);

  glm::vec4 testing_point2 = {2.0f, 12.0f, -25.0f, 1.0f}; 
  glm::vec4 transformed_point2 = camera.perspective_projection() * testing_point2;
  glm::vec4 expected2 = expected_mat * testing_point2;
  
  EXPECT_NEAR(transformed_point2.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point2.x, expected2.x, 0.001f);
  EXPECT_NEAR(transformed_point2.y, expected2.y, 0.001f);
  EXPECT_NEAR(transformed_point2.z, expected2.z, 0.001f);

  glm::vec3 ndc2 = glm::vec3(transformed_point2) / transformed_point2.w;
  
  EXPECT_GE(ndc2.x, -1.0f);
  EXPECT_LE(ndc2.x, 1.0f);
  EXPECT_GE(ndc2.y, -1.0f);
  EXPECT_LE(ndc2.y, 1.0f);
  EXPECT_GE(ndc2.z, -1.0f);
  EXPECT_LE(ndc2.z, 1.0f);

  float new_fov = 65.0f;
  camera.setFov(new_fov);
  glm::vec4 testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  glm::vec4 transformed_point = camera.perspective_projection() * testing_point;
  expected_mat = glm::perspective(glm::radians(new_fov), aspectRatio, init_near_plane, init_far_plane);
  glm::vec4 expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  glm::vec3 ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  float new_near = 0.5f;
  camera.setNearPerspectivePlane(new_near);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.perspective_projection() * testing_point;
  expected_mat = glm::perspective(glm::radians(new_fov), aspectRatio, new_near, init_far_plane);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  new_near = 0.001f;
  camera.setNearPlane(new_near);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.perspective_projection() * testing_point;
  expected_mat = glm::perspective(glm::radians(new_fov), aspectRatio, new_near, init_far_plane);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  float new_far = 200.0f;
  camera.setFarPerspectivePlane(new_far);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.perspective_projection() * testing_point;
  expected_mat = glm::perspective(glm::radians(new_fov), aspectRatio, new_near, new_far);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  new_far = 2000.0f;
  camera.setFarPlane(new_far);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.perspective_projection() * testing_point;
  expected_mat = glm::perspective(glm::radians(new_fov), aspectRatio, new_near, new_far);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  renderer.setSize(200, 300);
  aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.perspective_projection() * testing_point;
  expected_mat = glm::perspective(glm::radians(new_fov), aspectRatio, new_near, new_far);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);
};

TEST(CameraOrthogonalProjection, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 init_dir = {0.0f, 0.0f, 1.0f};
  float init_ortho_size = 60.0f;
  float init_near_plane = Engine::Config::CAMERA_ORTHO_NEAR_PLANE;
  float init_far_plane = Engine::Config::CAMERA_ORTHO_FAR_PLANE;

  
  Engine::Core::Camera camera(&renderer, init_pos, init_dir);
  camera.setCameraMode(Engine::ScriptShared::CameraMode::ORTHOGONAL);
  camera.setOrthoSize(init_ortho_size);
  camera.setNearPlane(init_near_plane);
  camera.setFarPlane(init_far_plane);
  
  float aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  float half_width = (init_ortho_size * aspectRatio) * 0.5f;
  float half_height = init_ortho_size * 0.5f;
  glm::mat4 expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height, init_near_plane, init_far_plane);
    
  glm::vec4 testing_point1 = glm::vec4(-3.0f, 2.0f, -10.0f, 1.0f); 
  glm::vec4 transformed_point1 = camera.orthogonal_projection() * testing_point1;
  glm::vec4 expected1 = expected_mat * testing_point1;
  
  EXPECT_NEAR(transformed_point1.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point1.x, expected1.x, 0.001f);
  EXPECT_NEAR(transformed_point1.y, expected1.y, 0.001f);
  EXPECT_NEAR(transformed_point1.z, expected1.z, 0.001f);

  glm::vec3 ndc1 = glm::vec3(transformed_point1) / transformed_point1.w;
  
  EXPECT_GE(ndc1.x, -1.0f);
  EXPECT_LE(ndc1.x, 1.0f);
  EXPECT_GE(ndc1.y, -1.0f);
  EXPECT_LE(ndc1.y, 1.0f);
  EXPECT_GE(ndc1.z, -1.0f);
  EXPECT_LE(ndc1.z, 1.0f);

  glm::vec4 testing_point2 = {2.0f, 12.0f, -25.0f, 1.0f}; 
  glm::vec4 transformed_point2 = camera.orthogonal_projection() * testing_point2;
  glm::vec4 expected2 = expected_mat * testing_point2;
  
  EXPECT_NEAR(transformed_point2.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point2.x, expected2.x, 0.001f);
  EXPECT_NEAR(transformed_point2.y, expected2.y, 0.001f);
  EXPECT_NEAR(transformed_point2.z, expected2.z, 0.001f);

  glm::vec3 ndc2 = glm::vec3(transformed_point2) / transformed_point2.w;
  
  EXPECT_GE(ndc2.x, -1.0f);
  EXPECT_LE(ndc2.x, 1.0f);
  EXPECT_GE(ndc2.y, -1.0f);
  EXPECT_LE(ndc2.y, 1.0f);
  EXPECT_GE(ndc2.z, -1.0f);
  EXPECT_LE(ndc2.z, 1.0f);

  float new_ortho_size = 65.0f;
  camera.setOrthoSize(new_ortho_size);
  aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  half_width = (new_ortho_size * aspectRatio) * 0.5f;
  half_height = new_ortho_size * 0.5f; 
  glm::vec4 testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  glm::vec4 transformed_point = camera.orthogonal_projection() * testing_point;
  expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height, init_near_plane, init_far_plane);
  glm::vec4 expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  glm::vec3 ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  float new_near = 0.5f;
  camera.setNearOrthogonalPlane(new_near);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.orthogonal_projection() * testing_point;
  expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height, new_near, init_far_plane);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  new_near = 0.001f;
  camera.setNearPlane(new_near);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.orthogonal_projection() * testing_point;
  expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height,  new_near, init_far_plane);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  float new_far = 200.0f;
  camera.setFarOrthogonalPlane(new_far);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.orthogonal_projection() * testing_point;
  expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height, new_near, new_far);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  new_far = 2000.0f;
  camera.setFarPlane(new_far);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.orthogonal_projection() * testing_point;
  expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height, new_near, new_far);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  renderer.setSize(200, 300);
  aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  half_width = (new_ortho_size * aspectRatio) * 0.5f;
  half_height = new_ortho_size * 0.5f;
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.orthogonal_projection() * testing_point;
  expected_mat = glm::ortho(-half_width, half_width, -half_height, half_height, new_near, new_far);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);
};

TEST(CameraProjection, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 init_dir = glm::vec3(0.0f, 0.0f, 1.0f);

  float init_ortho_size = 60.0f;
  float init_ortho_near_plane = Engine::Config::CAMERA_ORTHO_NEAR_PLANE;
  float init_ortho_far_plane = Engine::Config::CAMERA_ORTHO_FAR_PLANE;
  float init_fov = 60.0f;
  float init_perspective_near_plane = Engine::Config::CAMERA_NEAR_PLANE;
  float init_perspective_far_plane = Engine::Config::CAMERA_FAR_PLANE;

  Engine::Core::Camera camera(&renderer, init_pos, init_dir);
  camera.setOrthoSize(init_ortho_size);
  camera.setNearOrthogonalPlane(init_ortho_near_plane);
  camera.setFarOrthogonalPlane(init_ortho_far_plane);
  camera.setFov(init_fov);
  camera.setNearPerspectivePlane(init_perspective_near_plane);
  camera.setFarPerspectivePlane(init_perspective_far_plane);

  float aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  float half_width = (init_ortho_size * aspectRatio) * 0.5f;
  float half_height = init_ortho_size * 0.5f;
  glm::mat4 expected_orthogonal_mat = glm::ortho(-half_width, half_width, -half_height, half_height, init_ortho_near_plane, init_ortho_far_plane);
  glm::mat4 expected_perspective_mat = glm::perspective(glm::radians(init_fov), aspectRatio, init_perspective_near_plane, init_perspective_far_plane);
  
  camera.setCameraMode(Engine::ScriptShared::CameraMode::ORTHOGONAL);
  glm::vec4 testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  glm::vec4 transformed_point = camera.projection() * testing_point;
  glm::vec4 expected = expected_orthogonal_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 1.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  glm::vec3 ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);

  camera.setCameraMode(Engine::ScriptShared::CameraMode::PERSPECTIVE);
  testing_point = {2.0f, 12.0f, -25.0f, 1.0f}; 
  transformed_point = camera.projection() * testing_point;
  expected = expected_perspective_mat * testing_point;
  
  EXPECT_NEAR(transformed_point.w, 25.0f, 0.001f);
  EXPECT_NEAR(transformed_point.x, expected.x, 0.001f);
  EXPECT_NEAR(transformed_point.y, expected.y, 0.001f);
  EXPECT_NEAR(transformed_point.z, expected.z, 0.001f);

  ndc = glm::vec3(transformed_point) / transformed_point.w;
  
  EXPECT_GE(ndc.x, -1.0f);
  EXPECT_LE(ndc.x, 1.0f);
  EXPECT_GE(ndc.y, -1.0f);
  EXPECT_LE(ndc.y, 1.0f);
  EXPECT_GE(ndc.z, -1.0f);
  EXPECT_LE(ndc.z, 1.0f);
};

TEST(CameraView, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 init_dir = {0.0f, 0.0f, 1.0f};

  Engine::Core::Camera camera(&renderer, init_pos, init_dir);
  
  glm::vec4 testing_point = glm::vec4(2.0f, 0.0f, 1.0f, 1.0f);
  glm::quat orient = glm::quatLookAt(-init_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 expected_mat = glm::lookAt(init_pos, init_pos + init_dir, dynamicUp);
  glm::vec4 expected = expected_mat * testing_point;

  EXPECT_EQ(expected, camera.view() * testing_point);

  testing_point = glm::vec4(22.0f, -14.0f, 32.0f, 1.0f);
  orient = glm::quatLookAt(-init_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  expected_mat = glm::lookAt(init_pos, init_pos + init_dir, dynamicUp);
  expected = expected_mat * testing_point;

  EXPECT_EQ(expected, camera.view() * testing_point);

  glm::vec3 new_dir = {0.25f, 0.45f, 1.0f};
  camera.setDirection(new_dir);
  testing_point = glm::vec4(2.0f, 0.0f, 1.0f, 1.0f);
  orient = glm::quatLookAt(-new_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  expected_mat = glm::lookAt(init_pos, init_pos + new_dir, dynamicUp);
  expected = expected_mat * testing_point;

  EXPECT_NEAR(expected.x, (camera.view() * testing_point).x, 0.0001f);
  EXPECT_NEAR(expected.y, (camera.view() * testing_point).y, 0.0001f);
  EXPECT_NEAR(expected.z, (camera.view() * testing_point).z, 0.0001f);
  EXPECT_NEAR(expected.w, (camera.view() * testing_point).w, 0.0001f);

  testing_point = glm::vec4(22.0f, -14.0f, 32.0f, 1.0f);
  orient = glm::quatLookAt(-new_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  expected_mat = glm::lookAt(init_pos, init_pos + new_dir, dynamicUp);
  expected = expected_mat * testing_point;

  EXPECT_NEAR(expected.x, (camera.view() * testing_point).x, 0.0001f);
  EXPECT_NEAR(expected.y, (camera.view() * testing_point).y, 0.0001f);
  EXPECT_NEAR(expected.z, (camera.view() * testing_point).z, 0.0001f);
  EXPECT_NEAR(expected.w, (camera.view() * testing_point).w, 0.0001f);

  glm::vec3 new_pos = {20.0f, 1.45f, -25.0f};
  camera.setPosition(new_pos);
  testing_point = glm::vec4(2.0f, 0.0f, 1.0f, 1.0f);
  orient = glm::quatLookAt(-new_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  expected_mat = glm::lookAt(new_pos, new_pos + new_dir, dynamicUp);
  expected = expected_mat * testing_point;

  EXPECT_NEAR(expected.x, (camera.view() * testing_point).x, 0.0001f);
  EXPECT_NEAR(expected.y, (camera.view() * testing_point).y, 0.0001f);
  EXPECT_NEAR(expected.z, (camera.view() * testing_point).z, 0.0001f);
  EXPECT_NEAR(expected.w, (camera.view() * testing_point).w, 0.0001f);

  testing_point = glm::vec4(22.0f, -14.0f, 32.0f, 1.0f);
  orient = glm::quatLookAt(-new_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  expected_mat = glm::lookAt(new_pos, new_pos + new_dir, dynamicUp);
  expected = expected_mat * testing_point;
  
  EXPECT_NEAR(expected.x, (camera.view() * testing_point).x, 0.0001f);
  EXPECT_NEAR(expected.y, (camera.view() * testing_point).y, 0.0001f);
  EXPECT_NEAR(expected.z, (camera.view() * testing_point).z, 0.0001f);
  EXPECT_NEAR(expected.w, (camera.view() * testing_point).w, 0.0001f);

  float target_blank = 0.0f;
  camera.movementButtons(0.0f, target_blank);
  EXPECT_EQ(camera.view_mat_ready, false);
  
  camera.view();
  EXPECT_EQ(camera.view_mat_ready, true);
  
  camera.rotationButtons(0.0f, target_blank);
  EXPECT_EQ(camera.view_mat_ready, false);
};

TEST(CameraTransformation, HandlesInitialization){
  Mock::Renderer renderer;
  glm::vec3 init_pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 init_dir = {0.0f, 0.0f, 1.0f};
  float init_ortho_size = 60.0f;
  float init_fov = 60.0f;
  float init_perspective_near_plane = 0.01f; 
  float init_perspective_far_plane = 1000.0f;
  float init_orthogonal_near_plane = 0.01f; 
  float init_orthogonal_far_plane = 1000.0f;

  Engine::Core::Camera camera(&renderer, init_pos, init_dir);
  camera.setFov(init_fov);
  camera.setOrthoSize(init_ortho_size);
  camera.setNearPerspectivePlane(init_perspective_near_plane);
  camera.setFarPerspectivePlane(init_perspective_far_plane);
  camera.setNearOrthogonalPlane(init_orthogonal_near_plane);
  camera.setFarOrthogonalPlane(init_orthogonal_far_plane);

  float aspectRatio = renderer.getWindowData()->width / (float)renderer.getWindowData()->height;
  glm::vec4 testing_point = glm::vec4(2.0f, 0.0f, 1.0f, 1.0f);
  glm::quat orient = glm::quatLookAt(-init_dir, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 dynamicUp = orient * glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 expected_view_mat = glm::lookAt(init_pos, init_pos + init_dir, dynamicUp);
  glm::mat4 expected_perspective_mat = glm::perspective(glm::radians(init_fov), aspectRatio, init_perspective_near_plane, init_perspective_far_plane);
  glm::vec4 expected = expected_perspective_mat * expected_view_mat * testing_point;
  
  EXPECT_EQ(expected, camera.transformation() * testing_point);

  camera.setPosition({0.0f, 1.0f, 1.0f});
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setDirection({0.0f, 1.0f, 1.0f});
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setCameraMode(Engine::ScriptShared::CameraMode::PERSPECTIVE);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setFov(20.0f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setCameraMode(Engine::ScriptShared::CameraMode::ORTHOGONAL);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setFov(20.0f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setOrthoSize(20.0f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setNearPerspectivePlane(0.1f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setNearOrthogonalPlane(0.1f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setFarPerspectivePlane(100.0f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.setFarOrthogonalPlane(200.0f);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  float target_blank = 0.0f;
  camera.movementButtons(0.0f, target_blank);
  EXPECT_EQ(camera.transform_mat_ready, false);

  camera.transformation();
  EXPECT_EQ(camera.transform_mat_ready, true);

  camera.rotationButtons(0.0f, target_blank);
  EXPECT_EQ(camera.transform_mat_ready, false);
};



//// ========================= ////
//// ==== Setters/Getters ==== ////
//// ========================= ////
TEST(CameraPositionDirectionSettersGetters, HandlesPositionAndDirection) {
  Mock::Renderer renderer;
  Engine::Core::Camera camera(&renderer, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

  glm::vec3 newPos(10.0f, -5.0f, 3.14f);
  camera.setPosition(newPos);
  
  EXPECT_FLOAT_EQ(camera.getPosition().x, newPos.x);
  EXPECT_FLOAT_EQ(camera.getPosition().y, newPos.y);
  EXPECT_FLOAT_EQ(camera.getPosition().z, newPos.z);

  glm::vec3 newDir(1.0f, 1.0f, 0.0f);
  camera.setDirection(newDir);
  glm::vec3 expectedDir = glm::normalize(newDir);
  
  EXPECT_NEAR(camera.getDirection().x, expectedDir.x, 0.001f);
  EXPECT_NEAR(camera.getDirection().y, expectedDir.y, 0.001f);
  EXPECT_NEAR(camera.getDirection().z, expectedDir.z, 0.001f);

  camera.setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
  
  EXPECT_NEAR(camera.getDirection().x, 0.0f, 0.001f);
  EXPECT_NEAR(camera.getDirection().y, 0.0f, 0.001f);
  EXPECT_NEAR(camera.getDirection().z, 1.0f, 0.001f);
};

TEST(CameraParametersSettersGetters, HandlesProjectionPropertiesAndClamping) {
  Mock::Renderer renderer;
  Engine::Core::Camera camera(&renderer, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

  camera.setFov(90.0f);
  EXPECT_FLOAT_EQ(camera.getFov(), 90.0f);
  camera.setFov(-10.0f);
  EXPECT_FLOAT_EQ(camera.getFov(), 0.0f);

  camera.setOrthoSize(25.0f);
  EXPECT_FLOAT_EQ(camera.getOrthoSize(), 25.0f);
  camera.setOrthoSize(0.0f);
  EXPECT_FLOAT_EQ(camera.getOrthoSize(), 0.0f);

  camera.setNearPerspectivePlane(0.1f);
  EXPECT_FLOAT_EQ(camera.getNearPerspectivePlane(), 0.1f);
  camera.setNearPerspectivePlane(-1.0f);
  EXPECT_FLOAT_EQ(camera.getNearPerspectivePlane(), 0.0f);

  camera.setFarPerspectivePlane(1000.0f);
  EXPECT_FLOAT_EQ(camera.getFarPerspectivePlane(), 1000.0f);
  camera.setFarPerspectivePlane(0.0f);
  EXPECT_FLOAT_EQ(camera.getFarPerspectivePlane(), 0.0f);

  camera.setNearOrthogonalPlane(1.0f);
  EXPECT_FLOAT_EQ(camera.getNearOrthogonalPlane(), 1.0f);
  camera.setNearOrthogonalPlane(-5.0f);
  EXPECT_FLOAT_EQ(camera.getNearOrthogonalPlane(), 0.0f);

  camera.setFarOrthogonalPlane(500.0f);
  EXPECT_FLOAT_EQ(camera.getFarOrthogonalPlane(), 500.0f);
  camera.setFarOrthogonalPlane(-100.0f);
  EXPECT_FLOAT_EQ(camera.getFarOrthogonalPlane(), 0.0f);
};

TEST(CameraProjectionSettersGetters, HandlesModeSpecificPlanes) {
  Mock::Renderer renderer;
  Engine::Core::Camera camera(&renderer, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

  camera.setNearPerspectivePlane(0.1f);
  camera.setFarPerspectivePlane(100.0f);
  camera.setNearOrthogonalPlane(1.0f);
  camera.setFarOrthogonalPlane(200.0f);

  camera.setCameraMode(Engine::ScriptShared::CameraMode::PERSPECTIVE);
  EXPECT_EQ(camera.getCameraMode(), Engine::ScriptShared::CameraMode::PERSPECTIVE);
  EXPECT_FLOAT_EQ(camera.getNearPlane(), 0.1f);
  EXPECT_FLOAT_EQ(camera.getFarPlane(), 100.0f);

  camera.setNearPlane(0.5f);
  camera.setFarPlane(150.0f);
  EXPECT_FLOAT_EQ(camera.getNearPerspectivePlane(), 0.5f);
  EXPECT_FLOAT_EQ(camera.getFarPerspectivePlane(), 150.0f);
  
  EXPECT_FLOAT_EQ(camera.getNearOrthogonalPlane(), 1.0f);

  camera.setCameraMode(Engine::ScriptShared::CameraMode::ORTHOGONAL);
  EXPECT_EQ(camera.getCameraMode(), Engine::ScriptShared::CameraMode::ORTHOGONAL);
  EXPECT_FLOAT_EQ(camera.getNearPlane(), 1.0f);
  EXPECT_FLOAT_EQ(camera.getFarPlane(), 200.0f);

  camera.setNearPlane(2.0f);
  camera.setFarPlane(250.0f);
  EXPECT_FLOAT_EQ(camera.getNearOrthogonalPlane(), 2.0f);
  EXPECT_FLOAT_EQ(camera.getFarOrthogonalPlane(), 250.0f);
  
  EXPECT_FLOAT_EQ(camera.getNearPerspectivePlane(), 0.5f);
};

TEST(CameraMovementSettersGetters, HandlesMovementAndInputFlags) {
  Mock::Renderer renderer;
  Engine::Core::Camera camera(&renderer, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

  camera.setDefaultMovement(true);
  EXPECT_TRUE(camera.getDefaultMovement());
  camera.setDefaultMovement(false);
  EXPECT_FALSE(camera.getDefaultMovement());

  camera.setMouseActive(true);
  EXPECT_TRUE(camera.getMouseActive());
  camera.setMouseActive(false);
  EXPECT_FALSE(camera.getMouseActive());

  camera.setVelocity(5.5f);
  EXPECT_FLOAT_EQ(camera.getVelocity(), 5.5f);
  camera.setVelocity(-2.0f);
  EXPECT_FLOAT_EQ(camera.getVelocity(), 0.0f);

  camera.setSensitivity(0.8f);
  EXPECT_FLOAT_EQ(camera.getSensitivity(), 0.8f);
  camera.setSensitivity(0.0f);
  EXPECT_FLOAT_EQ(camera.getSensitivity(), 0.0f);
};



//// ================== ////
//// ==== Movement ==== ////
//// ================== ////
// Skiped requires MOCKS