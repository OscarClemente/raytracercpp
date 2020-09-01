#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

class Ray {
    public:
        Ray() = default;
        Ray(const Point3& origin, const Vec3& direction);
        
        Point3 origin() const;
        Vec3 direction() const;

        Point3 at(double t) const;

    private:
        Point3 mOrig;
        Vec3 mDir;
};

#endif