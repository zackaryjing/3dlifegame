#pragma once
#include "raster/aabb.hpp"
#include "raster/ray.hpp"
class material;


struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
    float u;
    float v;
};

class hitable {
public:
    virtual ~hitable() = default;
    virtual bool hit(const ray &r, float t_min, float t_max,
                     hit_record &rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb &box) const = 0;
};
