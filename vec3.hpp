#ifndef VEC_H
#define VEC_H
#include <cmath>
#include <iostream>

class Vec3 {
public:
  Vec3() {}
  Vec3(float e0, float e1, float e2) {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
  }
  inline float x() const { return e[0]; }
  inline float y() const { return e[1]; }
  inline float z() const { return e[2]; }
  inline float r() const { return e[0]; }
  inline float g() const { return e[1]; }
  inline float b() const { return e[2]; }

  inline bool operator==(const Vec3 &v) const {
    return e[0] == v.e[0] && e[1] == v.e[1] && e[2] == v.e[2];
  }
  inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
  inline Vec3 operator/(const float f) {
    return Vec3(-e[0] / f, -e[1] / f, -e[2] / f);
  }
  inline Vec3 &operator+=(const Vec3 &v2) {
    this->e[0] = this->x() + v2.x();
    this->e[1] = this->y() + v2.y();
    this->e[2] = this->z() + v2.z();
    return *this;
  }

  inline Vec3 &operator-=(const Vec3 &v2) {
    this->e[0] = this->x() - v2.x();
    this->e[1] = this->y() - v2.y();
    this->e[2] = this->z() - v2.z();
    return *this;
  }

  inline Vec3 &operator*=(const float t) {
    this->e[0] *= t;
    this->e[1] *= t;
    this->e[2] *= t;
    return *this;
  }

  inline Vec3 &operator/=(const float t) {
    this->e[0] /= t;
    this->e[1] /= t;
    this->e[2] /= t;
    return *this;
  }

  inline Vec3 operator+(const Vec3 v) {
    return Vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
  }

  inline float length() const {
    return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
  }

  inline float squared_length() const {
    return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
  }

  inline void make_unit() { *this /= length(); }

  float e[3];
};

inline float dot(const Vec3 &v1, const Vec3 &v2) {
  return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
  Vec3 result;
  result.e[0] = v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1];
  result.e[1] = v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0];
  result.e[2] = v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0];
  return result;
}

inline Vec3 operator-(const Vec3 v1, const Vec3 v2) {
  return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline Vec3 operator*(const float t, const Vec3 v) {
  return Vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}
inline Vec3 unit(Vec3 v) {
  Vec3 result;
  float l = result.length();
  result.e[0] = v.e[0] / l;
  result.e[1] = v.e[1] / l;
  result.e[2] = v.e[2] / l;
  return result;
}

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
  return out << "( " << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ')';
}

using color = Vec3;
#endif
