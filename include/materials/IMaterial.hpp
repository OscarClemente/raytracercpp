#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "rtweekend.hpp"
#include "hittable.hpp"

class IMaterial {
    public:
        virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

#endif