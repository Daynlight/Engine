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
#include "Core/Camera/CameraController.h"

#undef private
#undef protected



//// ======================= /////
//// ======== Mocks ======== /////
//// ======================= /////
namespace Mock{

class Renderer : public CW::Renderer::Renderer {
public:
  Renderer() {};
};
};



//// ==================== ////
//// === Constructors === ////
//// ==================== ////
TEST(CameraControllerDefaultConstructors, HandlesInitialization) {
  Engine::Core::CameraController controller;

  EXPECT_EQ(controller.renderer, nullptr);
  EXPECT_EQ(controller.active_camera_ref, nullptr);
  EXPECT_TRUE(controller.cameras.empty());
  EXPECT_EQ(controller.active_camera, "");
  EXPECT_FALSE(controller.active_camera_setted);
};

TEST(CameraControllerParamConstructors, HandlesInitialization) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  EXPECT_EQ(controller.renderer, &renderer);
  EXPECT_EQ(controller.active_camera_ref, nullptr);
  EXPECT_TRUE(controller.cameras.empty());
  EXPECT_EQ(controller.active_camera, "");
  EXPECT_FALSE(controller.active_camera_setted);
};

TEST(CameraControllerCopyConstructor, HandlesInitialization){
  Mock::Renderer renderer;
  Engine::Core::CameraController init_controller(&renderer);
  init_controller.spawnCamera("MainCamera", {0.0f, 1.0f, 5.0f}, {0.0f, 0.0f, -1.0f});
  init_controller.setActiveCamera("MainCamera");

  // Copy Constructor
  Engine::Core::CameraController construct_copy_controller(init_controller);

  EXPECT_EQ(construct_copy_controller.renderer, init_controller.renderer);
  EXPECT_EQ(construct_copy_controller.cameras.size(), init_controller.cameras.size());
  EXPECT_TRUE(construct_copy_controller.cameraExists("MainCamera"));
  EXPECT_EQ(construct_copy_controller.active_camera, init_controller.active_camera);
  EXPECT_EQ(construct_copy_controller.active_camera_setted, init_controller.active_camera_setted);
  EXPECT_EQ(construct_copy_controller.active_camera_ref, init_controller.active_camera_ref);

  // Copy Assignment
  Engine::Core::CameraController construct_copy_assign_controller;
  construct_copy_assign_controller = init_controller;

  EXPECT_EQ(construct_copy_assign_controller.renderer, init_controller.renderer);
  EXPECT_EQ(construct_copy_assign_controller.cameras.size(), init_controller.cameras.size());
  EXPECT_TRUE(construct_copy_assign_controller.cameraExists("MainCamera"));
  EXPECT_EQ(construct_copy_assign_controller.active_camera, init_controller.active_camera);
  EXPECT_EQ(construct_copy_assign_controller.active_camera_setted, init_controller.active_camera_setted);
  EXPECT_EQ(construct_copy_assign_controller.active_camera_ref, init_controller.active_camera_ref);

  // Self Assignment
  Engine::Core::CameraController construct_copy_self_controller(init_controller);
  Engine::Core::CameraController* org_controller_ptr = &construct_copy_self_controller;
  construct_copy_self_controller = construct_copy_self_controller;
  Engine::Core::CameraController* new_controller_ptr = &construct_copy_self_controller;

  EXPECT_EQ(org_controller_ptr, new_controller_ptr);
  EXPECT_EQ(construct_copy_self_controller.renderer, init_controller.renderer);
  EXPECT_EQ(construct_copy_self_controller.cameras.size(), init_controller.cameras.size());
  EXPECT_TRUE(construct_copy_self_controller.cameraExists("MainCamera"));
  EXPECT_EQ(construct_copy_self_controller.active_camera, init_controller.active_camera);
  EXPECT_EQ(construct_copy_self_controller.active_camera_setted, init_controller.active_camera_setted);
  EXPECT_EQ(construct_copy_self_controller.active_camera_ref, init_controller.active_camera_ref);
  };

  TEST(CameraControllerMoveConstructor, HandlesInitialization){
  Mock::Renderer renderer;
  Engine::Core::CameraController init_org_controller(&renderer);
  init_org_controller.spawnCamera("MainCamera", {0.0f, 1.0f, 5.0f}, {0.0f, 0.0f, -1.0f});
  init_org_controller.setActiveCamera("MainCamera");

  Engine::Core::Camera* expected_ref = init_org_controller.active_camera_ref;

  // Move Constructor
  Engine::Core::CameraController init_constructor_move_controller(init_org_controller);
  Engine::Core::CameraController construct_move_controller(std::move(init_constructor_move_controller));

  EXPECT_EQ(construct_move_controller.renderer, &renderer);
  EXPECT_EQ(construct_move_controller.cameras.size(), 1);
  EXPECT_TRUE(construct_move_controller.cameraExists("MainCamera"));
  EXPECT_EQ(construct_move_controller.active_camera, "MainCamera");
  EXPECT_TRUE(construct_move_controller.active_camera_setted);
  EXPECT_EQ(construct_move_controller.active_camera_ref, expected_ref);

  // Move Assignment
  Engine::Core::CameraController init_construct_move_assign_controller(init_org_controller);
  Engine::Core::CameraController construct_move_assign_controller;
  construct_move_assign_controller = std::move(init_construct_move_assign_controller);

  EXPECT_EQ(construct_move_assign_controller.renderer, &renderer);
  EXPECT_EQ(construct_move_assign_controller.cameras.size(), 1);
  EXPECT_TRUE(construct_move_assign_controller.cameraExists("MainCamera"));
  EXPECT_EQ(construct_move_assign_controller.active_camera, "MainCamera");
  EXPECT_TRUE(construct_move_assign_controller.active_camera_setted);
  EXPECT_EQ(construct_move_assign_controller.active_camera_ref, expected_ref);

  // Self Move Assignment
  Engine::Core::CameraController init_construct_move_self_controller(init_org_controller);
  Engine::Core::CameraController construct_move_self_controller(std::move(init_construct_move_self_controller));
  Engine::Core::CameraController* org_controller_ptr = &construct_move_self_controller;
  construct_move_self_controller = std::move(construct_move_self_controller);
  Engine::Core::CameraController* new_controller_ptr = &construct_move_self_controller;

  EXPECT_EQ(org_controller_ptr, new_controller_ptr);
  EXPECT_EQ(construct_move_self_controller.renderer, &renderer);
  EXPECT_EQ(construct_move_self_controller.cameras.size(), 1);
  EXPECT_TRUE(construct_move_self_controller.cameraExists("MainCamera"));
  EXPECT_EQ(construct_move_self_controller.active_camera, "MainCamera");
  EXPECT_TRUE(construct_move_self_controller.active_camera_setted);
  EXPECT_EQ(construct_move_self_controller.active_camera_ref, expected_ref);
};



