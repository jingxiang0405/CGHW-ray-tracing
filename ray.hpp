#ifndef RAY_H
#define RAY_H
#include "vec3.hpp"
struct Ray {

  Vec3 origin;
  Vec3 direction;
  Ray(const Vec3 &o, const Vec3 &d) : origin(o), direction(d) {}
  Vec3 pointAtParameter(float t) const { return t * direction + origin; }
};
#endif
