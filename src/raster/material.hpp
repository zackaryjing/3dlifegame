#pragma once
#include "raster/hitable.hpp"
#include "raster/ray.hpp"
#include "raster/utils.hpp"

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scatterd) const = 0;
};

inline vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(Rand::gen_float(), Rand::gen_float(),
                       Rand::gen_float()) -
            vec3(1, 1, 1);
    } while (dot(p, p) >= 1.0);
    return p;
}


class metal : public material {
    vec3 albedo;
    float fuzz;

public:
    metal(const vec3 &a, const float f = 0.0f) : albedo(a) {
        if (f < 1.0f) {
            fuzz = f;
        } else {
            fuzz = 1.0f;
        }
    };
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        vec3 reflected = reflect(unit(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
};


class lambertian : public material {
    vec3 albedo;

public:
    explicit lambertian(const vec3 &a) : albedo(a) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
};


class dielectric : public material {
    float ref_idx;
    float schlick(float cosine, float ref_idx) const {
        float r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

public:
    dielectric(float ri) : ref_idx(ri) {};
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const override {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) /
                     r_in.direction().length();
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) /
                     r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
            scattered = ray(rec.p, refracted);
        } else {
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }
        if (Rand::gen_float() < reflect_prob) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, refracted);
        }
        return true;
    }
};
