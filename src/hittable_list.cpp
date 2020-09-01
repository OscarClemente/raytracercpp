#include "hittable_list.hpp"

hittable_list::hittable_list(shared_ptr<hittable> object)
{
    add(object);
}

bool hittable_list::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = tmax;

    for (const auto& object : objects) {
        if (object->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

void hittable_list::clear()
{
    objects.clear();
}

void hittable_list::add(shared_ptr<hittable> object)
{
    objects.push_back(object);
}