#pragma once
#include "raster/perlin.hpp"
#include "raster/vec3.hpp"

class texture {
public:
    virtual ~texture() = default;
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class constant_texture : public texture {
public:
    constant_texture() {}
    constant_texture(vec3 c) : color(c) {}
    vec3 value(float u, float v, const vec3 &p) const override;
    vec3 color;
};
inline vec3 constant_texture::value(const float u, const float v,
                                    const vec3 &p) const {
    (void) u;
    (void) v;
    return color;
}


class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}
    vec3 value(float u, float v, const vec3 &p) const override {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        // float sines = sin(10 * p.x()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }
    texture *even;
    texture *odd;
};

class noise_texture : public texture {
public:
    noise_texture() {}
    virtual vec3 value(float u, float v, const vec3 &p) const {
        (void) u;
        (void) v;
        return vec3(1, 1, 1) * noise.noise(p);
    }
    perlin noise;
};
