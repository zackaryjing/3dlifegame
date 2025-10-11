#pragma once
#include <cmath>
#include "raster/utils.hpp"
#include "raster/vec3.hpp"

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
    float accum = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                accum += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w) * c[i][j][k]);
            }
        }
    }
    return accum;
}

class perlin {
public:
    float noise(const vec3 &p) const {
        // TODO: find out what the u,v,w here is used for?
        // why multiple by 4?
        float u = p.x() - std::floor(p.x());
        float v = p.y() - std::floor(p.y());
        float w = p.z() - std::floor(p.z());
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        float c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = ranfloat[perm_x[(i + di) & 255] ^
                                             perm_y[(j + dj) & 255] ^
                                             perm_z[(k + dk) & 255]];
                }
            }
        }
        return trilinear_interp(c, u, v, w);
    }
    static float *ranfloat;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};

static float *perlin_generate() {
    const auto p = new float[256];
    for (int i = 0; i < 256; ++i) {
        p[i] = Rand::gen_float();
    }
    return p;
}

void permute(int *p, int n) {
    for (int i = n - 1; i > 0; i--) {
        int target = static_cast<int>(Rand::gen_float() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int *perlin_generate_perm() {
    int *p = new int[256];
    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

float *perlin::ranfloat = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();
