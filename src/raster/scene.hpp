#pragma once
#include <stb_image.h>


#include "raster/bvh_node.hpp"
#include "raster/camera.hpp"
#include "raster/hitable.hpp"
#include "raster/hitable_list.hpp"
#include "raster/material.hpp"
#include "raster/sphere.hpp"
#include "raster/texture.hpp"
#include "raster/utils.hpp"
#include "raster/xy_rect.hpp"

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable *[n + 1];
    texture *checker =
            new checker_texture(new constant_texture({0.2, 0.3, 0.1}),
                                new constant_texture({0.9, 0.9, 0.9}));
    list[0] = new sphere{vec3(0, -1000, 0), 1000, new lambertian(checker)};
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = Rand::gen_float();
            vec3 center(a + 0.9 * Rand::gen_float(), 0.2,
                        b + 0.9 * Rand::gen_float());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    list[i++] = new moving_sphere(
                            center,
                            center + vec3(0, 0.5 * Rand::gen_float(), 0), 0.0,
                            1.0, 0.2,
                            new lambertian(new constant_texture(
                                    {Rand::gen_float() * Rand::gen_float(),
                                     Rand::gen_float() * Rand::gen_float(),
                                     Rand::gen_float() * Rand::gen_float()})));
                } else if (choose_mat < 0.95) {
                    list[i++] = new sphere(
                            center, 0.2,
                            new metal(vec3(0.5f * (1 + Rand::gen_float()),
                                           0.5f * (1 + Rand::gen_float()),
                                           0.5f * (1 + Rand::gen_float())),
                                      0.5f * Rand::gen_float()));
                } else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] =
            new sphere(vec3(-4, 1, 0), 1.0,
                       new lambertian(new constant_texture({0.4, 0.2, 0.1})));
    list[i++] =
            new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new bvh_node(list, i, 0.0, 1.0);
    // return new hitable_list(list, i);
}

hitable *two_spheres() {
    texture *checker =
            new checker_texture(new constant_texture({0.2, 0.3, 0.1}),
                                new constant_texture({0.9, 0.9, 0.9}));
    int n = 50;
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere({0, 10, 0}, 10, new lambertian(checker));
    list[1] = new sphere({0, -10, 0}, 10, new lambertian(checker));
    return new hitable_list(list, 2);
}

void test_scene() {

    constexpr int cnt = 5;

    hitable *list[cnt];
    float R = cos(M_PI / 4);
    list[0] = new sphere(vec3(-R, 0, -1), R,
                         new lambertian(new constant_texture({0, 0, 1})));
    list[1] = new sphere(vec3(R, 0, -1), R,
                         new lambertian(new constant_texture({1, 0, 0})));
    list[0] = new sphere(vec3(0, 0, -1), 0.5,
                         new lambertian(new constant_texture({0.1, 0.2, 0.5})));
    list[1] = new sphere(vec3(0, -100.5, -1), 100,
                         new lambertian(new constant_texture({0.8, 0.8, 0.0})));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list, cnt);

    camera cam(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0),
               90, 2.0, 0.0, 1.0, 0.0, 1.0);
}


hitable *two_perlin_spheres() {
    texture *pertext = new noise_texture(4);
    hitable **list = new hitable *[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
    return new hitable_list(list, 2);
}

hitable *earth_sphere() {
    int nx, ny, nn;
    unsigned char *tex_data =
            stbi_load(TEXTURE_DIR "earth_map_texture.jpg", &nx, &ny, &nn, 0);
    material *mat = new lambertian(new image_texture(tex_data, nx, ny));
    hitable **list = new hitable *[1];
    list[0] = new sphere(vec3(0, 0, 0), 2, mat);
    return new hitable_list(list, 1);
}

hitable *simple_light() {
    texture *pertext = new noise_texture(4);
    hitable **list = new hitable *[4];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
    list[2] = new sphere(
            vec3(0, 7, 0), 2,
            new diffuse_light(new constant_texture(vec3(0.7, 0.7, 0.7))));
    list[3] = new xy_rect(
            3, 5, 1, 3, -2,
            new diffuse_light(new constant_texture({0.7, 0.7, 0.7})));
    return new hitable_list(list, 4);
}
