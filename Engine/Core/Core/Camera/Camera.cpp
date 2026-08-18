// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Camera.h"
#include "Objects/GameObject.h"



//// ====================== ////
//// ==== Constructors ==== ////
//// ====================== ////
//// core
Engine::Core::Camera::Camera() noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::Camera()", "renderer is nullptr");
    return;
  };

  fbo = CW::Renderer::Framebuffer(fbo_size.x, fbo_size.y);
};



Engine::Core::Camera::~Camera() noexcept {};



Engine::Core::Camera::Camera(CW::Renderer::Renderer* renderer, glm::vec3 position, glm::vec3 direction) noexcept
  : renderer(renderer) {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::Camera(CW::Renderer::Renderer* renderer, glm::vec3 position, glm::vec3 direction)", "renderer is nullptr");
  };

  setPosition(position);
  setDirection(direction);
};



//// copy
Engine::Core::Camera::Camera(const Camera &second) noexcept
  :renderer(second.renderer),
   position(second.position),
   direction(second.direction),
   orientation(second.orientation),
   mode(second.mode),
   fov(second.fov),
   ortho_size(second.ortho_size),
   transform_mat_ready(second.transform_mat_ready),
   transform_mat(second.transform_mat),
   view_mat_ready(second.view_mat_ready),
   view_mat(second.view_mat),
   last_aspect_ratio_orthogonal(second.last_aspect_ratio_orthogonal),
   last_aspect_ratio_perspective(second.last_aspect_ratio_perspective),
   perspective_near_plane(second.perspective_near_plane),
   orthogonal_near_plane(second.orthogonal_near_plane),
   perspective_far_plane(second.perspective_far_plane),
   orthogonal_far_plane(second.orthogonal_far_plane),
   perspective_mat_ready(second.perspective_mat_ready),
   orthogonal_mat_ready(second.orthogonal_mat_ready),
   perspective_mat(second.perspective_mat),
   orthogonal_mat(second.orthogonal_mat),
   fbo(second.fbo),
   fbo_size(second.fbo_size),
   track_window_size(second.track_window_size),
   fbo_size_ready(second.fbo_size_ready),
   default_movemement_on(second.default_movemement_on),
   sensitivity(second.sensitivity),
   velocity(second.velocity),
   lastMouseX(second.lastMouseX),
   lastMouseY(second.lastMouseY),
   mouse_is_active(false),
   cursor_visible_lock(second.cursor_visible_lock),
   cursor_lock(second.cursor_lock) 
{
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::Camera(const Camera &second)", "renderer is nullptr");
  };
};



Engine::Core::Camera &Engine::Core::Camera::operator=(const Camera &second) noexcept {
  if (!second.renderer || !second.renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::operator=(const Camera &second)", "renderer is nullptr");
  };

  if (this == &second) return *this;

  renderer = second.renderer;
  position = second.position;
  direction = second.direction;
  mode = second.mode;
  fov = second.fov;
  ortho_size = second.ortho_size;
  transform_mat_ready = second.transform_mat_ready;
  transform_mat = second.transform_mat;
  view_mat_ready = second.view_mat_ready;
  view_mat = second.view_mat;
  last_aspect_ratio_orthogonal = second.last_aspect_ratio_orthogonal;
  last_aspect_ratio_perspective = second.last_aspect_ratio_perspective;
  perspective_near_plane = second.perspective_near_plane;
  orthogonal_near_plane = second.orthogonal_near_plane;
  perspective_far_plane = second.perspective_far_plane;
  orthogonal_far_plane = second.orthogonal_far_plane;
  fbo = second.fbo;
  fbo_size = second.fbo_size;
  track_window_size = second.track_window_size;
  fbo_size_ready = second.fbo_size_ready;  
  perspective_mat_ready = second.perspective_mat_ready;
  orthogonal_mat_ready = second.orthogonal_mat_ready;
  perspective_mat = second.perspective_mat;
  orthogonal_mat = second.orthogonal_mat;
  default_movemement_on = second.default_movemement_on;
  sensitivity = second.sensitivity;
  velocity = second.velocity;
  lastMouseX = second.lastMouseX;
  lastMouseY = second.lastMouseY;
  mouse_is_active = false;
  cursor_visible_lock = second.cursor_visible_lock;
  cursor_lock = second.cursor_lock; 

  return *this;
};



