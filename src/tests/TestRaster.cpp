#include <cfloat>
#include <fstream>
#include <glm/detail/qualifier.hpp>
#include <iostream>
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


int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    ofstream file("./out.bmp", std::ios::binary);
    int rowStride = ((nx * 3 + 3) & (~3));


    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    infoHeader.biWidth = nx;
    infoHeader.biHeight = ny;
    infoHeader.biSizeImage = rowStride * ny;
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    file.write(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    vector<uint8_t> row(rowStride);

    constexpr int cnt = 5;

    hitable *list[cnt];
    list[0] = new sphere(vec3(0, 0, -1), 0.5,
                         new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100,
                         new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list, cnt);

    camera cam;

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = (static_cast<float>(i) + Rand::gen_float()) /
                          static_cast<float>(nx);
                float v = (static_cast<float>(j) + Rand::gen_float()) /
                          static_cast<float>(ny);
                ray r = cam.get_ray(u, v);
                // vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }

            col /= static_cast<float>(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            auto ir = static_cast<uint8_t>(255.99 * col[0]);
            auto ig = static_cast<uint8_t>(255.99 * col[1]);
            auto ib = static_cast<uint8_t>(255.99 * col[2]);
            row[i * 3 + 2] = ir;
            row[i * 3 + 1] = ig;
            row[i * 3 + 0] = ib;
            // cout << std::vformat("{:3d} {:3d} {:3d}",
            //                      std::make_format_args(ib, ig, ir))
            //      << endl;
        }
        file.write(reinterpret_cast<char *>(row.data()), rowStride);
    }
    file.close();
}
