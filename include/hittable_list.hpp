#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        hittable_list() = default;
        hittable_list(shared_ptr<hittable> object);

        void clear();
        void add(shared_ptr<hittable> object);

        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

        std::vector<shared_ptr<hittable>> objects;
};

#endif