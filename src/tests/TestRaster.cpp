#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include "raster/ray.hpp"
using std::cout;
using std::endl;
using std::ofstream;
using std::sqrt;
using std::vector;


#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType = 0x4D42;
    uint32_t bfSize;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;
};
struct BMPInfoHeader {
    uint32_t biSize = 40;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter = 0;
    int32_t biYPelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};
#pragma pack(pop)

float hit_sphere(const vec3 &center, float radius, const ray &r) {
    const vec3 oc = r.origin() - center;
    const float a = dot(r.direction(), r.direction());
    const float b = 2.0 * dot(oc, r.direction());
    const float c = dot(oc, oc) - radius * radius;
    const float discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        return static_cast<float>((-b - std::sqrt(discriminant)) / (2.0 * a));
    }
    return -1.0;
}

vec3 color(const ray &r) {
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit(r.point_at_parameter(t) - vec3(0, 0, -1));
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    vec3 unit_direction = unit(r.direction());
    t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}


int main() {
    int nx = 200;
    int ny = 100;
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

    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            float u = static_cast<float>(i) / static_cast<float>(nx);
            float v = static_cast<float>(j) / static_cast<float>(ny);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 col = color(r);
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
