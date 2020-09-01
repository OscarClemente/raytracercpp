#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "IMaterial.hpp"
#include "RTAux.hpp"
#include "Hittable.hpp"

class Dielectric : public IMaterial {
    public:
        Dielectric(double ri);

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;

    private:
        double mRefIdx;
        double schlick(double cosine, double ref_idx) const;
};

#endif