//// move
Engine::Core::Camera::Camera(Camera &&second) noexcept   
  :renderer(std::move(second.renderer)),
   position(std::move(second.position)),
   direction(std::move(second.direction)),
   orientation(std::move(second.orientation)),
   mode(std::move(second.mode)),
   fov(std::move(second.fov)),
   ortho_size(std::move(second.ortho_size)),
   transform_mat_ready(std::move(second.transform_mat_ready)),
   transform_mat(std::move(second.transform_mat)),
   view_mat_ready(std::move(second.view_mat_ready)),
   view_mat(std::move(second.view_mat)),
   last_aspect_ratio_orthogonal(std::move(second.last_aspect_ratio_orthogonal)),
   last_aspect_ratio_perspective(std::move(second.last_aspect_ratio_perspective)),
   perspective_near_plane(std::move(second.perspective_near_plane)),
   orthogonal_near_plane(std::move(second.orthogonal_near_plane)),
   perspective_far_plane(std::move(second.perspective_far_plane)),
   orthogonal_far_plane(std::move(second.orthogonal_far_plane)),
   perspective_mat_ready(std::move(second.perspective_mat_ready)),
   orthogonal_mat_ready(std::move(second.orthogonal_mat_ready)),
   perspective_mat(std::move(second.perspective_mat)),
   orthogonal_mat(std::move(second.orthogonal_mat)),
   fbo(std::move(second.fbo)),
   fbo_size(std::move(second.fbo_size)),
   track_window_size(std::move(second.track_window_size)),
   fbo_size_ready(std::move(second.fbo_size_ready)),
   default_movemement_on(std::move(second.default_movemement_on)),
   sensitivity(std::move(second.sensitivity)),
   velocity(std::move(second.velocity)),
   lastMouseX(std::move(second.lastMouseX)),
   lastMouseY(std::move(second.lastMouseY)),
   mouse_is_active(false),
   cursor_visible_lock(std::move(second.cursor_visible_lock)),
   cursor_lock(std::move(second.cursor_lock)) 
{
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::Camera(Camera &&second)", "renderer is nullptr");
  };
};



Engine::Core::Camera& Engine::Core::Camera::operator=(Camera &&second) noexcept {
  if (!second.renderer || !second.renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::operator=(Camera &&second)", "renderer is nullptr");
  };

  if (this == &second) return *this;

  renderer = std::move(second.renderer);
  position = std::move(second.position);
  direction = std::move(second.direction);
  mode = std::move(second.mode);
  fov = std::move(second.fov);
  ortho_size = std::move(second.ortho_size);
  transform_mat_ready = std::move(second.transform_mat_ready);
  transform_mat = std::move(second.transform_mat);
  view_mat_ready = std::move(second.view_mat_ready);
  view_mat = std::move(second.view_mat);
  last_aspect_ratio_orthogonal = std::move(second.last_aspect_ratio_orthogonal);
  last_aspect_ratio_perspective = std::move(second.last_aspect_ratio_perspective);
  perspective_near_plane = std::move(second.perspective_near_plane);
  orthogonal_near_plane = std::move(second.orthogonal_near_plane);
  perspective_far_plane = std::move(second.perspective_far_plane);
  orthogonal_far_plane = std::move(second.orthogonal_far_plane);
  perspective_mat_ready = std::move(second.perspective_mat_ready);
  orthogonal_mat_ready = std::move(second.orthogonal_mat_ready);
  perspective_mat = std::move(second.perspective_mat);
  orthogonal_mat = std::move(second.orthogonal_mat);
  fbo = std::move(second.fbo);
  fbo_size = std::move(second.fbo_size);
  track_window_size = std::move(second.track_window_size);
  fbo_size_ready = std::move(second.fbo_size_ready);
  default_movemement_on = std::move(second.default_movemement_on);
  sensitivity = std::move(second.sensitivity);
  velocity = std::move(second.velocity);
  lastMouseX = std::move(second.lastMouseX);
  lastMouseY = std::move(second.lastMouseY);
  mouse_is_active = false;
  cursor_visible_lock = std::move(second.cursor_visible_lock);
  cursor_lock = std::move(second.cursor_lock); 

  return *this;
};



