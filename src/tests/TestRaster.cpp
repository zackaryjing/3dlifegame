#include <cfloat>
#include <fstream>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

#include "raster/bmp.hpp"
#include "raster/camera.hpp"
#include "raster/hitable_list.hpp"
#include "raster/material.hpp"
#include "raster/ray.hpp"
#include "raster/sphere.hpp"
#include "raster/utils.hpp"

using std::cout;
using std::endl;
using std::ofstream;
using std::sqrt;
using std::vector;


vec3 color(const ray &r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 &&
            rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return vec3(0, 0, 0);
        }
        // vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        // return 0.5 * color(ray(rec.p, target - rec.p), world);
    } else {
        vec3 unit_direction = unit(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere{vec3(0, -1000, 0), 1000,
                         new lambertian(vec3(0.5, 0.5, 0.5))};
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
                            new lambertian(vec3(
                                    Rand::gen_float() * Rand::gen_float(),
                                    Rand::gen_float() * Rand::gen_float(),
                                    Rand::gen_float() * Rand::gen_float())));
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
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0,
                           new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] =
            new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new hitable_list(list, i);
}

void test_scene() {

    constexpr int cnt = 5;

    hitable *list[cnt];
    float R = cos(M_PI / 4);
    list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0, 0, 1)));
    list[1] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));
    list[0] = new sphere(vec3(0, 0, -1), 0.5,
                         new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100,
                         new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list, cnt);

    camera cam(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0),
               90, 2.0, 0.0, 1.0, 0.0, 1.0);
}


int main() {
    int nx = 150 * 1.0;
    int ny = 100 * 1.0;
    int ns = 30;
    ofstream file("./out2.bmp", std::ios::binary);
    int rowStride = ((nx * 3 + 3) & (~3));


    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    infoHeader.biWidth = nx;
    infoHeader.biHeight = ny;
    infoHeader.biSizeImage = rowStride * ny;
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    file.write(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    vector file_content(ny, vector<uint8_t>(rowStride));

    hitable *world = random_scene();

    vec3 lookfrom(13, 2, 3.0);
    vec3 lookat(0.0, 0.0, 0.0);
    float aperture = 0.0;
    float dist_to_focus = (lookfrom - lookat).length() * 0.9f;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 15,
               static_cast<float>(nx) / static_cast<float>(ny), aperture,
               dist_to_focus, 0.0, 1.0);
    int done = 0;

#pragma omp parallel for collapse(2)
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            if (i == 0) {
                cout << std::format("progress: {:.2f}%",
                                    100 * done / static_cast<float>(ny))
                     << endl;
                done++;
            }

            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = (static_cast<float>(i) + Rand::gen_float()) /
                          static_cast<float>(nx);
                float v = (static_cast<float>(j) + Rand::gen_float()) /
                          static_cast<float>(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            col /= static_cast<float>(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            auto ir = static_cast<uint8_t>(255.99 * col[0]);
            auto ig = static_cast<uint8_t>(255.99 * col[1]);
            auto ib = static_cast<uint8_t>(255.99 * col[2]);
            file_content[j][i * 3 + 2] = ir;
            file_content[j][i * 3 + 1] = ig;
            file_content[j][i * 3 + 0] = ib;
        }
    }
    for (auto &row_content: file_content) {
        file.write(reinterpret_cast<char *>(row_content.data()), rowStride);
    }
    file.close();
}
