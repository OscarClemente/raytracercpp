#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "RTAux.hpp"
#include "Hittable.hpp"

class IMaterial {
    public:
        virtual bool scatter (
            const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const = 0;
};

#endif