//// ==================== ////
//// ==== Projection ==== ////
//// ==================== ////
glm::mat4 Engine::Core::Camera::transformation() noexcept {
  view();
  projection();

  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::transformation()", "renderer is nullptr {returning mat4(1.0f)}");
    if(transform_mat_ready) return transform_mat;
    return glm::mat4(1.0f); 
  };
  

  if(!transform_mat_ready){
    if(mode == Engine::ScriptShared::CameraMode::ORTHOGONAL) 
      transform_mat = orthogonal_mat * view_mat;
    else 
      transform_mat = perspective_mat * view_mat;
    transform_mat_ready = true;
  };
  
  return transform_mat;
};



glm::mat4 Engine::Core::Camera::view() noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::view()", "renderer is nullptr {returning mat4(1.0f)}");
    if(view_mat_ready) return view_mat; 
    return glm::mat4(1.0f); 
  };

  if(!view_mat_ready){
    glm::vec3 dynamicUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);
    view_mat = glm::lookAt(position, position + direction, dynamicUp);
    view_mat_ready = true;
    transform_mat_ready = false;
  };

  return view_mat;
};



glm::mat4 Engine::Core::Camera::projection() noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::projection()", "renderer is nullptr {returning mat4(1.0f)}");
    return glm::mat4(1.0f); 
  };

  if (mode == Engine::ScriptShared::CameraMode::ORTHOGONAL) {
    return orthogonal_projection();
  } else {
    return perspective_projection();
  };
};



glm::mat4 Engine::Core::Camera::perspective_projection() noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::perspective_projection()", "renderer is nullptr {returning mat4(1.0f)}");
    if(perspective_mat_ready) return perspective_mat; 
    return glm::mat4(1.0f); 
  };

  float aspect_ratio = renderer->getWindowData()->width / (float)renderer->getWindowData()->height;
  if(!perspective_mat_ready || std::abs(last_aspect_ratio_perspective - aspect_ratio) > Engine::Config::EPS){
    last_aspect_ratio_perspective = aspect_ratio;
    perspective_mat = glm::perspective(glm::radians(fov), aspect_ratio, perspective_near_plane, perspective_far_plane);
    perspective_mat_ready = true;
    transform_mat_ready = false;
  };
  
  return perspective_mat;
};



glm::mat4 Engine::Core::Camera::orthogonal_projection() noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::orthogonal_projection()", "renderer is nullptr {returning mat4(1.0f)}");
    if(orthogonal_mat_ready) return orthogonal_mat;
    return glm::mat4(1.0f); 
  };

  float aspect_ratio = renderer->getWindowData()->width / (float)renderer->getWindowData()->height; 
  if(!orthogonal_mat_ready || std::abs(last_aspect_ratio_orthogonal - aspect_ratio) > Engine::Config::EPS){
    last_aspect_ratio_orthogonal = aspect_ratio;
    float half_width = (ortho_size * aspect_ratio) * 0.5f;
    float half_height = ortho_size * 0.5f;
    orthogonal_mat = glm::ortho(-half_width, half_width, -half_height, half_height, orthogonal_near_plane, orthogonal_far_plane);  
    orthogonal_mat_ready = true;
    transform_mat_ready = false;
  };

  return orthogonal_mat;
};



//// ========================= ////
//// ==== Setters/Getters ==== ////
//// ========================= ////
glm::vec3 Engine::Core::Camera::getPosition() const noexcept {
  return position;
};



