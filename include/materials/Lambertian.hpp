#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "IMaterial.hpp"
#include "rtweekend.hpp"
#include "hittable.hpp"

class Lambertian: public IMaterial {
    public:
        Lambertian(const color& a);

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

    private:
        color albedo;
};

#endif