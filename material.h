#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"
#include "ray.h"

struct hit_record;

vec3 reflect(const vec3& v, const vec3& normal) {
    return v - 2 * dot(v, normal) * normal;
}

// Used to create diffuse materials
vec3 random_in_unit_sphere() {
    vec3 p;
    // Select a random point in a unit cube
    // Repeat until that point is also in unit sphere
    // Point is in unit sphere if squared length is less than 1.0
    do {
        p = 2.0 * vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    // Albedo is the fraction of light reflected from the material
    vec3 albedo;
};

class metal : public material {
public:
    metal(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
};

#endif

