#include "shape.hpp"

//
// Sphere
//

Vec3 transmit(const Vec3 &I, const Vec3 &N, float eta) {
  float cosi = std::fmax(-1.0f, std::fmin(1.0f, dot(I, N)));
  float etai = 1.0f, etat = eta;
  Vec3 n = N;
  if (cosi < 0) {
    cosi = -cosi;
  } else {
    std::swap(etai, etat);
    n = -N;
  }
  float etaRatio = etai / etat;
  float k = 1.0f - etaRatio * etaRatio * (1.0f - cosi * cosi);
  if (k < 0.0f) {
    return Vec3(0.0f, 0.0f, 0.0f);
  } else {
    return etaRatio * I + (etaRatio * cosi - std::sqrt(k)) * n;
  }
}

Sphere::Sphere(Vec3 c, float r, Material m)
    : _material(m), _center(c), _radius(r) {}

Vec3 Sphere::normalAtPoint(const Vec3 &p) const {
  Vec3 v = p;
  return unit(v - _center);
}

bool Sphere::hit(const Ray &r, double t_min, double t_max,
                 HitRecord &rec) const {

  Vec3 oc = r.origin - _center;
  double a = dot(r.direction, r.direction);
  double half_b = dot(oc, r.direction);
  double c = dot(oc, oc) - _radius * _radius;
  double d = half_b * half_b - a * c;
  if (d < 0)
    return false;
  double sqrt_d = std::sqrt(d);

  double root = (-half_b - sqrt_d) / a;
  if (root < t_min || root > t_max) {
    root = (-half_b + sqrt_d) / a;
    if (root < t_min || root > t_max)
      return false;
  }

  rec.t = root;
  rec.pos = r.pointAtParameter(root);
  rec.set_face_normal(r, (rec.pos - _center) / _radius);

  rec.normal =
      (rec.front_face ? 1.0f : -1.0f) * ((rec.pos - _center) / _radius);

  rec.reflect = r.direction - 2 * dot(r.direction, rec.normal) * rec.normal;
  rec.transmit =
      transmit(r.direction, rec.normal,
               !rec.front_face ? _material.ior : 1.0f / _material.ior);
  rec.w_l = _material.w_l;
  rec.w_r = _material.w_r;
  rec.w_t = _material.w_t;
  return true;
}
