// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Camera.h"



//// ====================== ////
//// ==== Constructors ==== ////
//// ====================== ////
//// core
Engine::Camera::Camera() noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::Camera()", "renderer is nullptr");
  };
};



Engine::Camera::~Camera() noexcept {};



Engine::Camera::Camera(CW::Renderer::Renderer* renderer, glm::vec3 position, glm::vec3 direction) noexcept
  : renderer(renderer) {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::Camera(CW::Renderer::Renderer* renderer, glm::vec3 position, glm::vec3 direction)", "renderer is nullptr");
  };

  setPosition(position);
  setDirection(direction);
};



//// copy
Engine::Camera::Camera(const Camera &second) noexcept
  :renderer(second.renderer),
   position(second.position),
   direction(second.direction),
   orientation(second.orientation),
   mode(second.mode),
   fov(second.fov),
   ortho_size(second.ortho_size),
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
    Engine::Utils::Logger::get().warn("Engine::Camera::Camera(const Camera &second)", "renderer is nullptr");
  };
};



Engine::Camera &Engine::Camera::operator=(const Camera &second) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::operator=(const Camera &second)", "renderer is nullptr");
  };

  renderer = second.renderer;
  setPosition(second.position);
  setDirection(second.direction);
  setCameraMode(second.mode);
  setFov(second.fov);
  setOrthoSize(second.ortho_size);
  setDefaultMovement(second.default_movemement_on);
  setSensitivity(second.sensitivity);
  setVelocity(second.velocity);

  lastMouseX = second.lastMouseX;
  lastMouseY = second.lastMouseY;
  mouse_is_active = false;
  cursor_visible_lock = second.cursor_visible_lock;
  cursor_lock = second.cursor_lock; 

  return *this;
};



//// move
Engine::Camera::Camera(Camera &&second) noexcept   
  :renderer(std::move(second.renderer)),
   position(std::move(second.position)),
   direction(std::move(second.direction)),
   orientation(std::move(second.orientation)),
   mode(std::move(second.mode)),
   fov(std::move(second.fov)),
   ortho_size(std::move(second.ortho_size)),
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
    Engine::Utils::Logger::get().warn("Engine::Camera::Camera(Camera &&second)", "renderer is nullptr");
  };
};



Engine::Camera& Engine::Camera::operator=(Camera &&second) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::operator=(Camera &&second)", "renderer is nullptr");
  };

  renderer = std::move(second.renderer);
  setPosition(std::move(second.position));
  setDirection(std::move(second.direction));
  setCameraMode(std::move(second.mode));
  setFov(std::move(second.fov));
  setOrthoSize(std::move(second.ortho_size));
  setDefaultMovement(std::move(second.default_movemement_on));
  setSensitivity(std::move(second.sensitivity));
  setVelocity(std::move(second.velocity));

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
glm::mat4 Engine::Camera::transformation() const noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::transformation()", "renderer is nullptr {returning mat4(1.0f)}");
    return glm::mat4(1.0f); 
  };

  return projection() * view();
};



glm::mat4 Engine::Camera::view() const noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::view()", "renderer is nullptr {returning mat4(1.0f)}");
    return glm::mat4(1.0f); 
  };

  glm::vec3 dynamicUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);
  return glm::lookAt(position, position + direction, dynamicUp);
};



glm::mat4 Engine::Camera::projection() const noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::projection()", "renderer is nullptr {returning mat4(1.0f)}");
    return glm::mat4(1.0f); 
  };

  if (mode == Engine::CameraMode::ORTHOGONAL) {
    return orthogonal_projection();
  } else {
    return projection_projection();
  };
};



glm::mat4 Engine::Camera::projection_projection() const noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::projection_projection()", "renderer is nullptr {returning mat4(1.0f)}");
    return glm::mat4(1.0f); 
  };

  float aspectRatio = renderer->getWindowData()->width / (float)renderer->getWindowData()->height;
  return glm::perspective(glm::radians(Engine::Config::CAMERA_FOV), aspectRatio, Engine::Config::CAMERA_NEAR_PLANE, Engine::Config::CAMERA_FAR_PLANE);
};



