// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Scene.h"



Engine::Core::Scene::Scene(CW::Renderer::Renderer& window)
  : window(window), camera_controller(&window), light_camera(&window), fbo(1920, 1080), post_fbo(1920, 1080), shadows_fbo(1920 * 5, 1080 * 5), screen_quad("screen_quad", &Engine::Core::Resources::get().meshes)
#ifndef PRODUCTION
  , debug_camera(&window)
#endif
{
  Engine::Utils::Logger::get().info("Scene", "Scene Initialized");
};



Engine::Core::Scene::~Scene(){
  Engine::Utils::Logger::get().info("Scene", "Scene Destroyed");
};



void Engine::Core::Scene::onLoad(){
  Engine::Utils::Logger::get().info("Scene", "Loading Scene");
  
  Engine::Utils::Logger::get().info("Scene", "Data Loaded from DataSerializer");


  post_uniform["u_water_height"]->set<float>(Engine::Config::WATER_HEIGHT);
  post_uniform["u_Near"]->set<float>(Engine::Config::CAMERA_NEAR_PLANE);
  post_uniform["u_Far"]->set<float>(Engine::Config::CAMERA_ORTHO_FAR_PLANE);
  post_uniform["u_FogDensity"]->set<float>(Engine::Config::FOG_DENSITY);
  post_uniform["u_FogColor"]->set<glm::vec3>(Engine::Config::FOG_COLOR);
  Engine::Utils::Logger::get().info("Scene", "PostProcessing Uniforms Initialized");
  

  light_camera.setCameraMode(Engine::CameraMode::PERSPECTIVE);
  light_camera.setFov(110.0f);
  last_light_camera_fov = light_camera.getFov(); 
  light_camera.setPosition(Engine::Core::Resources::get().lights[0].position);
  last_light_camera_pos = light_camera.getPosition();
  light_camera.setDirection(glm::normalize(-Engine::Core::Resources::get().lights[0].position));
  last_light_camera_dir = light_camera.getDirection();
  light_space_matrix = light_camera.transformation();
  
  shadows_uniform_off["u_ShadowEnabled"]->set<int>(0);
  shadows_uniform_off["u_ShadowDepthTexture"]->set<int>(16);
  shadows_uniform_off["u_LightSpaceMatrix"]->set<glm::mat4>(light_space_matrix);
  shadows_uniform_on["u_ShadowEnabled"]->set<int>(1);
  shadows_uniform_on["u_ShadowDepthTexture"]->set<int>(16);
  shadows_uniform_on["u_LightSpaceMatrix"]->set<glm::mat4>(light_space_matrix);
  Engine::Utils::Logger::get().info("Scene", "Shadows Camera and Uniform Initialized");
  

  camera_controller.spawnCamera(
    "Main", 
    {174.780f, 26.939f, -80.027f}, 
    {-0.847f, -0.466f, -0.256f}
  );
  camera_controller.setActiveCamera("Main");
  
  Engine::Utils::Logger::get().info("Scene", "Main Camera Initialized");


#ifndef PRODUCTION
  debug_camera.setPosition({453.198f, 250.233f, -26.842f});
  debug_camera.setDirection({-0.668f, -0.734f, -0.122f});
  debug_camera.setDefaultMovement(true);
  Engine::Utils::Logger::get().info("Scene", "Debug Camera Initialized");
#endif
    

  compileShadows();
  Engine::Utils::Logger::get().info("Scene", "Shadows Compiled");
};



void Engine::Core::Scene::onUpdate(float delta_time){
  camera_controller.getActiveCamera().event(delta_time);

  unsigned int size = Engine::ObjectManager::get().objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().objects[i].onUpdate(delta_time);
    if(size > Engine::ObjectManager::get().objects.size()){
      size = Engine::ObjectManager::get().objects.size();
      i--;
      if(size == 0) break;
    };
  };

  size = Engine::ObjectManager::get().script_objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().script_objects[i].onUpdate(delta_time);
    if(size > Engine::ObjectManager::get().script_objects.size()){
      size = Engine::ObjectManager::get().script_objects.size();
      i--;
      if(size == 0) break;
    };
  };
};



void Engine::Core::Scene::onFixedUpdate(float fixed_delta_time){
#ifndef PRODUCTION
  save_acc += fixed_delta_time;

  if(save_acc >= Engine::Config::SAVE_TIMESTAMP){
    save_acc -= Engine::Config::SAVE_TIMESTAMP;
    DataSerializer::get().saveAll();
    Engine::Utils::Logger::get().info("Scene", "Auto-Save scene data");
  };
#endif
  
  unsigned int size = Engine::ObjectManager::get().objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().objects[i].onFixedUpdate(fixed_delta_time, (*this));
    if(size > Engine::ObjectManager::get().objects.size()){
      size = Engine::ObjectManager::get().objects.size();
      i--;
      if(size == 0) break;
    };
  }; 

  size = Engine::ObjectManager::get().script_objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().script_objects[i].onFixedUpdate(fixed_delta_time, (*this));
    if(size > Engine::ObjectManager::get().script_objects.size()){
      size = Engine::ObjectManager::get().script_objects.size();
      i--;
      if(size == 0) break;
    };
  };
};



