#pragma once

#include <iomanip>
#include <tuple>
#include <vector>
using std::tuple;
using std::vector;

namespace Type {
    typedef tuple<float, float, float> FVec3;
    typedef tuple<int, int, int> IVec3;
    typedef vector<FVec3> VFVec3;
    typedef vector<IVec3> VIVec3;
    typedef vector<tuple<float, float>> VFVec2;
} // namespace Type


inline Type::FVec3 substract(const Type::FVec3 &first,
                             const Type::FVec3 &second) {
    return {get<0>(first) - get<0>(second), get<1>(first) - get<1>(second),
            get<2>(first) - get<2>(second)};
}

inline Type::FVec3 getAvgCenter(std::initializer_list<Type::FVec3> vertices) {
    size_t n = vertices.size();
    float finalx = 0.0f, finaly = 0.0f, finalz = 0.0f;
    for (auto [x, y, z]: vertices) {
        finalx += x, finaly += y, finalz += z;
    }

    return {finalx / n, finaly / n, finalz / n};
}

inline Type::FVec3 getAvgCenter(Type::VFVec3 &vertices) {
    size_t n = vertices.size();
    float finalx = 0.0f, finaly = 0.0f, finalz = 0.0f;
    for (auto [x, y, z]: vertices) {
        finalx += x, finaly += y, finalz += z;
    }

    return {finalx / n, finaly / n, finalz / n};
}

inline Type::FVec3 normalize(Type::FVec3 vec) {
    auto [x, y, z] = vec;
    float norm = sqrt(x * x + y * y + z * z);
    return {x / norm, y / norm, z / norm};
}


inline void showVFVec3(Type::VFVec3 vecs) {
    std::cout << std::fixed << std::setprecision(2);
    for (auto vec: vecs) {
        auto [x, y, z] = vec;
        cout << x << "\t " << y << "\t" << z << endl;
    }
    cout << endl;
}
