#ifndef QUARKGL_CAMERA_H_
#define QUARKGL_CAMERA_H_

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <qrk/shared.h>

namespace qrk {

enum class CameraDirection {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

constexpr float DEFAULT_YAW = 270.0f;
constexpr float DEFAULT_PITCH = 0.0f;
constexpr float DEFAULT_SPEED = 2.5f;
constexpr float DEFAULT_SENSITIVITY = 0.1f;
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEAR = 0.1f;
constexpr float DEFAULT_FAR = 100.0f;
constexpr float DEFAULT_ASPECT_RATIO = 4.0f / 3.0f;

constexpr float MIN_FOV = 1.0f;
constexpr float MAX_FOV = 45.0f;

class Camera {
 private:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 worldUp_;

  float yaw_;
  float pitch_;
  float speed_;
  float sensitivity_;
  float fov_;

  float aspectRatio_;
  float near_;
  float far_;

  void updateCameraVectors();

 public:
  Camera(glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH,
         float speed = DEFAULT_SPEED, float sensitivity = DEFAULT_SENSITIVITY,
         float fov = DEFAULT_FOV, float aspectRatio = DEFAULT_ASPECT_RATIO,
         float near = DEFAULT_NEAR, float far = DEFAULT_FAR);

  glm::vec3 getPosition() { return position_; }
  glm::vec3 getFront() { return front_; }
  float getFov() { return fov_; }

  // TODO: Allow programmatic position modification.
  // TODO: Add the rest of the getters/setters for fields.
  float getAspectRatio() { return aspectRatio_; }
  void setAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
  void setAspectRatio(ScreenSize size) {
    aspectRatio_ = size.width / static_cast<float>(size.height);
  }

  glm::mat4 getViewTransform();
  glm::mat4 getPerspectiveTransform();
  void processKeyboard(CameraDirection direction, float deltaTime);
  void processMouseMove(float xoffset, float yoffset,
                        bool constrainPitch = true);
  void processMouseScroll(float yoffset);
};
}  // namespace qrk

#endif
