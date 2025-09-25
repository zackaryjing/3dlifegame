#pragma once
#include "raster/ray.hpp"
#include "hitable.hpp"

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scatterd) const = 0;
};
