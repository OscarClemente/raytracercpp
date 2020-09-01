#include "materials/Lambertian.hpp"

Lambertian::Lambertian(const color& a): albedo(a)
{
}

bool Lambertian::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
{
    vec3 scatter_direction = rec.normal + random_unit_vector();
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}