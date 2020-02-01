#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, float r) : center(cen), radius(r) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
};

// The equation for a sphere  at (cx, cy, cz) is
// dot((p - c), (p - c)) = r*r
// Where c is the center of the sphere, p is a point on the sphere,
// and r is the radius of the sphere

// Rays that hit the sphere, satisfy the equation
// dot((p(t) - c), (p(t) - c)) = r*r
// Which is equivalent to
// dot(A + t*B - c), (A + t*B - c)) = r*r
// Where A is the origin of the ray, t is its length, and B is its direction

// This expands to the quadratic
// t*t*dot(B, B) + 2*t*dot(B, A - C) + dot(A - C, A - C) - r*r = 0
// For this quadratic, the number of positive roots indicates collisions
// 0 roots has no collion
// 1 root has 1 collision
// 2 roots has 2 collisions
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;

    // If there's a ray collision, discrimant > 0
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c))/a;
        // Only "count" the ray hit if tmin < t < tmax
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        // Check the other sign of the sqrt
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}

#endif
