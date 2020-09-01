#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "IMaterial.hpp"
#include "rtweekend.hpp"
#include "hittable.hpp"

class Dielectric : public IMaterial {
    public:
        Dielectric(double ri);

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

    private:
        double ref_idx;
        double schlick(double cosine, double ref_idx) const;
};

#endif