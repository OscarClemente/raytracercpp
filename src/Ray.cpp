#include "Ray.hpp"

Ray::Ray(const Point3& origin, const Vec3& direction)
        : mOrig(origin), mDir(direction)
{
}

Point3 Ray::origin() const 
{
    return mOrig;
}

Vec3 Ray::direction() const
{
    return mDir;
}

Point3 Ray::at(double t) const
{
    return mOrig + t * mDir;
}