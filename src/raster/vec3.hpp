#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>

class vec3 {
    float e[3]{};

public:
    vec3() = default;
    vec3(const float e0, const float e1, const float e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }
    [[nodiscard]] float x() const { return e[0]; }
    [[nodiscard]] float y() const { return e[1]; }
    [[nodiscard]] float z() const { return e[2]; }
    [[nodiscard]] float r() const { return e[0]; }
    [[nodiscard]] float g() const { return e[1]; }
    [[nodiscard]] float b() const { return e[2]; }
    const vec3 &operator+() const { return *this; }
    vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    float operator[](const int i) const { return e[i]; }
    float &operator[](const int i) { return e[i]; }
    vec3 &operator+=(const vec3 &v2) {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        e[2] += v2.e[2];
        return *this;
    }
    vec3 &operator-=(const vec3 &v2) {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        e[2] -= v2.e[2];
        return *this;
    };
    vec3 &operator*=(const vec3 &v2) {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        e[2] *= v2.e[2];
        return *this;
    };
    vec3 &operator/=(const float t) {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    };
    vec3 &operator*=(const float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    };

    vec3 operator+(const vec3 &v2) const {
        return {e[0] + v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]};
    }
    vec3 operator-(const vec3 &v2) const {
        return {e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]};
    }
    vec3 operator*(const vec3 &v2) const {
        return {e[0] * v2.e[0], e[1] * v2.e[1], e[2] * v2.e[2]};
    }
    vec3 operator/(const vec3 &v2) const {
        return {e[0] / v2.e[0], e[1] / v2.e[1], e[2] / v2.e[2]};
    }

    vec3 operator/(const float t) const {
        return {e[0] / t, e[1] / t, e[2] / t};
    }

    vec3 operator*(const float t) const {
        return {e[0] * t, e[1] * t, e[2] * t};
    }
    friend float dot(const vec3 &v1, const vec3 &v2) {
        return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
    }
    friend vec3 cross(const vec3 &v1, const vec3 &v2) {
        return {
                v1.e[1] * v2[2] - v1.e[2] * v2[1],
                v1.e[2] * v2[0] - v1.e[0] * v2[2],
                v1.e[0] * v2[1] - v1.e[1] * v2[0],
        };
    }
    friend vec3 unit(const vec3 &v) { return v / v.length(); }

    friend vec3 operator/(const float t, const vec3 v) { return v / t; }

    friend vec3 operator*(const float t, const vec3 v) { return v * t; }

    friend std::ostream &operator<<(std::ostream &os, const vec3 &v) {
        os << v.x() << ", " << v.y() << ", " << v.z();
        return os;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
    [[nodiscard]] float squared_length() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};
