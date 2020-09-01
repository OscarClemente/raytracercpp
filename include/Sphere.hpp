#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Hittable.hpp"
#include "Vec3.hpp"

class Sphere : public Hittable {
    public:
        Sphere(Point3 cen, double r, shared_ptr<IMaterial> m);

        virtual bool hit(const Ray& r, double tmin, double tmax, hit_record& rec) const override;

    private:
        Point3 center;
        double radius;
        shared_ptr<IMaterial> mat_ptr;
};

#endif