//// ================= ////
//// ==== Control ==== ////
//// ================= ////
TEST(CameraControllerSpawnWithParams, SpawnCameraAndCheckExists) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  glm::vec3 pos = {1.0f, 2.0f, 3.0f};
  glm::vec3 dir = {0.0f, -1.0f, 0.0f};

  EXPECT_FALSE(controller.cameraExists("PlayerCamera"));
  controller.spawnCamera("PlayerCamera", pos, dir);
  EXPECT_TRUE(controller.cameraExists("PlayerCamera"));

  auto& cam = controller.getCoreCamera("PlayerCamera");
  EXPECT_NEAR(cam.getPosition().x, pos.x, 0.0001f);
  EXPECT_NEAR(cam.getPosition().y, pos.y, 0.0001f);
  EXPECT_NEAR(cam.getPosition().z, pos.z, 0.0001f);
  EXPECT_NEAR(cam.getDirection().x, dir.x, 0.0001f);
  EXPECT_NEAR(cam.getDirection().y, dir.y, 0.0001f);
  EXPECT_NEAR(cam.getDirection().z, dir.z, 0.0001f);
};

TEST(CameraControllerSpawnWithPosition, SpawnCameraIgnoresDuplicates) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  controller.spawnCamera("Cam1", {1.0f, 1.0f, 1.0f});
  EXPECT_EQ(controller.cameras.size(), 1);

  controller.spawnCamera("Cam1", {9.0f, 9.0f, 9.0f});
  EXPECT_EQ(controller.cameras.size(), 1);

  auto& cam = controller.getCoreCamera("Cam1");
  EXPECT_NEAR(cam.getPosition().x, 1.0f, 0.0001f);
};

TEST(CameraControllerSpawnCamera, DeleteCamera) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  controller.spawnCamera("Cam1");
  controller.spawnCamera("Cam2");
  EXPECT_TRUE(controller.cameraExists("Cam1"));
  EXPECT_TRUE(controller.cameraExists("Cam2"));

  controller.deleteCamera("Cam1");
  EXPECT_FALSE(controller.cameraExists("Cam1"));
  EXPECT_TRUE(controller.cameraExists("Cam2"));

  EXPECT_NO_THROW(controller.deleteCamera("NonExistentCam"));
};

