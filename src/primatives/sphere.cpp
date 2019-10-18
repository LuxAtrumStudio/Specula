#include "specula/primatives/sphere.hpp"

#include "specula/primatives/primative.hpp"

#include <functional>

#include <glm/glm.hpp>

#include "specula/fmt.hpp"
#include "specula/log.hpp"

specula::Sphere::Sphere(const float &r)
    : Primative([this](const glm::vec3 &p) {
        return glm::length(p) - this->radius_;
      }),
      radius_(r) {
}
