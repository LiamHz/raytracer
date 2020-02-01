#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "ray.h"

// Write a ppm image file with a background, and a sphere using ray tracing

vec3 color(const ray& r, hitable *world) {

    hit_record rec;

    // If a ray from the origin hits a hitable object, return the normal
    // Represented by colors
    if (world->hit(r, 0.0, MAXFLOAT, rec)) {
        return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    }

    // If a ray hits nothing, blend white and blue based on the ray's y coord
    else {
        // Turn ray into a unit vector. This makes -1.0 < y < 1.0
        vec3 unit_direction = unit_vector(r.direction());

        // Scale ray to 0.0 < t < 1.0
        float t = 0.5*(unit_direction.y() + 1.0);

        // Return a linear interpolation (lerp) between
        // blue (t=1.0) and white (t=0.0)
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
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

    // Create a list of hitable objects
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0,-100.5,-1), 100);
    hitable *world = new hitable_list(list,2);

    // Write pixels out in rows from left to right (int i)
    // Write rows from top to bottom (int j)
    // Set r, g, and b to values between 0.0 and 1.0
    // Convert from 0 to 1 float range to 0 to 256 int range
    // Write RGB triplet to file
    for (int j= ny-1; j>= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            // Create a ray from the origin to the point through current pixel
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r, world);

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }

    ofs.close();
}
