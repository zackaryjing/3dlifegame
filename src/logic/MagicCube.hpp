#pragma once
#include <unordered_map>
#include <unordered_set>
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
    vector<tuple<int, int, int>> positionBackup;
    AnimationManager &animationManger;
    bool useAnimation = true;
    float curTime = 0.0f;
    float startTime = 0.0f;
    float duration = 1.0f;
    explicit MagicCube(AnimationManager &animationManger) :
        animationManger(animationManger) {
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
        positionBackup = position;
        setColor();
        setModelMat();
    }

    void reset() {
        position = positionBackup;
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
            cube->textureCoord = vector(72, 1.01f);
        }
        for (int face = 0; face < 6; ++face) {
            for (const auto &cube_index: cubeIndex[face]) {
                for (const auto &face_index: cubeFaceIndex[face]) {
                    cubes[cube_index]->textureCoord[face_index * 2] =
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
            auto modelMat = glm::mat4(1.0);
            modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));
            modelMat = glm::translate(modelMat, tuple2vec3(position[i]));
            modelMat = glm::scale(modelMat, glm::vec3(0.95f, 0.95f, 0.95f));
            cubes[i]->modelMat = modelMat;
        }
    }

    static bool checkValid(const string &op) {
        static std::pmr::unordered_set<string> operand = {
                "U",  "D",  "R",  "L",  "F",  "B", //
                "U'", "D'", "R'", "L'", "F'", "B'", //
                "U2", "D2", "R2", "L2", "F2", "B2", //
                "x",  "y",  "z", //
                "x'", "y'", "z'", //
                "x2", "y2", "z2"};
        return operand.contains(op);
    }

    bool executeInput(const string &input, string &msg, const float _curTime) {
        this->curTime = _curTime;
        startTime = curTime;
        for (auto &operation: Str::split(input, " ")) {
            if (not checkValid(operation)) {
                msg = std::format("{:} is not a valid operation", operation);
                return false;
            }
            turn(operation);
        }
        msg = "Action performed";
        return true;
    }

    void turn(const string &op) {
        auto [axis, mask] = getAxisMask(op[0]);
        int direction = 1;
        if (op.size() == 2) {
            if (op[1] == '2') {
                direction = 2;
            } else {
                direction = 0;
            }
        }
        if (axis < 0) {
            axis = -axis;
            if (direction != 2) {
                direction ^= 1;
            }
        }
        applyOperation(axis, direction, mask);
        turnModels(axis, direction, mask);
    }

    void turnModels(const int axis, const int direction, const uint mask) {
        float degree = 0;
        switch (direction) {
            case 0: {
                degree = 90;
                break;
            }
            case 1: {
                degree = -90;
                break;
            }
            default: {
                degree = 180;
                break;
            }
        }
        glm::vec3 rollAxis;
        switch (axis) {
            case 1: {
                rollAxis = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            }
            case 2: {
                rollAxis = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            }
            default: {
                rollAxis = glm::vec3(0.0f, 0.0f, 1.0f);
                break;
            }
        }
        if (useAnimation) {
            for (int i = 0; i < 26; ++i) {
                if (mask >> i & 1) {
                    animationManger.addAnimation(new RotateAnimation(
                            (degree == 180 ? duration : duration / 2),
                            startTime, false, rollAxis, degree,
                            cubes[i]->modelMat));
                }
            }
        } else {
            for (int i = 0; i < 26; ++i) {
                if (mask >> i & 1) {
                    cubes[i]->modelMat =
                            glm::rotate(glm::mat4(1.0f), glm::radians(degree),
                                        rollAxis) *
                            cubes[i]->modelMat;
                }
            }
        }
        startTime += (degree == 180 ? duration : duration / 2) + 0.1;
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

// switch top edge block
// random test: R F B2 U2 D F' R' L' F2 U2 R' D' B

// R L U' D F' R F' B'
// F2 U R' L F2 L' R U F2


// U R U' R' U' F' U F
// U' F' U F U R U' R'

// F R U R' U' F'
// L U L' U L U2 L'
// R U2 R' U' R U2 L' U R' U' L
// F2 U L R' F2 R L' U F2
