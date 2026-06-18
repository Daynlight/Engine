#include "SDF.h"



UW::SDFObject::SDFObject(int type, glm::vec3 position, float size)
  :type(type), position(position), size(size){};



UW::SDF::SDF(){
  emplace_back({0, {150.0f, 25.0f, -100.0f}, 15.0f});
};



UW::SDF::~SDF(){
  destroy();
};



void UW::SDF::render(CW::Renderer::Framebuffer& fbo, UW::Camera& camera, CW::Renderer::Renderer& window){
  if(!is_compiled) compile();
  
  fbo.bind();

  // glDisable(GL_DEPTH_TEST);
  std::string shader_name = "SDF";

  CW::Renderer::Uniform sdf_uniform; 
  unsigned int quad_mesh_id = Resources::get().meshes.get_id("screen_quad");

  // No camera uniforms needed anymore!
  sdf_uniform["transformation"]->set<glm::mat4>(camera.transformation(&window));
  Resources::get().getShader(shader_name).getUniforms().emplace_back(&sdf_uniform);
  Resources::get().getShader(shader_name).bind();
  
  this->bind(3);
  Resources::get().meshes[quad_mesh_id].render();
  this->unbind();
  
  Resources::get().getShader(shader_name).unbind();
  Resources::get().getShader(shader_name).getUniforms().clear();

  fbo.unbind();
};



void UW::SDF::compile(){
  buffer.create();
  buffer.set<SDFObject>(data);
  is_compiled = true;
};



void UW::SDF::destroy(){
  buffer.destroy();
  is_compiled = false;
};



void UW::SDF::bind(GLuint socket){
  if(!is_compiled) compile();

  buffer.bind(socket);
};



void UW::SDF::unbind(){
  if(!is_compiled) return;

  buffer.unbind();
};



UW::SDFObject& UW::SDF::operator[](unsigned int index){
  is_compiled = false;
  return data[index];
};



UW::SDFObject UW::SDF::get(unsigned int index) const{
  return data[index];
};



void UW::SDF::clear(){
  is_compiled = false;
  data.clear();
};



void UW::SDF::erase(unsigned int index){
  is_compiled = false;
  data.erase(data.begin() + index);
};



unsigned int UW::SDF::size() const {
  return data.size();
};



void UW::SDF::emplace_back(SDFObject sdf){
  is_compiled = false;
  data.emplace_back(sdf);
};



void UW::SDF::emplace_back(std::initializer_list<SDFObject> sdfs){
  is_compiled = false;
  for (SDFObject el : sdfs) this->data.emplace_back(el);
};
