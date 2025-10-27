#pragma once

#include "raster/aabb.hpp"
#include "raster/hitable.hpp"

class sphere final : public hitable {
public:
    sphere() = default;
    sphere(vec3 cen, float r, material *mat_ptr) :
        center(cen), radius(r), mat_ptr(mat_ptr) {};
    bool hit(const ray &r, float tmin, float tmax,
             hit_record &rec) const override;
    bool bounding_box(float t0, float t1, aabb &box) const override;
    vec3 center;
    float radius;
    material *mat_ptr;
};


inline void get_sphere_uv(const vec3 &p, float &u, float &v) {
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
}

inline bool sphere::hit(const ray &r, float t_min, float t_max,
                        hit_record &rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - std::sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return true;
        }
        temp = (-b + std::sqrt(b * b - a * c)) / a;
        if (temp < t_max && t_min < temp) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return true;
        }
    }
    return false;
}


inline bool sphere::bounding_box(float t0, float t1, aabb &box) const {
    (void) t0;
    (void) t1;
    box = aabb(center - vec3(radius, radius, radius),
               center + vec3(radius, radius, radius));
    return true;
}


class moving_sphere : public hitable {
public:
    moving_sphere() = default;
    moving_sphere(const vec3 cen0, const vec3 cen1, const float t0,
                  const float t1, const float r, material *m) :
        center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r),
        mat_ptr(m) {};
    virtual bool hit(const ray &r, float tmin, float tmax,
                     hit_record &rec) const override;
    virtual bool bounding_box(float t0, float t1, aabb &box) const override;
    vec3 center(float time) const;
    vec3 center0, center1;
    float time0, time1;
    float radius;
    material *mat_ptr;
};

inline vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}


inline bool moving_sphere::hit(const ray &r, float t_min, float t_max,
                               hit_record &rec) const {
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

inline bool moving_sphere::bounding_box(const float t0, const float t1,
                                        aabb &box) const {
    const auto box0 = aabb(center(t0) - vec3(radius, radius, radius),
                           center(t0) + vec3(radius, radius, radius));
    const auto box1 = aabb(center(t1) - vec3(radius, radius, radius),
                           center(t1) + vec3(radius, radius, radius));
    box = surrounding_box(box0, box1);
    return true;
}
