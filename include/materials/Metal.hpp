#ifndef METAL_HPP
#define METAL_HPP

#include "IMaterial.hpp"
#include "RTAux.hpp"
#include "Hittable.hpp"

class Metal : public IMaterial {
    public:
        Metal(const Color& a, double f);

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;

    private:
        Color mAlbedo;
        double mFuzz;
};

#endif