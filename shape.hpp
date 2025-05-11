#ifndef SHAPE_H
#define SHAPE_H

#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
class Shape {
public:
  virtual ~Shape() = default;
  virtual Vec3 normalAtPoint(const Vec3 &p) const = 0;
};

struct HitRecord {
  Vec3 pos;
  Vec3 normal;
  Vec3 reflect;
  Vec3 transmit;
  double t;
  bool front_face;
  float w_r;
  float w_t;
  float w_l;
  inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
    front_face = dot(r.direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hitable {
public:
  virtual ~Hitable() = default;
  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const = 0;
};

class Sphere : public Shape, public Hitable {

public:
  Sphere(Vec3 c, float r, Material m);
  Vec3 normalAtPoint(const Vec3 &p) const override;
  bool hit(const Ray &r, double t_min, double t_max,
           HitRecord &rec) const override;

private:
  Material _material;
  Vec3 _center;
  float _radius;
};

#endif
