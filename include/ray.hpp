#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
    public:
        ray() = default;
        ray(const point3& origin, const vec3& direction);
        
        point3 origin() const;
        vec3 direction() const;

        point3 at(double t) const;

        point3 orig;
        vec3 dir;
};

#endif