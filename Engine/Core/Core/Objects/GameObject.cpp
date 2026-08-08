// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "GameObject.h"



void PatchScriptPointers(std::vector<Engine::Core::Script::GameObjectScriptRecord>& scripts, Engine::ScriptShared::GameObjectData* new_data_ptr) {
  for (auto& record : scripts) {
    if (record.script) {
      record.script->game_object_data = new_data_ptr;
    };
  };
};



Engine::GameObject::GameObject(const std::string& name, const std::string& mesh, const std::string& shader, const std::vector<std::string>& materials, const std::vector<std::string>& textures, const std::vector<Engine::Core::Script::GameObjectScriptRecord>& scripts, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
  : scripts(scripts), mesh(mesh, &Engine::Core::Resources::get().meshes) {
  Engine::Utils::Logger::get().info("GameObject", "GameObject Constructor Called!");
  game_object_data.name = name;
  game_object_data.mesh = mesh;
  game_object_data.shader = shader;
  game_object_data.materials = materials;
  game_object_data.textures = textures;
  game_object_data.position = position;
  game_object_data.rotation = rotation;
  game_object_data.scale = scale;
  copy_game_object_data = game_object_data;
  this->mesh.setName(copy_game_object_data.mesh);

  // onLoad();
};



Engine::GameObject::GameObject(const std::string& name, const GameObject& other){
  Engine::Utils::Logger::get().info("GameObject", "GameObject Duplicating");
  
  for(const auto& script : other.scripts) scripts.emplace_back(script.getPath());
  game_object_data = other.game_object_data;
  copy_game_object_data = other.game_object_data;
  game_object_data.name = name;
  
  copy_game_object_data = game_object_data;
  mesh = other.mesh;

  PatchScriptPointers(this->scripts, &this->copy_game_object_data);
  
  // onLoad();
};



Engine::GameObject::~GameObject(){
  onDestroy();
};



Engine::GameObject::GameObject(const GameObject& other) 
  : Object(other), uniform(other.uniform), scripts(other.scripts),
    mesh_last(other.mesh_last),
    game_object_data(other.game_object_data),
    mesh(other.mesh)
{
  copy_game_object_data = game_object_data;
  mesh.setName(copy_game_object_data.name);
  PatchScriptPointers(this->scripts, &this->copy_game_object_data);
}



Engine::GameObject& Engine::GameObject::operator=(const GameObject& other) {
  if (this == &other) return *this;
  
  Object::operator=(other);
  uniform = other.uniform;
  scripts = other.scripts;
  mesh_last = other.mesh_last;
  mesh = other.mesh;
  game_object_data = other.game_object_data;
  copy_game_object_data = game_object_data;
  mesh.setName(copy_game_object_data.name);

  PatchScriptPointers(this->scripts, &this->copy_game_object_data);
  return *this;
}



Engine::GameObject::GameObject(GameObject&& other) noexcept
  : Object(std::move(other)), uniform(std::move(other.uniform)), scripts(std::move(other.scripts)),
    mesh_last(std::move(other.mesh_last)),
    game_object_data(std::move(other.game_object_data)),
    mesh(std::move(other.mesh))
{
  copy_game_object_data = game_object_data;
  mesh.setName(copy_game_object_data.name);
  PatchScriptPointers(this->scripts, &this->copy_game_object_data);
};



Engine::GameObject& Engine::GameObject::operator=(GameObject&& other) noexcept {
  if (this == &other) return *this;

  Object::operator=(std::move(other));
  uniform = std::move(other.uniform);
  scripts = std::move(other.scripts);
  mesh_last = std::move(other.mesh_last);
  mesh = std::move(other.mesh);
  game_object_data = std::move(other.game_object_data);
  copy_game_object_data = game_object_data;
  mesh.setName(copy_game_object_data.name);

  PatchScriptPointers(this->scripts, &this->copy_game_object_data);
  return *this;
};



void Engine::GameObject::stopScript(unsigned int index){
  scripts[index].onDestroy();
  scripts[index].removeModule();
};



void Engine::GameObject::startScript(unsigned int index, Engine::Core::Scene& scene){
  scripts[index].loadModule();
  copy_game_object_data = game_object_data;
  scripts[index].onLoad(&copy_game_object_data, scene);
};



void Engine::GameObject::stopScripts(){
  for(int i = 0; i < scripts.size(); i++)
    stopScript(i);
};



void Engine::GameObject::startScripts(Engine::Core::Scene& scene){
  for(int i = 0; i < scripts.size(); i++)
    startScript(i, scene);
};



void Engine::GameObject::onLoad(Engine::Core::Scene& scene){
  for(auto& script : scripts) {
    script.loadModule();
    script.onLoad(&copy_game_object_data, scene);
  };
};



void Engine::GameObject::onDestroy(){
  for(auto& script : scripts) {
    script.onDestroy();
    script.removeModule();
  };
};



void Engine::GameObject::onUpdate(float delta_time){
  if(Engine::Core::Resources::get().simulation_mode){
    for(auto& script : scripts) {
      if(!script.script_on) continue;
      script.syncPointer(&copy_game_object_data);
      script.onUpdate(delta_time);
    };
  };
};



void Engine::GameObject::onFixedUpdate(float fixed_delta_time, Engine::Core::Scene& scene){
  if(Engine::Core::Resources::get().simulation_mode){
    for(auto& script : scripts) {
      if(!script.script_on) continue;
      script.syncPointer(&copy_game_object_data);
      script.observe(&copy_game_object_data, scene);
      script.onFixedUpdate(fixed_delta_time);
    };
  };
};



void Engine::GameObject::render(CW::Renderer::Renderer *renderer, ICamera &culling_camera, ICamera &render_camera, CW::Renderer::Uniform& shadows_uniform){
  if(copy_game_object_data.mesh == "empty") return;

  if(scripts.size() == 0) copy_game_object_data = game_object_data;
  
  if(mesh_last != copy_game_object_data.mesh){
    mesh.setName(copy_game_object_data.mesh);
    mesh_last = copy_game_object_data.mesh;
  };

  CW::Renderer::Mesh* mesh = this->mesh.get();
  if(!mesh) return;

  uniform["projection"]->set<glm::mat4>(render_camera.projection());
  uniform["view"]->set<glm::mat4>(render_camera.view());
  
  uniform["cameraPosition"]->set<glm::vec3>(culling_camera.getPosition());
  uniform["lightCount"]->set<int>(Engine::Core::Resources::get().lights.size());
  uniform["window_size"]->set<glm::vec2>({renderer->getWindowData()->width, renderer->getWindowData()->height});

  glm::vec3 pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), copy_game_object_data.position);
  glm::mat4 rotationMat = glm::eulerAngleXYZ(copy_game_object_data.rotation.x, copy_game_object_data.rotation.y, copy_game_object_data.rotation.z);
  glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), this->copy_game_object_data.scale);
  glm::mat4 preRotate = glm::translate(glm::mat4(1.0f), -pivotOffset);
  glm::mat4 postRotate = glm::translate(glm::mat4(1.0f), pivotOffset);
  glm::mat4 model = translationMat * postRotate * rotationMat * preRotate * scaleMat;

  for(auto& el : copy_game_object_data.uniforms) {
    std::visit([&](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;

      if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3>) uniform[el.first]->set<T>(arg);
    }, el.second);
  };

  if(Engine::Core::Resources::get().simulation_mode){
    for(auto& script : scripts) {
      if(!script.script_on) continue;
      script.onRender();
    };
  };

  if(!copy_game_object_data.culling_on || isVisible(culling_camera.transformation(), model, *mesh)){
    if(copy_game_object_data.gl_depth_lequal)
      glDepthFunc(GL_LEQUAL);
    if(copy_game_object_data.dont_write_to_depth_mask)
      glDepthMask(GL_FALSE);
    if(copy_game_object_data.gl_draw_patches)
      glPatchParameteri(GL_PATCH_VERTICES, 4);
    if(copy_game_object_data.gl_blend){
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };

    uniform["model"]->set<glm::mat4>(model);

    for(unsigned int i = 0; i < copy_game_object_data.textures.size(); i++){
      Engine::Core::Resources::get().getTexture(this->copy_game_object_data.textures[i]).bind(i);
      uniform["texture" + std::to_string(i)]->set<int>(i);
    };
    
    Engine::Core::Resources::get().getShader(this->copy_game_object_data.shader).getUniforms().emplace_back(&shadows_uniform);
    Engine::Core::Resources::get().getShader(this->copy_game_object_data.shader).getUniforms().emplace_back(&uniform);
    
    Engine::Core::Resources::get().getShader(this->copy_game_object_data.shader).bind();
    
    std::vector<int> translation;
    for(std::string el : copy_game_object_data.materials){
      translation.emplace_back(Engine::Core::Resources::get().materials.translate_material(el));
    };

    GLint loc = glGetUniformLocation(Engine::Core::Resources::get().getShader(copy_game_object_data.shader).getShaderProgram(), "mat_translate");
    glUniform1iv(loc, translation.size(), translation.data());
    
    if(this->copy_game_object_data.gl_nearest){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else{
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    };

    if(copy_game_object_data.gl_draw_patches)
      mesh->render(GL_PATCHES);
    else
      mesh->render();

    if(this->copy_game_object_data.gl_nearest){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    };
    
    Engine::Core::Resources::get().getShader(this->copy_game_object_data.shader).unbind();

    for(unsigned int i = 0; i < copy_game_object_data.textures.size(); i++) 
      Engine::Core::Resources::get().getTexture(this->copy_game_object_data.textures[i]).unbind();

    Engine::Core::Resources::get().getShader(this->copy_game_object_data.shader).getUniforms().clear();
    
    if(copy_game_object_data.gl_depth_lequal)
      glDepthFunc(GL_LESS);
    if(copy_game_object_data.dont_write_to_depth_mask)
      glDepthMask(GL_TRUE);
    if(copy_game_object_data.gl_blend)
      glDisable(GL_BLEND);
  };
};



