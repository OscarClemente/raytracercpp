#include "materials/Metal.hpp"

Metal::Metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1)
{
}

bool Metal::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
{
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}