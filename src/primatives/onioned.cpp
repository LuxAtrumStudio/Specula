#include "specula/primatives/onioned.hpp"

#include "specula/primatives/primative.hpp"

#include <functional>

#include <glm/glm.hpp>

#include "specula/fmt.hpp"
#include "specula/log.hpp"

specula::OnionedPrimative::OnionedPrimative(
    const std::shared_ptr<Primative> &base, const float &t)
    : Primative([this](const glm::vec3 &p) {
        return glm::abs(this->base_->distance_(p)) - this->thickness_;
      }),
      thickness_(t), base_(base) {}