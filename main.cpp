#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

// Write a ppm image file with a background, and a sphere using ray tracing

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;

    // If a ray from the origin hits a hitable object, return the normal
    // Represented by colors

    // Setting t_min to 0.001 (instead of 0) prevents shadow acne
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
             return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0 , 0);
        }
    }

    // If a ray hits nothing, blend white and blue based on the ray's y coord
    else {
        // Turn ray into a unit vector. This makes -1.0 < y < 1.0
        vec3 unit_direction = unit_vector(r.direction());

        // Scale ray to 0.0 < t < 1.0
        float t = 0.5 * (unit_direction.y() + 1.0);

        // Return a linear interpolation (lerp) between
        // blue (t=1.0) and white (t=0.0)
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    // Set the width and height of canvas
    int nx = 480;
    int ny = 360;
    int ns = 10;

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
    hitable *list[5];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list,5);

    // Instantiate camera
    camera cam;

    // Write pixels out in rows from left to right (int i)
    // Write rows from top to bottom (int j)
    // Set r, g, and b to values between 0.0 and 1.0
    // Convert from 0 to 1 float range to 0 to 256 int range
    // Write RGB triplet to file
    for (int j = ny-1; j>= 0; j--) {
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
                col += color(r, world, 0);
            }

            col /= float(ns);

            // Gamma correct pixel values
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

            // Scale pixel values from float 0 to 1, to int 0 to 256
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }

    ofs.close();
}
