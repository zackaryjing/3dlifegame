#include <fstream>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::ofstream;
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

int main() {
    int nx = 200;
    int ny = 100;
    ofstream file("./out.bmp",std::ios::binary);
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

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float r = static_cast<float>(i) / static_cast<float>(nx);
            float g = static_cast<float>(j) / static_cast<float>(ny);
            float b = 0.2f;
            auto ir = static_cast<uint8_t>(255.99 * r);
            auto ig = static_cast<uint8_t>(255.99 * g);
            auto ib = static_cast<uint8_t>(255.99 * b);
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
