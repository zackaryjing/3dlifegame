#pragma once
#include <fstream>
#include <iostream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
using std::string;


#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
struct BMPInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

class BmpFile {
public:
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    int bytesPerPixel;
    int rowSize;
    vector<uint8_t> pixels;
};

class BmpLoader {
public:
    static BmpFile load(string filePath) {
        ifstream file(filePath, std::ios::binary);
        if (!file) {
            cerr << "Cannot open " << filePath << endl;
            throw std::invalid_argument("file path error");
        }
        BmpFile bmpFile;
        file.read(reinterpret_cast<char *>(&bmpFile.fileHeader),
                  sizeof(bmpFile.fileHeader));
        file.read(reinterpret_cast<char *>(&bmpFile.infoHeader),
                  sizeof(bmpFile.infoHeader));
        auto width = bmpFile.infoHeader.biWidth;
        auto height = bmpFile.infoHeader.biHeight;
        auto bitCount = bmpFile.infoHeader.biBitCount;
        bmpFile.bytesPerPixel = bitCount / 8;

        bmpFile.rowSize = (bitCount * width + 31) / 32 * 4;

        bmpFile.pixels = vector<uint8_t>(bmpFile.rowSize * height);
        file.seekg(bmpFile.fileHeader.bfOffBits);

        for (int y = height - 1; y >= 0; --y) {
            file.read(reinterpret_cast<char *>(bmpFile.pixels.data() +
                                               y * bmpFile.rowSize),
                      bmpFile.rowSize);
        }

        file.close();
        return bmpFile;
    }
};
