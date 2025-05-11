#ifndef SCENE_H
#define SCENE_H
#include "ray.hpp"
#include "shape.hpp"
#include <memory.h>
#include <vector>
class Scene : public Hitable {
public:
  void add(std::shared_ptr<Hitable>);

  bool hit(const Ray &r, double t_min, double t_max,
           HitRecord &rec) const override;

  std::vector<std::shared_ptr<Hitable>> objects;
};

Scene build_scene();

#endif
