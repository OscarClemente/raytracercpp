#include "HittableList.hpp"

HittableList::HittableList(shared_ptr<Hittable> object)
{
    add(object);
}

bool HittableList::hit(const Ray& r, double tmin, double tmax, hit_record& rec) const {
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

void HittableList::clear()
{
    objects.clear();
}

void HittableList::add(shared_ptr<Hittable> object)
{
    objects.push_back(object);
}