void Engine::Core::Scene::onDestroy() {
  Engine::Utils::Logger::get().info("Scene", "Destroying Scene");

  unsigned int size = Engine::ObjectManager::get().objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().objects[i].onDestroy();
    if(size > Engine::ObjectManager::get().objects.size()){
      size = Engine::ObjectManager::get().objects.size();
      i--;
      if(size == 0) break;
    };
  };

  size = Engine::ObjectManager::get().script_objects.size();
  for(int i = 0; i < size; i++){
    Engine::ObjectManager::get().script_objects[i].onDestroy();
    if(size > Engine::ObjectManager::get().script_objects.size()){
      size = Engine::ObjectManager::get().script_objects.size();
      i--;
      if(size == 0) break;
    };
  };

  Engine::Utils::Logger::get().info("Scene", "Objects onDestroy");
  
  Engine::ObjectManager::get().objects.clear();
  Engine::ObjectManager::get().script_objects.clear();
  Engine::Utils::Logger::get().info("Scene", "Objects Removed");

  Engine::Utils::Logger::get().info("Scene", "Destroyed Scene");
};



void Engine::Core::Scene::compileShadows(){
  shadows_fbo.bind();
  
  if(last_light_camera_pos != light_camera.getPosition()){
    light_camera.setFov(110.0f);
    last_light_camera_fov = light_camera.getFov();

    light_camera.setPosition(Engine::Core::Resources::get().lights[0].position);
    last_light_camera_pos = light_camera.getPosition();
    
    light_camera.setDirection(glm::normalize(-Engine::Core::Resources::get().lights[0].position));
    last_light_camera_dir = light_camera.getDirection();
    
    light_space_matrix = light_camera.transformation();
    shadows_uniform_off["u_LightSpaceMatrix"]->set<glm::mat4>(light_space_matrix);
    shadows_uniform_on["u_LightSpaceMatrix"]->set<glm::mat4>(light_space_matrix);
  };  

  window.beginFrame();

  for(Engine::GameObject& object : Engine::ObjectManager::get().objects) object.render(&window, light_camera, light_camera, shadows_uniform_off);
  for(Engine::GameObject& object : Engine::ObjectManager::get().script_objects) object.render(&window, light_camera, light_camera, shadows_uniform_off);

  shadows_fbo.unbind();
};



void Engine::Core::Scene::render(){
  Engine::Core::Resources::get().lights.bind(0);
  Engine::Core::Resources::get().materials.bind(1);


#ifndef PRODUCTION
  if(!shadows_on){
    shadows_fbo.bind();
    window.beginFrame();
    shadows_fbo.unbind();
  }
  else
#endif
    compileShadows();


  fbo.bind();

#ifndef PRODUCTION
  if(debug_camera_on)
    renderFrame(debug_camera);
  else
#endif
    renderFrame(this->camera_controller.getActiveCamera());

  fbo.unbind();


  Engine::Core::Resources::get().materials.unbind();
  Engine::Core::Resources::get().lights.unbind();


  window.beginFrame();


  // fbo.blitToScreen(window.getWindowData()->width, window.getWindowData()->height);
  // else
  // #endif
  
#ifndef PRODUCTION
  if(post_processing_on)
#endif
    postProcessing();
};



void Engine::Core::Scene::renderFrame(Engine::ICamera& camera){
  window.beginFrame();

  glActiveTexture(GL_TEXTURE16);
  glBindTexture(GL_TEXTURE_2D, shadows_fbo.getDepthTexture());

  for(Engine::GameObject& object : Engine::ObjectManager::get().script_objects) object.render(&window, this->camera_controller.getActiveCamera(), camera, shadows_uniform_on);
  for(Engine::GameObject& object : Engine::ObjectManager::get().objects) object.render(&window, this->camera_controller.getActiveCamera(), camera, shadows_uniform_on);

  
  glActiveTexture(GL_TEXTURE16);
  glBindTexture(GL_TEXTURE_2D, 0);
};



void Engine::Core::Scene::postProcessing(){
  CW::Renderer::Mesh* screen_mesh =  screen_quad.get();
  if(!screen_mesh) return;

  post_fbo.bind();

  window.beginFrame(); 

  glDisable(GL_DEPTH_TEST); 

  std::string shader_name = "PostProcessing";

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo.getColorTexture());
  post_uniform["u_SceneColorTexture"]->set<int>(0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo.getDepthTexture());
  post_uniform["u_SceneDepthTexture"]->set<int>(1);

#ifndef PRODUCTION
  if(debug_camera_on){
    glm::mat4 invViewProj = glm::inverse(debug_camera.projection() * debug_camera.view());
    post_uniform["u_InvViewProj"]->set<glm::mat4>(invViewProj);
    post_uniform["u_CamPos"]->set<glm::vec3>(debug_camera.getPosition());
  }
  else{
#endif
    glm::mat4 invViewProj = glm::inverse(camera_controller.getActiveCamera().projection() * camera_controller.getActiveCamera().view());
    post_uniform["u_InvViewProj"]->set<glm::mat4>(invViewProj);
    post_uniform["u_CamPos"]->set<glm::vec3>(camera_controller.getActiveCamera().getPosition());
#ifndef PRODUCTION
  }
#endif

  Engine::Core::Resources::get().getShader(shader_name).getUniforms().emplace_back(&post_uniform);
  Engine::Core::Resources::get().getShader(shader_name).bind();
  
  screen_mesh->render();
  
  Engine::Core::Resources::get().getShader(shader_name).unbind();
  Engine::Core::Resources::get().getShader(shader_name).getUniforms().clear();

  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);

  glEnable(GL_DEPTH_TEST); 

  post_fbo.unbind();
};
