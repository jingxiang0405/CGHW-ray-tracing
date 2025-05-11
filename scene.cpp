#include "scene.hpp"
#include "shape.hpp"
#include <memory>

void Scene::add(std::shared_ptr<Hitable> obj) { objects.push_back(obj); }
bool Scene::hit(const Ray &r, double t_min, double t_max,
                HitRecord &rec) const {
  HitRecord temp;
  bool hit_anything = false;
  double closest = t_max;

  for (const auto &obj : objects) {
    if (obj->hit(r, t_min, closest, temp)) {
      hit_anything = true;
      closest = temp.t;
      rec = temp;
    }
  }
  return hit_anything;
}

Scene build_scene() {
  Scene world;

  world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -2), 100,
                                     Material::Lambertian()));
  world.add(std::make_shared<Sphere>(Vec3(0, 0, -2), 0.5, Material::Len()));
  world.add(
      std::make_shared<Sphere>(Vec3(1, 0, -1.75), 0.5, Material::Mirror()));
  world.add(std::make_shared<Sphere>(Vec3(-1, 0, -2.25), 0.5,
                                     Material::Lambertian()));

  srand(1234);

  Material materials[] = {Material::Mirror(), Material::Lambertian(),
                          Material::Len()};

  for (int i = 0; i < 48; i++) {

    float xr = ((float)rand() / (float)(RAND_MAX)) * 6.0f - 3.0f;

    float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;

    int mi = rand() % 3;
    world.add(
        std::make_shared<Sphere>(Vec3(xr, -0.45, zr - 2), 0.05, materials[mi]));
  }

  return world;
}
