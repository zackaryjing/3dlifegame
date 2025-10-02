#pragma once

#include <iostream>

#include "raster/hitable.hpp"

using std::cerr;
using std::endl;

class hitable_list : public hitable {
public:
    hitable_list() = default;
    hitable_list(hitable **l, int n) { list = l, list_size = n; };
    bool hit(const ray &r, float tmin, float tmax,
             hit_record &rec) const override;

    bool bounding_box(float t0, float t1, aabb &box) const override;
    hitable **list;
    int list_size;
};


inline bool hitable_list::hit(const ray &r, float t_min, float t_max,
                       hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

inline bool hitable_list::bounding_box(float t0, float t1, aabb &box) const {
    aabb res;
    if (not list[0]->bounding_box(t0, t1, res)) {
        cerr << "hitable list has no bounding box" << endl;
        return false;
    }
    if (list_size > 1) {
        aabb temp;
        for (int i = 1; i < list_size; ++i) {
            if (not list[i]->bounding_box(t0, t1, temp)) {
                cerr << "hitable list element " << i << " has no bounding box "
                     << endl;
                res = surrounding_box(res, temp);
            }
        }
    }
    box = res;
    return true;
}