void Engine::Core::Camera::setPosition(glm::vec3 position) noexcept {
  view_mat_ready = false;
  transform_mat_ready = false;
  
  this->position = position;
};



glm::vec3 Engine::Core::Camera::getDirection() const noexcept {
  return direction;
};



void Engine::Core::Camera::setDirection(glm::vec3 direction) noexcept {
  view_mat_ready = false;
  transform_mat_ready = false;

  if (glm::length(direction) > Engine::Config::EPS) {
    this->direction = glm::normalize(direction);
    this->orientation = glm::quatLookAt(-this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
  } else {
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  };
  
  resetMouse();
};



float Engine::Core::Camera::getFov() const noexcept {
  return fov;
};



void Engine::Core::Camera::setFov(float fov) noexcept {
  perspective_mat_ready = false;
  transform_mat_ready = false;
  this->fov = fov;
  if(fov <= Engine::Config::EPS) this->fov = 0.0f;
};



float Engine::Core::Camera::getOrthoSize() const noexcept {
  return ortho_size;
};



void Engine::Core::Camera::setOrthoSize(float size) noexcept {
  orthogonal_mat_ready = false;
  transform_mat_ready = false;
  this->ortho_size = size;
  if(size <= Engine::Config::EPS) this->ortho_size = 0.0f;
};



float Engine::Core::Camera::getNearPlane() const noexcept{
  if(mode == Engine::ScriptShared::CameraMode::ORTHOGONAL)
    return getNearOrthogonalPlane();
  return getNearPerspectivePlane();
};



void Engine::Core::Camera::setNearPlane(float near) noexcept{
  if(mode == Engine::ScriptShared::CameraMode::ORTHOGONAL)
    setNearOrthogonalPlane(near);
  else setNearPerspectivePlane(near);
};



float Engine::Core::Camera::getFarPlane() const noexcept{
  if(mode == Engine::ScriptShared::CameraMode::ORTHOGONAL)
    return getFarOrthogonalPlane();
  return getFarPerspectivePlane();
};



void Engine::Core::Camera::setFarPlane(float far) noexcept {
  if(mode == Engine::ScriptShared::CameraMode::ORTHOGONAL)
    setFarOrthogonalPlane(far);
  else setFarPerspectivePlane(far);
};



float Engine::Core::Camera::getNearPerspectivePlane() const noexcept {
  return perspective_near_plane;
};



void Engine::Core::Camera::setNearPerspectivePlane(float near) noexcept {
  perspective_mat_ready = false;
  transform_mat_ready = false;
  perspective_near_plane = near;
  if(perspective_near_plane <= Engine::Config::EPS) this->perspective_near_plane = 0.0f;
};



float Engine::Core::Camera::getFarPerspectivePlane() const noexcept {
  return perspective_far_plane;
};



void Engine::Core::Camera::setFarPerspectivePlane(float far) noexcept {
  perspective_mat_ready = false;
  transform_mat_ready = false;
  perspective_far_plane = far;
  if(perspective_far_plane <= Engine::Config::EPS) this->perspective_far_plane = 0.0f;
};



float Engine::Core::Camera::getNearOrthogonalPlane() const noexcept {
  return orthogonal_near_plane;
};



void Engine::Core::Camera::setNearOrthogonalPlane(float near) noexcept {
  orthogonal_mat_ready = false;
  transform_mat_ready = false;
  orthogonal_near_plane = near;
  if(orthogonal_near_plane <= Engine::Config::EPS) this->orthogonal_near_plane = 0.0f;
};



float Engine::Core::Camera::getFarOrthogonalPlane() const noexcept {
  return orthogonal_far_plane;
};



void Engine::Core::Camera::setFarOrthogonalPlane(float far) noexcept {
  orthogonal_mat_ready = false;
  transform_mat_ready = false;
  orthogonal_far_plane = far;
  if(orthogonal_far_plane <= Engine::Config::EPS) this->orthogonal_far_plane = 0.0f;
};



Engine::ScriptShared::CameraMode Engine::Core::Camera::getCameraMode() const noexcept {
  return mode;
};



void Engine::Core::Camera::setCameraMode(Engine::ScriptShared::CameraMode mode) noexcept {
  transform_mat_ready = false;
  this->mode = mode;
};



glm::ivec2 Engine::Core::Camera::getFboSize() const noexcept {
  return fbo_size;
};



void Engine::Core::Camera::setFboSize(glm::ivec2 size) noexcept {
  fbo_size = size;
  if(fbo_size.x < 0) fbo_size.x = 0;
  if(fbo_size.y < 0) fbo_size.y = 0;
  fbo_size_ready = false;
};



CW::Renderer::Framebuffer &Engine::Core::Camera::getFbo() noexcept {
  return fbo;
};



void Engine::Core::Camera::setTrackWindowSize(bool track) noexcept {
  track_window_size = track;
};



bool Engine::Core::Camera::getTrackWindowSize() const noexcept {
  return track_window_size;
};



bool Engine::Core::Camera::getDefaultMovement() const noexcept {
  return default_movemement_on;
};



void Engine::Core::Camera::setDefaultMovement(bool state) noexcept {
  default_movemement_on = state;
};



float Engine::Core::Camera::getVelocity() const noexcept {
  return velocity;
};



void Engine::Core::Camera::setVelocity(float velocity) noexcept {
  this->velocity = velocity;
  if(velocity <= Engine::Config::EPS) this->velocity = 0.0f;
};



float Engine::Core::Camera::getSensitivity() const noexcept {
  return sensitivity;
};



void Engine::Core::Camera::setSensitivity(float sensitivity) noexcept {
  this->sensitivity = sensitivity;
  if(sensitivity <= Engine::Config::EPS) this->sensitivity = 0.0f;
};



bool Engine::Core::Camera::getMouseActive() const noexcept{
  return mouse_is_active;
};



void Engine::Core::Camera::setMouseActive(bool active) noexcept{
  mouse_is_active = active;
};



//// ================ ////
//// ==== Render ==== ////
//// ================ ////
void Engine::Core::Camera::render(std::vector<Engine::Core::GameObject> &objects){
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::render(std::vector<Engine::Core::GameObject> &objects)", "renderer is nullptr {skipping}");
    return; 
  };

  if(track_window_size) autoSizeToWindow();
  if(!fbo_size_ready) {
    fbo.rescale(fbo_size.x, fbo_size.y);
    fbo_size_ready = true;
  };
  
  
  fbo.bind();
  CW::Renderer::Uniform uniform;
  for(Engine::Core::GameObject& object : objects) object.render(renderer, static_cast<Engine::ScriptShared::ICamera&>(*this), static_cast<Engine::ScriptShared::ICamera&>(*this), uniform);
  fbo.unbind();
};