TEST(CameraControllerActiveCameraState, ActiveCameraManagement) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  controller.spawnCamera("Cam1");
  controller.spawnCamera("Cam2");

  controller.setActiveCamera("Cam1");
  EXPECT_EQ(controller.getActiveCameraName(), "Cam1");
  EXPECT_TRUE(controller.active_camera_setted);
  EXPECT_EQ(controller.active_camera_ref, &controller.getCoreCamera("Cam1"));

  EXPECT_NO_THROW({
    Engine::ScriptShared::ICamera& icam = controller.getActiveCamera();
    Engine::Core::Camera& ccam = controller.getCoreActiveCamera();
  });

  controller.setActiveCamera("Cam2");
  EXPECT_EQ(controller.getActiveCameraName(), "Cam2");
  EXPECT_EQ(controller.active_camera_ref, &controller.getCoreCamera("Cam2"));

  controller.setActiveCamera("NonExistentCam");
  EXPECT_EQ(controller.getActiveCameraName(), "Cam2");
};

TEST(CameraControllerActiveCamera, DeleteActiveCameraResetsState) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  controller.spawnCamera("Cam1");
  controller.setActiveCamera("Cam1");

  EXPECT_EQ(controller.getActiveCameraName(), "Cam1");
  EXPECT_TRUE(controller.active_camera_setted);

  controller.deleteCamera("Cam1");

  EXPECT_EQ(controller.getActiveCameraName(), "");
  EXPECT_FALSE(controller.active_camera_setted);
  
  EXPECT_THROW(controller.getActiveCamera(), std::runtime_error);
  EXPECT_THROW(controller.getCoreActiveCamera(), std::runtime_error);
};

TEST(CameraControllerRuntimeErrors, GetActiveCameraThrowsIfNoneSetted) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  EXPECT_THROW(controller.getActiveCamera(), std::runtime_error);
  EXPECT_THROW(controller.getCoreActiveCamera(), std::runtime_error);
};

TEST(CameraControllerMultipleCameras, SpawnCameraUpdatesActiveCameraRef) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  controller.spawnCamera("Cam1");
  controller.setActiveCamera("Cam1");

  Engine::Core::Camera* initial_ref = &controller.getCoreCamera("Cam1");

  for (int i = 0; i < 50; ++i) {
    controller.spawnCamera("ExtraCam" + std::to_string(i));
  };

  EXPECT_EQ(controller.getActiveCameraName(), "Cam1");
  EXPECT_TRUE(controller.active_camera_setted);
  EXPECT_EQ(controller.active_camera_ref, &controller.getCoreCamera("Cam1"));
  EXPECT_NO_THROW(controller.getActiveCamera());
};

TEST(CameraControllerEditingCamera, EditCamerasAndVerifyActive) {
  Mock::Renderer renderer;
  Engine::Core::CameraController controller(&renderer);

  controller.spawnCamera("FirstCamera");
  Engine::Core::Camera& cam1 = controller.getCoreCamera("FirstCamera");
  
  cam1.position = {10.0f, 20.0f, 30.0f};
  cam1.direction = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
  cam1.fov = 90.0f;
  cam1.velocity = 15.0f;

  controller.spawnCamera("SecondCamera");
  Engine::Core::Camera& cam2 = controller.getCoreCamera("SecondCamera");

  cam2.position = {-5.0f, -15.0f, 25.0f};
  cam2.direction = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f));
  cam2.fov = 60.0f;
  cam2.velocity = 5.0f;

  Engine::Core::Camera& verify_cam1 = controller.getCoreCamera("FirstCamera");
  EXPECT_NEAR(verify_cam1.position.x, 10.0f, 0.0001f);
  EXPECT_NEAR(verify_cam1.position.y, 20.0f, 0.0001f);
  EXPECT_NEAR(verify_cam1.position.z, 30.0f, 0.0001f);
  EXPECT_NEAR(verify_cam1.fov, 90.0f, 0.0001f);

  controller.setActiveCamera("SecondCamera");
  EXPECT_EQ(controller.getActiveCameraName(), "SecondCamera");
  EXPECT_TRUE(controller.active_camera_setted);

  Engine::Core::Camera& active_cam = controller.getCoreActiveCamera();
  
  EXPECT_NEAR(active_cam.position.x, -5.0f, 0.0001f);
  EXPECT_NEAR(active_cam.position.y, -15.0f, 0.0001f);
  EXPECT_NEAR(active_cam.position.z, 25.0f, 0.0001f);
  
  EXPECT_NEAR(active_cam.direction.x, cam2.direction.x, 0.0001f);
  EXPECT_NEAR(active_cam.direction.y, cam2.direction.y, 0.0001f);
  EXPECT_NEAR(active_cam.direction.z, cam2.direction.z, 0.0001f);
  
  EXPECT_NEAR(active_cam.fov, 60.0f, 0.0001f);
  EXPECT_NEAR(active_cam.velocity, 5.0f, 0.0001f);
};