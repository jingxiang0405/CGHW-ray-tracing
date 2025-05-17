#include "lodepng.h"
#include "ray.hpp"
#include "scene.hpp"
#include "shape.hpp"
#include "vec3.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include <limits>

const int MAX_DEPTH = 3;

float max(float a, float b) { return a > b ? a : b; }
// Light
Vec3 LightPosition(-10, 10, 0);
Vec3 LightIntensity(1.0, 1.0, 1.0);

HitRecord intersect(const Scene &world, Vec3 p, Vec3 d,
                    bool &have_intersection) {
  float t_min = 0.001f;
  float t_max = std::numeric_limits<float>::max();
  std::vector<HitRecord> records;

  for (auto &obj : world.objects) {
    HitRecord rec;
    if (obj->hit(Ray(p, d), t_min, t_max, rec)) {
      records.push_back(rec);
      have_intersection = true;
    }
  }

  if (!have_intersection)
    return HitRecord();

  HitRecord nearest = records[0];
  for (const auto &r : records) {
    if (r.t < nearest.t) {
      nearest = r;
    }
  }

  return nearest;
}

color shade(Vec3 L, Vec3 LightIntensity, HitRecord rec) {

  Vec3 color(0, 0, 0);

  float lightness = max(0, dot(rec.normal, L));
  color = lightness * LightIntensity;
  return color;
}
color ray_color(const Ray &r, int depth, const Scene &world) {

  if (depth <= 0)
    return color(0, 0, 0);

  HitRecord rec;
  // background
  if (!world.hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
    float a = 0.5 * (r.direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }
  color reflected(0, 0, 0);
  color transmitted(0, 0, 0);
  float w_l = 0, w_t = 0, w_r = 0;
  color local;

  Vec3 L = unit(LightPosition - rec.pos);
  bool have_intersection = false;
  intersect(world, rec.pos + 0.0001f * rec.normal, L, have_intersection);

  if (have_intersection) {
    // shadow
    local = color(0, 0, 0);
    w_l = 0.0f;
  } else {
    local = shade(L, LightIntensity, rec);
  }

  w_l = rec.w_l;
  w_t = rec.w_t;
  w_r = rec.w_r;
  Vec3 bias = 0.0001f * rec.normal;
  Ray ray_reflect(rec.pos + bias, rec.reflect);
  Ray ray_transmit(rec.pos - bias, rec.transmit);

  reflected = ray_color(ray_reflect, depth - 1, world);
  transmitted = ray_color(ray_transmit, depth - 1, world);

  return (w_l * local + w_r * reflected + w_t * transmitted);
}

// Camera
Vec3 origin(0, 0, 1);
Vec3 lower_left_corner(-2, -1, -1);
Vec3 horizontal(4, 0, 0);
Vec3 vertical(0, 2, 0);

namespace fs = std::filesystem;
fs::path make_unique_path(const fs::path &desired) {
  if (!fs::exists(desired)) {
    return desired;
  }

  auto parent = desired.parent_path();
  auto stem = desired.stem().string();     // e.g. "output"
  auto ext = desired.extension().string(); // e.g. ".txt"

  int index = 1;
  while (true) {
    // build "stem(index).ext"
    std::ostringstream oss;
    oss << stem << '(' << index++ << ')' << ext;
    fs::path candidate = parent / oss.str();

    if (!fs::exists(candidate))
      return candidate;
  }
}
int main(int argc, char *argv[]) {
  auto aspect = 16.0 / 9.0;
  int width = 2000;
  int height = int(width / aspect);

  Scene world = build_scene();

  std::vector<unsigned char> framebuffer;
  framebuffer.resize(width * height * 4);
  fs::path outFile = make_unique_path("out/ray-tracing.ppm");
  std::ofstream file(outFile);
  //
  //
  file << "P3\n" << width << " " << height << "\n255\n";

  for (int j = height - 1; j >= 0; j--) {
    for (int i = 0; i < width; i++) {
      color pixel_color(0, 0, 0);
      float u = float(i) / float(width);
      float v = float(j) / float(height);
      Vec3 uvcenter = lower_left_corner + u * horizontal + v * vertical;
      Ray r(origin, unit(uvcenter - origin));
      pixel_color += ray_color(r, MAX_DEPTH, world);

      unsigned char R =
          (unsigned char)(255 * std::clamp(pixel_color.r(), 0.0f, 1.0f));
      unsigned char G =
          (unsigned char)(255 * std::clamp(pixel_color.g(), 0.0f, 1.0f));
      unsigned char B =
          (unsigned char)(255 * std::clamp(pixel_color.b(), 0.0f, 1.0f));
      int row = height - 1 - j;
      int idx = 4 * (row * width + i);
      framebuffer[idx + 0] = R;
      framebuffer[idx + 1] = G;
      framebuffer[idx + 2] = B;
      framebuffer[idx + 3] = 255;
      // write color
      // file << int(pixel_color.r() * 255) << " " << int(pixel_color.g() * 255)
      //      << " " << int(pixel_color.b() * 255) << "\n";
    }
  }

  unsigned error = lodepng::encode("out/ray.png", framebuffer, width, height);
  if (error) {
    std::cerr << "PNG encoding error " << error << ": "
              << lodepng_error_text(error) << "\n";
    return 1;
  }

  std::cout << "Saved image to out/ray.png\n";
  return 0;
}
