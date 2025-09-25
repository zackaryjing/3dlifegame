#pragma once
#include "raster/vec3.hpp"

class ray {
public:
    vec3 _origin;
    vec3 _dir;
    ray() = default;
    ray(const vec3 &origin, const vec3 &dir) {
        _origin = origin;
        _dir = dir;
    };
    [[nodiscard]] vec3 origin() const { return _origin; };
    [[nodiscard]] vec3 direction() const { return _dir; };
    [[nodiscard]] vec3 point_at_parameter(float t) const {
        return _origin + t * _dir;
    };
};
