#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "model/Model.hpp"
#include "utils/Node.hpp"
#include "utils/Type.hpp"

#ifndef MODEL_DIR
#define MODEL_DIR "../../assets/model/"
#endif

using std::ifstream;
using std::list;
using std::stof;
using std::stoi;
using std::string;
using std::tuple;
using std::vector;


enum class SplitMode {
    SkipSameSep, // count multiple continuous sep as one
    KeepSameSep, // count continuous sep as single one
};

class ModelLoader {
private:
public:
    static inline void init() {}
    static inline tuple<VFVec3, VFVec3, VFVec2, vector<VIVec3>, string>
    loadRawData(string &path) {
        ifstream file(path);
        string line;
        VFVec3 vertices_raw;
        VFVec3 normals_raw;
        VFVec2 texture_coord_raw;
        vector<VIVec3> faces;
        string material;
        // Caution: order matters.
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (line.starts_with("mtllib")) {
                material = split(line, " ")[1];
            } else if (line.starts_with("vn")) {
                auto normal = split(line, " ");
                normals_raw.emplace_back(stof(normal[1]), stof(normal[2]),
                                         stof(normal[3]));
            } else if (line.starts_with("vt")) {
                auto coord = split(line, " ");
                texture_coord_raw.emplace_back(stof(coord[1]), stof(coord[2]));
            } else if (line.starts_with("v")) {
                auto vertex = split(line, " ");
                vertices_raw.emplace_back(stof(vertex[1]), stof(vertex[2]),
                                          stof(vertex[3]));
            } else if (line.starts_with("f")) {
                auto face_indices = split(line, " ");
                VIVec3 face;
                for (size_t i = 1; i < face_indices.size(); ++i) {
                    auto indices =
                            split(face_indices[i], "/", SplitMode::KeepSameSep);
                    face.emplace_back(stoi(indices[0]), stoi(indices[1]),
                                      stoi(indices[2]));
                }
                faces.push_back(face);
            }
        }
        auto test_output = path + "_test";
        writeToModel(test_output, vertices_raw, normals_raw, texture_coord_raw,
                     faces);
        return {vertices_raw, normals_raw, texture_coord_raw, faces, material};
    }

    static inline void writeToModel(string &path, VFVec3 vertices_raw,
                                    VFVec3 normals_raw,
                                    VFVec2 texture_coord_raw,
                                    vector<VIVec3> faces) {
        std::ofstream file(path);
        for (auto [vx, vy, vz]: vertices_raw) {
            file << "v " << vx << " " << vy << " " << vz << endl;
        }
        for (auto [tx, ty]: texture_coord_raw) {
            file << "vt " << tx << " " << ty << endl;
        }
        for (auto [nx, ny, nz]: normals_raw) {
            file << "vn " << nx << " " << ny << " " << nz << endl;
        }
        for (auto face: faces) {
            file << "f";
            for (auto [vi, ti, ni]: face) {
                file << " " << vi << "/" << ti << "/" << ni;
            }
            file << endl;
        }
        file.close();
    }

    /**
     * Targeting on normal obj file.
     */
    static inline Model loadModel(string &path) {
        auto [vertices_raw, normals_raw, texture_coord_raw, faces,
              material_name] =
                loadRawData(path);

        VFVec3 vertices;
        VFVec3 normals;
        VFVec2 texture_coord;
        // TODO: this only works when the polygon is convex
        for (auto face: faces) {
            if (face.size() < 3) {
                throw std::length_error("Face must have more than 2 vertex");
            } else if (face.size() == 3) {
                for (int i = 0; i < 3; ++i) {
                    auto v_index = get<0>(face[i]) - 1;
                    auto t_index = get<1>(face[i]) - 1;
                    auto n_index = get<2>(face[i]) - 1;
                    vertices.push_back(vertices_raw[v_index]);
                    normals.push_back(normals_raw[n_index]);
                    texture_coord.push_back(texture_coord_raw[t_index]);
                }
            } else if (face.size() == 4) {
                for (auto i: {0, 1, 2, 2, 3, 0}) {
                    auto v_index = get<0>(face[i]) - 1;
                    auto t_index = get<1>(face[i]) - 1;
                    auto n_index = get<2>(face[i]) - 1;
                    vertices.push_back(vertices_raw[v_index]);
                    normals.push_back(normals_raw[n_index]);
                    texture_coord.push_back(texture_coord_raw[t_index]);
                }
            } else {
                size_t m = face.size();
                auto available = Node<int>::with_capacity(m);

                auto cur = available;
                while (m > 2) {
                    shared_ptr<Node<int>> mid;
                    for (int i = 0; i < 3; ++i) {
                        if (i == 1) {
                            mid = cur;
                        }
                        auto f_index = cur->val;
                        auto v_index = get<0>(face[f_index]) - 1;
                        auto t_index = get<1>(face[f_index]) - 1;
                        auto n_index = get<2>(face[f_index]) - 1;
                        vertices.push_back(vertices_raw[v_index]);
                        normals.push_back(normals_raw[n_index]);
                        texture_coord.push_back(texture_coord_raw[t_index]);
                        cur = cur->next;
                    }
                    mid->detach();
                    m--;
                }
            }
        }
        return Model::genModel(vertices, normals, texture_coord, material_name);
    };


    static inline vector<string>
    split(const string &str, const string &separator,
          SplitMode splitMode = SplitMode::SkipSameSep) {
        if (separator.empty()) {
            throw std::invalid_argument("Seperator can't be empty");
        }
        vector<string> res;
        size_t m = separator.size();
        size_t n = str.size();
        if (m == 1) {
            char sep = separator[0];
            size_t base = 0;
            for (size_t i = 0; i < n; ++i) {
                if (str[i] == sep) {
                    if (i > base) {
                        res.push_back(str.substr(base, i - base));
                    } else if (splitMode == SplitMode::KeepSameSep) {
                        res.push_back("");
                    }
                    base = i + 1;
                }
            }
            if (str[n - 1] != sep) {
                res.push_back(str.substr(base, n - base));
            } else if (splitMode == SplitMode::KeepSameSep) {
                res.push_back("");
            }
        } else {
            size_t base = 0;
            while (base < n) {
                size_t pos = str.find(separator, base);
                if (pos == string::npos) {
                    break;
                }
                if (pos > base) {
                    res.push_back(str.substr(base, pos - base));
                } else if (splitMode == SplitMode::KeepSameSep) {
                    res.push_back("");
                }
                base = pos + m;
            }
            if (base < n) {
                res.push_back(str.substr(base, n - base));
            } else if (splitMode == SplitMode::KeepSameSep) {
                res.push_back("");
            }
        }
        return res;
    }
};
