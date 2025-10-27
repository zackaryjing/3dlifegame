#pragma once
#include <cmath>
#include "raster/utils.hpp"
#include "raster/vec3.hpp"

using std::floor;

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
    float accum = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                accum += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) * c[i][j][k];
            }
        }
    }
    return accum;
}

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accum = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                vec3 weight_v(u - i, v - j, w - k);
                float i_weight = (i == 0) ? (1 - uu) : uu;
                float j_weight = (j == 0) ? (1 - vv) : vv;
                float k_weight = (k == 0) ? (1 - ww) : ww;
                accum += i_weight * j_weight * k_weight *
                         dot(c[i][j][k], weight_v);
            }
        }
    }
    return accum;
}

class perlin {
public:
    float noise(const vec3 &p) const {
        const float u = p.x() - floor(p.x());
        const float v = p.y() - floor(p.y());
        const float w = p.z() - floor(p.z());
        const int i = floor(p.x());
        const int j = floor(p.y());
        const int k = floor(p.z());
        vec3 c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^
                                           perm_y[(j + dj) & 255] ^
                                           perm_z[(k + dk) & 255]];
                }
            }
        }
        return perlin_interp(c, u, v, w);
    }

    float turb(const vec3 &p, int depth = 7) const {
        float accum = 0;
        vec3 temp_p = p;
        float weight = 1.0;
        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }
        return fabs(accum);
    }

    static vec3 *ranvec;
    static float *ranfloat;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};

static float *perlin_generate_float() {
    const auto p = new float[256];
    for (int i = 0; i < 256; ++i) {
        p[i] = Rand::gen_float();
    }
    return p;
}

static vec3 *perlin_generate() {
    const auto p = new vec3[256];
    for (int i = 0; i < 256; ++i) {
        p[i] = unit(vec3{-1 + 2 * Rand::gen_float(), -1 + 2 * Rand::gen_float(),
                         -1 + 2 * Rand::gen_float()});
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


vec3 *perlin::ranvec = perlin_generate();
float *perlin::ranfloat = perlin_generate_float();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();