void Engine::Core::Camera::autoSizeToWindow(){
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::autoSizeToWindow()", "renderer is nullptr {skipping}");
    return; 
  };

  if(renderer->getWindowData()->width == fbo_size.x && renderer->getWindowData()->height == fbo_size.y) return;
  setFboSize({renderer->getWindowData()->width, renderer->getWindowData()->height});
};



void Engine::Core::Camera::clearFbo(){
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::clearFbo()", "renderer is nullptr {skipping}");
    return; 
  };

  if(track_window_size) autoSizeToWindow();
  if(!fbo_size_ready) {
    fbo.rescale(fbo_size.x, fbo_size.y);
    fbo_size_ready = true;
  };

  fbo.bind();
  renderer->beginFrame();
  fbo.unbind();
};



//// ================== ////
//// ==== Movement ==== ////
//// ================== ////
void Engine::Core::Camera::event(float delta_time) {
  if(!default_movemement_on) return;

  cursorControl(delta_time);
  if(cursor_lock) return;

  float target_bank = 0.0f;
  velocityButtons(delta_time);
  movementButtons(delta_time, target_bank);
  rotationButtons(delta_time, target_bank);
  
  mouse_is_active = true;
};



void Engine::Core::Camera::resetMouse(){
  mouse_is_active = false;
};



