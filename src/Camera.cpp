#include "Camera.hpp"

Camera::Camera(Point3 lookfrom,
               Point3 lookat,
               Vec3   vup,
               double vfov,
               double aspect_ratio,
               double aperture,
               double focus_dist
              )
{
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom-lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    mOrigin = lookfrom;
    mHorizontal = focus_dist * viewport_width * u;
    mVertical = focus_dist * viewport_height * v;
    mLowerLeftCorner = mOrigin - mHorizontal/2 - mVertical/2 - focus_dist*w;

    mLensRadius = aperture / 2;
}

Ray Camera::get_Ray(double s, double t) const {
    Vec3 rd = mLensRadius * random_in_unit_disk();
    Vec3 offset = u * rd.x() + v * rd.y();

    return Ray(mOrigin + offset, mLowerLeftCorner + s * mHorizontal + t * mVertical - mOrigin - offset);
}