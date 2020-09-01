#ifndef METAL_HPP
#define METAL_HPP

#include "IMaterial.hpp"
#include "rtweekend.hpp"
#include "hittable.hpp"

class Metal : public IMaterial {
    public:
        Metal(const color& a, double f);

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

    private:
        color albedo;
        double fuzz;
};

#endif