void Engine::Core::Camera::cursorControl(float delta_time) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::cursorControl()", "renderer is nullptr skiping");
    return;
  };

  if (cursor_lock) renderer->setCursorOn(true);
  else renderer->setCursorOn(false);

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_SWAP_MODE_BTN) && cursor_visible_lock <= 0.0f) {
    cursor_lock = !cursor_lock;
    cursor_visible_lock = Engine::Config::CAMERA_SWAP_COOLDOWN;
    resetMouse();
  }
  else if (cursor_visible_lock > 0.0f) {
    cursor_visible_lock -= delta_time;
  };
};



void Engine::Core::Camera::velocityButtons(float delta_time) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::velocityButtons()", "renderer is nullptr skiping");
    return;
  };

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ACCELERATE)) velocity += Engine::Config::CAMERA_ACCELERATION_RATE * delta_time;
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_DECELERATE)) velocity -= Engine::Config::CAMERA_ACCELERATION_RATE * delta_time;
  if (velocity < Engine::Config::CAMERA_MIN_VELOCITY) velocity = Engine::Config::CAMERA_MIN_VELOCITY;
};



void Engine::Core::Camera::movementButtons(float delta_time, float& target_bank) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::movementButtons()", "renderer is nullptr skiping");
    return;
  };

  glm::vec3 right = orientation * glm::vec3(1.0f, 0.0f, 0.0f);

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_FORWARD)) position += direction * velocity * delta_time;
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_BACK))    position -= direction * velocity * delta_time;
  
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_RIGHT)) {
    position -= right * velocity * delta_time;
    target_bank -= Engine::Config::CAMERA_TILT_ACCELERATION;
  };
  
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_MOVE_LEFT)) {
    position += right * velocity * delta_time;
    target_bank += Engine::Config::CAMERA_TILT_ACCELERATION;
  };
  
  view_mat_ready = false;
  transform_mat_ready = false;
};



void Engine::Core::Camera::rotationButtons(float delta_time, float& target_bank_input) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Core::Camera::rotationButtons()", "renderer is nullptr skiping");
    return;
  };

  float xoffset = renderer->getInputData()->mouse_x - lastMouseX;
  float yoffset = renderer->getInputData()->mouse_y - lastMouseY; 
  lastMouseX = renderer->getInputData()->mouse_x;
  lastMouseY = renderer->getInputData()->mouse_y;

  if (mouse_is_active) {
    glm::vec3 localRight = orientation * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::quat qPitch = glm::angleAxis(glm::radians(yoffset * sensitivity), localRight);
    
    glm::quat qYaw = glm::angleAxis(glm::radians(-xoffset * sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
    
    orientation = qYaw * qPitch * orientation;
    orientation = glm::normalize(orientation);
  };

  bool manual_roll = false;
  float roll_input = 0.0f;

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ROLL_LEFT)) {
    roll_input = -Engine::Config::CAMERA_MAX_TILT;
    manual_roll = true;
  };

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ROLL_RIGHT)) {
    roll_input = Engine::Config::CAMERA_MAX_TILT;
    manual_roll = true;
  };

  if (!manual_roll) {
    roll_input = target_bank_input;
  };

  glm::vec3 localForward = orientation * glm::vec3(0.0f, 0.0f, 1.0f);
  glm::quat targetBankRot = glm::angleAxis(glm::radians(roll_input), localForward);
  
  glm::quat targetOrientation = targetBankRot * orientation;

  orientation = glm::slerp(orientation, targetOrientation, Engine::Config::CAMERA_ROLL_INTERPOLATION_SPEED * delta_time);
  orientation = glm::normalize(orientation);

  direction = orientation * glm::vec3(0.0f, 0.0f, 1.0f);

  view_mat_ready = false;
  transform_mat_ready = false;
};
