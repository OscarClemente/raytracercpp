#include "Sphere.hpp"

Sphere::Sphere(Point3 cen, double r, shared_ptr<IMaterial> m) 
        : mCenter(cen), mRadius(r), mMatPtr(m)
{  
}

bool Sphere::hit(const Ray& r, double tmin, double tmax, hit_record& rec) const {
    Vec3 oc = r.origin() - mCenter;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - mRadius*mRadius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            Vec3 outward_normal = (rec.p - mCenter) / mRadius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mMatPtr;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - mCenter) / mRadius;
            Vec3 outward_normal = (rec.p - mCenter) / mRadius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mMatPtr;
            return true;
        }
    }

    return false;
}