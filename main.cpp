#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "ray.h"
#include "camera.h"

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
    int ns = 50;

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

    // Create a list of hitable objects
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0,-100.5,-1), 100);
    hitable *world = new hitable_list(list,2);

    // Instantiate camera
    camera cam;

    // Write pixels out in rows from left to right (int i)
    // Write rows from top to bottom (int j)
    // Set r, g, and b to values between 0.0 and 1.0
    // Convert from 0 to 1 float range to 0 to 256 int range
    // Write RGB triplet to file
    for (int j= ny-1; j>= 0; j--) {
        for (int i = 0; i < nx; i++) {

            // Multisample Antialiasing (MSAA)
            // Send ns samples through each pixel, with the direction of each
            // ray slightly randomized. The pixel takes the average color of
            // these sample rays. This blends the foreground and background on
            // edge pixels.
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }

            col /= float(ns);

            // Scale pixel values from float 0 to 1, to int 0 to 256
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }

    ofs.close();
}
