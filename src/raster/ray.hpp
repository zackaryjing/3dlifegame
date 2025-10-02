#pragma once
#include "raster/vec3.hpp"

class ray {
public:
    vec3 _origin;
    vec3 _dir;
    float _time;
    ray() = default;
    ray(const vec3 &origin, const vec3 &dir, float ti = 0.0) {
        _origin = origin;
        _dir = dir;
        _time = ti;
    };
    float time() const { return _time; }
    [[nodiscard]] vec3 origin() const { return _origin; };
    [[nodiscard]] vec3 direction() const { return _dir; };
    [[nodiscard]] vec3 point_at_parameter(float t) const {
        return _origin + t * _dir;
    };
};


vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * dot(v, n) * n; }

// bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
// {
//     vec3 uv = unit(v);
//     float dt = dot(uv, n);
//     float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
//     if (discriminant > 0) {
//         refracted = ni_over_nt * (v - n * dt) - n * std::sqrt(discriminant);
//         return true;
//     } else {
//         return false;
//     }
// }

bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted) {
    const vec3 uv = unit(v);
    const vec3 un = unit(n);
    float h = -dot(uv, un);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - h * h);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv + h * un) - un * std::sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}
