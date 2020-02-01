#include <iostream>
#include <fstream>
#include "ray.h"

// Write a ppm image file with a background, and a sphere using ray tracing

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
float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

vec3 color(const ray& r) {
    // Blend white and blue depending on the y coord of the ray

    // If the ray hits a sphere centered at (0, 0, -1) with radius 0.5
    // Return red for that pixel
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
        return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
    }

    // Turn ray into a unit vector. This makes -1.0 < y < 1.0
    vec3 unit_direction = unit_vector(r.direction());

    // Scale ray to 0.0 < t < 1.0
    t = 0.5*(unit_direction.y() + 1.0);

    // Return a linear interpolation (lerp) between
    // blue (t=1.0) and white (t=0.0)
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
    // Set the width and height of canvas
    int nx = 720;
    int ny = 480;

    // Create a ppm file to store the image data
    std::ofstream ofs;
    ofs.open("./out.ppm");

    // ppm is a image file format that can be defined with plain text

    // Example ppm file:
    // P3
    // 3 2
    // 255
    // 255    0   0
    // 0    255 255
    // ...

    // 1st line: P3 means colors are in ASCII
    // 2nd line: Define number of columns and rows
    // 3rd line: Max color
    // Nth line: RGB triplets

    ofs << "P3\n" << nx << " " << ny << "\n255\n";

    // Define dimensions of the canvas
    // Horizontal and vertical define the other corners of the canvas relative
    // to the lower_left_corner
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    // Write pixels out in rows from left to right (int i)
    // Write rows from top to bottom (int j)
    // Set r, g, and b to values between 0.0 and 1.0
    // Convert from 0 to 1 float range to 0 to 256 int range
    // Write RGB triplet to file
    for (int j= ny-1; j>= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }

    ofs.close();
}
