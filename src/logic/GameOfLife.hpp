#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;


class Layer {
public:
    static inline int width = 0;
    static inline int height = 0;
    int layerId;
    std::vector<std::vector<bool>> data;
    float ratio = 0.5;

    Layer(const int layerId) :
        layerId(layerId), data(vector(width, vector<bool>(height))) {}

    static void setSize(const int _width, const int _height) {
        width = _width;
        height = _height;
    }
    void randomInit() {
        const int limit = (int) (ratio * (float) RAND_MAX);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (const int num = rand(); num < limit) {
                    data[i][j] = true;
                }
            }
        }
    }

    vector<vector<bool>> next() {
        auto nxt = vector(height, vector<bool>(width));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                nxt[i][j] = check(i, j);
            }
        }
        return nxt;
    }

    bool check(const int y, const int x) {
        int cnt = 0;
        for (int i = height - 1; i <= height + 1; i++) {
            for (int j = width - 1; j <= width + 1; j++) {
                cnt += data[(y + i) % height][(x + j) % width];
            }
        }

        switch (cnt) {
            case 3:
                return true;
            case 4:
                return data[y][x];
            default:
                return false;
        }
    }

    void toModelMat(int index) {
        (void)index;
        vector<glm::mat4> res;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                res.push_back(glm::translate(glm::mat4(1.0f),
                                             glm::vec3(1.0f, 1.0f, 1.0f)));
            }
            cout << endl;
        }
    }

    void debug_output() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << (data[i][j] ? "#" : " ");
            }
            cout << endl;
        }
    }
};
