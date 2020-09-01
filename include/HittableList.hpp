#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "Hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
    public:
        HittableList() = default;
        HittableList(shared_ptr<Hittable> object);

        void clear();
        void add(shared_ptr<Hittable> object);

        virtual bool hit(const Ray& r, double tmin, double tmax, hit_record& rec) const override;

    private:
        std::vector<shared_ptr<Hittable>> mObjects;
};

#endif