#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
    public:
        sphere(point3 cen, double r, shared_ptr<IMaterial> m);

        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

        point3 center;
        double radius;
        shared_ptr<IMaterial> mat_ptr;
};

#endif