class Material {

public:
  static Material Lambertian() { return Material(1.0f, 0.0f, 0.0f, 1.0f); }
  static Material Mirror() { return Material(0.0f, 1.0f, 0.0f, 1.517f); }

  static Material Len() { return Material(0.0f, 0.1f, 0.9f, 1.517f); }
  float w_l;
  float w_r;
  float w_t;
  float ior;

private:
  Material(float l, float r, float t, float ior)
      : w_l(l), w_r(r), w_t(t), ior(ior) {}
};
