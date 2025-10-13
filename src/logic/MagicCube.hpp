#pragma once
#include <unordered_map>
#include <vector>
#include "group/Group.hpp"
#include "model/Model.hpp"
using std::tuple;
using std::unordered_map;
using std::vector;
class MagicCube {
protected:
    string vShaderPath = GLSL_DIR "MagicCubeShader.vs";
    string fShaderPath = GLSL_DIR "MagicCubeShader.fs";

public:
    vector<shared_ptr<Model>> cubes;

    vector<vector<int>> cubeFaceIndex;
    vector<vector<int>> cubeIndex;
    // unordered_map<string, >
    // L R U D F B
    vector<tuple<int, int, int>> position;
    MagicCube() {
        cubes.reserve(26);
        for (int i = 0; i < 26; ++i) {
            cubes.push_back(make_shared<Model>(Model::getCube()));
        }
        // top bottom left right front back
        cubeFaceIndex = {
                {30, 31, 32, 33, 34, 35}, {24, 25, 26, 27, 28, 29},
                {12, 13, 14, 15, 16, 17}, {18, 19, 20, 21, 22, 23},
                {0, 1, 2, 3, 4, 5},       {6, 7, 8, 9, 10, 11},
        };
        cubeIndex = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                     {17, 18, 19, 20, 21, 22, 23, 24, 25},
                     {0, 3, 6, 9, 12, 14, 17, 20, 23},
                     {2, 5, 8, 11, 13, 16, 19, 22, 25},
                     {6, 7, 8, 14, 15, 16, 23, 24, 25},
                     {0, 1, 2, 9, 10, 11, 17, 18, 19}};
        position = {
                {-1, 1, -1},  {0, 1, -1},  {1, 1, -1}, //
                {-1, 1, 0},   {0, 1, 0},   {1, 1, 0}, //
                {-1, 1, 1},   {0, 1, 1},   {1, 1, 1}, //
                {-1, 0, -1},  {0, 0, -1},  {1, 0, -1}, //
                {-1, 0, 0},   {1, 0, 0}, //
                {-1, 0, 1},   {0, 0, 1},   {1, 0, 1}, //
                {-1, -1, -1}, {0, -1, -1}, {1, -1, -1}, //
                {-1, -1, 0},  {0, -1, 0},  {1, -1, 0}, //
                {-1, -1, 1},  {0, -1, 1},  {1, -1, 1}, //
        };
        setColor();
        setModelMat();
    }

    Group toGroup() const {
        auto instance = Group(vShaderPath, fShaderPath);
        instance.addModels(cubes);
        return instance;
    }

    void setColor() const {
        const vector colors = {0.67f, 0.84f, 0.01f, 0.34f, 0.17f, 0.51f};
        for (auto &cube: cubes) {
            cube->texture_coord = vector(72, 1.01f);
        }
        for (int face = 0; face < 6; ++face) {
            for (const auto &cube_index: cubeIndex[face]) {
                for (const auto &face_index: cubeFaceIndex[face]) {
                    cubes[cube_index]->texture_coord[face_index * 2] =
                            colors[face];
                }
            }
        }
    }


    void setModelMat() const {
        auto tuple2vec3 = [](const tuple<int, int, int> &t) {
            return glm::vec3(get<0>(t), get<1>(t), get<2>(t));
        };
        for (int i = 0; i < 26; ++i) {
            cubes[i]->modelMat =
                    glm::translate(cubes[i]->modelMat, tuple2vec3(position[i]));
            cubes[i]->modelMat = glm::scale(cubes[i]->modelMat,
                                            glm::vec3(0.95f, 0.95f, 0.95f));
        }
    }

    void turn(const string &op) {
        auto [axis, mask] = getAxisMask(op[0]);
        int direction = 0;
        if (op.size() == 2) {
            if (op[1] == 2) {
                direction = 2;
            } else {
                direction = 1;
            }
        }
        if (axis < 0) {
            axis = -axis;
            direction ^= 1;
        }
        applyOperation(axis, direction, mask);
    }


    void applyOperation(const int axis, const int direction, const uint mask) {
        // axis: 1:x 2:y 3:z
        // direction: 0:90 1:-90 2:180
        for (int i = 0; i < 26; ++i) {
            if (mask >> i & 1) {
                auto [x, y, z] = position[i];
                switch (axis) {
                    case 1:
                        switch (direction) {
                            case 0:
                                position[i] = {x, -z, y};
                                break;
                            case 1:
                                position[i] = {x, z, -y};
                                break;
                            default:
                                position[i] = {x, -y, -z};
                        }
                        break;
                    case 2:
                        switch (direction) {
                            case 0:
                                position[i] = {z, y, -x};
                                break;
                            case 1:
                                position[i] = {-z, y, x};
                                break;
                            default:
                                position[i] = {-x, y, -z};
                        }
                        break;
                    default:
                        switch (direction) {
                            case 0:
                                position[i] = {-y, x, z};
                                break;
                            case 1:
                                position[i] = {y, -x, z};
                                break;
                            default:
                                position[i] = {-x, -y, z};
                        }
                }
            }
        }
    }

    pair<int, uint> getAxisMask(const char c) const {
        uint mask = 0;
        int axis = 0;
        switch (c) {
            case 'U': {
                for (int i = 0; i < 26; ++i) {
                    if (get<1>(position[i]) == 1) {
                        mask |= 1 << i;
                        axis = 2;
                    }
                }
                break;
            }
            case 'D': {
                for (int i = 0; i < 26; ++i) {
                    if (get<1>(position[i]) == -1) {
                        mask |= 1 << i;
                        axis = -2;
                    }
                }
                break;
            }
            case 'L': {
                for (int i = 0; i < 26; ++i) {
                    if (get<0>(position[i]) == -1) {
                        mask |= 1 << i;
                        axis = -1;
                    }
                }
                break;
            }
            case 'R': {
                for (int i = 0; i < 26; ++i) {
                    if (get<0>(position[i]) == 1) {
                        mask |= 1 << i;
                        axis = 1;
                    }
                }
                break;
            }
            case 'F': {
                for (int i = 0; i < 26; ++i) {
                    if (get<2>(position[i]) == 1) {
                        mask |= 1 << i;
                        axis = 3;
                    }
                }
                break;
            }
            case 'B': {
                for (int i = 0; i < 26; ++i) {
                    if (get<2>(position[i]) == -1) {
                        mask |= 1 << i;
                        axis = -3;
                    }
                }
                break;
            }
            case 'x': {
                axis = 1;
                mask = UINT_MAX;
                break;
            }
            case 'y': {
                axis = 2;
                mask = UINT_MAX;
                break;
            }
            case 'z': {
                axis = 3;
                mask = UINT_MAX;
                break;
            }
            default:
                break;
        }
        return {axis, mask};
    }
};
