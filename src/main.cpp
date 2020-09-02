#include "RTAux.hpp"
#include "Color.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "materials/IMaterial.hpp"
#include "materials/Lambertian.hpp"
#include "materials/Metal.hpp"
#include "materials/Dielectric.hpp"

#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include <sstream>

Color rayColor(const Ray& r, const Hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
    {
        return Color(0, 0, 0);
    }
    if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth-1);
        return Color(0, 0, 0);
    }
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<IMaterial> Sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    Sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    Sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else {
                    // glass
                    Sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

void f1(int id, int nthread, std::shared_ptr<std::stringstream> ss)
{
    for (int i = 0; i < 100; i++)
    {
        (*ss) << id << " out of " << nthread << " threads | " << i << std::endl;
    }
}
/*
int main()
{
    std::cout << "Start" << std::endl;
    int nthreads = 4;
    std::vector<std::shared_ptr<std::stringstream>> ssVector;
    std::vector<std::thread> tVector;

    for (int i = 0; i < nthreads; i++)
    {
        std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
        ssVector.push_back(ss);
        std::thread t(f1, i, nthreads, ss);
        tVector.push_back(std::move(t));
    }

    for (int i = 0; i < nthreads; i++)
    {
        tVector.at(i).join();
        std::cout << ssVector.at(i)->str();
    }

    std::cout << "End" << std::endl;
}
*/
struct RenderData
{
    Camera* cam;
    HittableList* world;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;
};

void renderPixels(int threadId, std::shared_ptr<std::stringstream> ss, RenderData renderData, int fromLine, int toLine)
{
    for (int j = fromLine - 1; j >= toLine; --j)
    {
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        //std::cerr << "Thread: " << threadId << " Scanlines remaining: " << j << std::endl;
        for (int i = 0; i < renderData.image_width; ++i)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < renderData.samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (renderData.image_width-1);
                auto v = (j + random_double()) / (renderData.image_height-1);
                Ray r = renderData.cam->get_Ray(u, v);
                pixel_color += rayColor(r, *(renderData.world), renderData.max_depth);
            }
            writeColor((*ss), pixel_color, renderData.samples_per_pixel);
        }       
    }
}

int main()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 20;

    // World
    
    auto R = cos(pi/4);
    HittableList world;

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.0);
    auto material_right  = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.3);

    world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    //auto world = random_scene();
    // Camera
    Point3 lookfrom(0, 1, 4);
    Point3 lookat(0, 0, -1);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 5.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Rendering data in a struct to bundle them up a bit
    RenderData renderData;
    renderData.cam = &cam;
    renderData.world = &world;
    renderData.image_width = image_width;
    renderData.image_height = image_height;
    renderData.samples_per_pixel = samples_per_pixel;
    renderData.max_depth = max_depth;

    // image_height, image_width, samples_per_pixel, camera, world, max_depth);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int nThreads = 6;

    std::cerr << "Rendering with " << nThreads << " threads." << std::endl;

    std::vector<std::shared_ptr<std::stringstream>> ssVector;
    std::vector<std::thread> threadVector;

    for (int i = 0; i < nThreads; i++)
    {
        std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
        ssVector.push_back(ss);

        int step = renderData.image_height / nThreads;
        int fromLine = renderData.image_height - (step * i);
        int toLine = renderData.image_height - (step * (i + 1));

        std::thread t(renderPixels, i, ss, renderData, fromLine, toLine);
        threadVector.push_back(std::move(t));
    }

    for (int i = 0; i < nThreads; i++)
    {
        threadVector.at(i).join();
        std::cout << ssVector.at(i)->str();
    }

    std::cout << "End" << std::endl;
    std::cerr << "\nDone.\n";
}