glm::mat4 Engine::Camera::orthogonal_projection() const noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::orthogonal_projection()", "renderer is nullptr {returning mat4(1.0f)}");
    return glm::mat4(1.0f); 
  };

  float aspectRatio = renderer->getWindowData()->width / (float)renderer->getWindowData()->height; 
  float halfWidth = (ortho_size * aspectRatio) * 0.5f;
  float halfHeight = ortho_size * 0.5f;
  return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, Engine::Config::CAMERA_NEAR_PLANE, Engine::Config::CAMERA_ORTHO_FAR_PLANE);  
};



//// ========================= ////
//// ==== Setters/Getters ==== ////
//// ========================= ////
glm::vec3 Engine::Camera::getPosition() const noexcept {
  return position;
};



void Engine::Camera::setPosition(glm::vec3 position) noexcept {
  this->position = position;
};



glm::vec3 Engine::Camera::getDirection() const noexcept {
  return direction;
};



void Engine::Camera::setDirection(glm::vec3 direction) noexcept {
  if (glm::length(direction) > Engine::Config::EPS) {
    this->direction = glm::normalize(direction);
    this->orientation = glm::quatLookAt(-this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
  } else {
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  };
  
  resetMouse();
};



float Engine::Camera::getFov() const noexcept {
  return fov;
};



void Engine::Camera::setFov(float fov) noexcept {
  this->fov = fov;
  if(fov <= Engine::Config::EPS) this->fov = 0.0f;
};



float Engine::Camera::getOrthoSize() const noexcept {
  return ortho_size;
};



void Engine::Camera::setOrthoSize(float size) noexcept {
  this->ortho_size = size;
  if(size <= Engine::Config::EPS) this->ortho_size = 0.0f;
};



Engine::CameraMode Engine::Camera::getCameraMode() const noexcept {
  return mode;
};



void Engine::Camera::setCameraMode(Engine::CameraMode mode) noexcept {
  this->mode = mode;
};



bool Engine::Camera::getDefaultMovement() const noexcept {
  return default_movemement_on;
};



void Engine::Camera::setDefaultMovement(bool state) noexcept {
  default_movemement_on = state;
};



float Engine::Camera::getVelocity() const noexcept {
  return velocity;
};



void Engine::Camera::setVelocity(float velocity) noexcept {
  this->velocity = velocity;
  if(velocity <= Engine::Config::EPS) this->velocity = 0.0f;
};



float Engine::Camera::getSensitivity() const noexcept {
  return sensitivity;
};



void Engine::Camera::setSensitivity(float sensitivity) noexcept {
  this->sensitivity = sensitivity;
  if(sensitivity <= Engine::Config::EPS) this->sensitivity = 0.0f;
};



bool Engine::Camera::getMouseActive() const noexcept{
  return mouse_is_active;
};



void Engine::Camera::setMouseActive(bool active) noexcept{
  mouse_is_active = active;
};



//// ================== ////
//// ==== Movement ==== ////
//// ================== ////
void Engine::Camera::event(float delta_time) {
  if(!default_movemement_on) return;

  cursorControl(delta_time);
  if(cursor_lock) return;

  float target_bank = 0.0f;
  velocityButtons(delta_time);
  movementButtons(delta_time, target_bank);
  rotationButtons(delta_time, target_bank);
  
  mouse_is_active = true;
};



void Engine::Camera::resetMouse(){
  mouse_is_active = false;
};



void Engine::Camera::cursorControl(float delta_time) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::cursorControl()", "renderer is nullptr skiping");
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



void Engine::Camera::velocityButtons(float delta_time) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::velocityButtons()", "renderer is nullptr skiping");
    return;
  };

  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_ACCELERATE)) velocity += Engine::Config::CAMERA_ACCELERATION_RATE * delta_time;
  if (renderer->getInputData()->is_key_down(Engine::Config::CAMERA_DECELERATE)) velocity -= Engine::Config::CAMERA_ACCELERATION_RATE * delta_time;
  if (velocity < Engine::Config::CAMERA_MIN_VELOCITY) velocity = Engine::Config::CAMERA_MIN_VELOCITY;
};



void Engine::Camera::movementButtons(float delta_time, float& target_bank) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::movementButtons()", "renderer is nullptr skiping");
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
};



void Engine::Camera::rotationButtons(float delta_time, float& target_bank_input) noexcept {
  if (!renderer || !renderer->getWindowData()) {
    Engine::Utils::Logger::get().warn("Engine::Camera::rotationButtons()", "renderer is nullptr skiping");
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
};
