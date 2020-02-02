#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:
    // vfov is top to bottom of camera view in degrees
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
        vec3 u, v, w;

        // The height and width of the camera view are determined by the vfov
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;

        // Define the location and orientation of the camera
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Define dimensions of the camera
        // Horizontal and vertical define the other corners of the canvas
        // relative to the lower_left_corner
        // lower_left_corner = vec3(-half_width, -half_height, -1.0);
        lower_left_corner = origin -half_width*u -half_height*v - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }
    ray get_ray(float u, float v) { return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin); }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif
