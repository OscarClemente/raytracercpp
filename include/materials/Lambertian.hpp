#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "IMaterial.hpp"
#include "RTAux.hpp"
#include "Hittable.hpp"

class Lambertian: public IMaterial {
    public:
        Lambertian(const Color& a);

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;

    private:
        Color albedo;
};

#endif