#include <cfloat>
#include <fstream>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

#include "raster/bmp.hpp"
#include "raster/camera.hpp"
#include "raster/material.hpp"
#include "raster/ray.hpp"
#include "raster/scene.hpp"
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
    } else {
        vec3 unit_direction = unit(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

// out2 22.38
// out3 7.54
int main() {
    int nx = 150 * 1.0;
    int ny = 100 * 1.0;
    int ns = 30;
    ofstream file("./out4.bmp", std::ios::binary);
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

    hitable *world = two_perlin_spheres();

    vec3 lookfrom(13, 2, 3.0);
    vec3 lookat(0.0, 0.0, 0.0);
    float aperture = 0.0;
    float dist_to_focus = (lookfrom - lookat).length() * 0.9f;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20,
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
