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
    (void) p;
    return color;
}


class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}
    vec3 value(float u, float v, const vec3 &p) const override {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        // float sines = sin(10 * p.x()) * sin(10 * pkz());
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
    noise_texture(float sc) : scale(sc) {}
    virtual vec3 value(float u, float v, const vec3 &p) const {
        (void) u;
        (void) v;
        // return vec3(1, 1, 1) * 0.5 * (1 + noise.noise(scale * p));
        // return vec3(1, 1, 1) * 0.5 * (1 + noise.turb(scale * p));
        return vec3(1, 1, 1) * 0.5 *
               (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }
    perlin noise;
    float scale;
};

class image_texture : public texture {
public:
    image_texture() : data(nullptr), nx(0), ny(0) {}
    image_texture(unsigned char *pixels, int A, int B) :
        data(pixels), nx(A), ny(B) {}
    [[nodiscard]] vec3 value(float u, float v, const vec3 &p) const override;
    unsigned char *data;
    int nx, ny;
};

inline vec3 image_texture::value(float u, float v, const vec3 &p) const {
    int i = u * nx;
    int j = (1 - v) * ny - 0.001;
    std::clamp(i, 0, nx - 1);
    std::clamp(j, 0, ny - 1);
    float r = int(data[3 * i + 3 * nx * j]) / 255.0;
    float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0;
    float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0;
    return {r, g, b};
}

