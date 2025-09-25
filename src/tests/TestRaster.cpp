#include <cfloat>
#include <chrono>
#include <fstream>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <random>
#include <vector>

#include "raster/bmp.hpp"
#include "raster/camera.hpp"
#include "raster/hitable.hpp"
#include "raster/hitable_list.hpp"
#include "raster/ray.hpp"
#include "raster/sphere.hpp"
using std::cout;
using std::endl;
using std::ofstream;
using std::sqrt;
using std::vector;
using std::chrono::system_clock;

unsigned seed = system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);
std::uniform_real_distribution<float> dist(0.0f, 1.0f);

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(dist(generator), dist(generator), dist(generator)) -
            vec3(1, 1, 1);
    } while (dot(p, p) >= 1.0);
    return p;
}

vec3 color(const ray &r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
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

    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);

    camera cam;

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = (i + dist(generator)) / static_cast<float>(nx);
                float v = (j + dist(generator)) / static_cast<float>(ny);
                ray r = cam.get_ray(u, v);
                // vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
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
