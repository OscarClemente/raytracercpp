#include "materials/Lambertian.hpp"

Lambertian::Lambertian(const Color& a): mAlbedo(a)
{
}

bool Lambertian::scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const
{
    Vec3 scatter_direction = rec.normal + random_unit_vector();
    scattered = Ray(rec.p, scatter_direction);
    attenuation = mAlbedo;
    return true;
}