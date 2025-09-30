#pragma once
#include <iostream>
#include "raster/ray.hpp"
#include "raster/utils.hpp"
using std::cout;
using std::endl;

inline vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(Rand::gen_float(), Rand::gen_float(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
           float aperture, float focus_dist) {
        lens_radius = aperture / 2;
        const float theta = vfov * static_cast<float>(M_PI) / 180;
        const float half_height = std::tan(theta / 2);
        const float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit(lookfrom - lookat);
        u = unit(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - half_width * focus_dist * u -
                            half_height * focus_dist * v - w * focus_dist;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }
    ray get_ray(const float s, const float t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return {origin + offset, lower_left_corner + s * horizontal +
                                         t * vertical - origin - offset};
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};