bool Engine::GameObject::isVisible(glm::mat4 culling_camera_transform, glm::mat4 model, const CW::Renderer::Mesh& mesh){
  auto cullingBox = mesh.getCullingBox();
  glm::vec3 localMin = glm::vec3(cullingBox[0][0], cullingBox[0][1], cullingBox[0][2]); 
  glm::vec3 localMax = glm::vec3(cullingBox[1][0], cullingBox[1][1], cullingBox[1][2]);

  glm::vec3 corners[8] = {
    glm::vec3(model * glm::vec4(localMin.x, localMin.y, localMin.z, 1.0f)),
    glm::vec3(model * glm::vec4(localMax.x, localMin.y, localMin.z, 1.0f)),
    glm::vec3(model * glm::vec4(localMin.x, localMax.y, localMin.z, 1.0f)),
    glm::vec3(model * glm::vec4(localMax.x, localMax.y, localMin.z, 1.0f)),

    glm::vec3(model * glm::vec4(localMin.x, localMin.y, localMax.z, 1.0f)),
    glm::vec3(model * glm::vec4(localMax.x, localMin.y, localMax.z, 1.0f)),
    glm::vec3(model * glm::vec4(localMin.x, localMax.y, localMax.z, 1.0f)),
    glm::vec3(model * glm::vec4(localMax.x, localMax.y, localMax.z, 1.0f))
  };

  glm::vec3 aabbMin = corners[0];
  glm::vec3 aabbMax = corners[0];

  for (int i = 1; i < 8; i++){
    aabbMin = glm::min(aabbMin, corners[i]);
    aabbMax = glm::max(aabbMax, corners[i]);
  };

  glm::mat4 m = culling_camera_transform;
  glm::vec4 planes[6];

  // Left
  planes[0] = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]);
  // Right
  planes[1] = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]);
  // Bottom
  planes[2] = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]);
  // Top
  planes[3] = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]);
  // Near
  planes[4] = glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]);
  // Far
  planes[5] = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]);

  for (int i = 0; i < 6; i++){
    float length = glm::length(glm::vec3(planes[i]));
    if (length > 0.0f) planes[i] /= length;
  };

  for (int i = 0; i < 6; i++){
    glm::vec3 normal = glm::vec3(planes[i]);
    glm::vec3 positiveVertex;

    positiveVertex.x = (normal.x >= 0.0f) ? aabbMax.x : aabbMin.x;
    positiveVertex.y = (normal.y >= 0.0f) ? aabbMax.y : aabbMin.y;
    positiveVertex.z = (normal.z >= 0.0f) ? aabbMax.z : aabbMin.z;

    float distance = glm::dot(normal, positiveVertex) + planes[i].w;

    if (distance < 0.0f) return false;
  };

  return true;
};
