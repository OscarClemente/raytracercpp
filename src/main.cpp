#include "RTAux.hpp"
#include "Color.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "materials/IMaterial.hpp"
#include "materials/Lambertian.hpp"
#include "materials/Metal.hpp"
#include "materials/Dielectric.hpp"
#include "Scenes.hpp"

#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include <sstream>
#include <utility>
#include <mutex>
#include <chrono>

struct RenderChunk
{
    int state;
    std::shared_ptr<std::stringstream> ss;
    std::pair<int, int> ulPoint;
    std::pair<int, int> lrPoint;
};

struct RenderData
{
    Camera* cam;
    HittableList* world;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;
};

std::vector<std::shared_ptr<RenderChunk>> mChunkVector;
std::mutex mChunkMutex;
RenderData mRenderData;

void createRenderChunks(int vChunks)
{
    int vStep = mRenderData.image_height / vChunks;

    if (vChunks > mRenderData.image_height)
    {
        vChunks = mRenderData.image_height;
    }

    for (int i = 0; i < vChunks; ++i)
    {
        std::shared_ptr<RenderChunk> renderChunk = std::make_shared<RenderChunk>();
        std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();

        // Upper Left point
        int ulx = 0;
        int uly = (mRenderData.image_height - (vStep * i)) - 1;
        auto ulPoint = std::make_pair(ulx, uly);

        // Lower Right point
        int lrx = mRenderData.image_width - 1;
        int lry = mRenderData.image_height - (vStep * (i + 1));

        if (i == vChunks - 1)
        {
            lry = 0;
        }

        auto lrPoint = std::make_pair(lrx, lry);

        renderChunk->state = 0;
        renderChunk->ss = ss;
        renderChunk->ulPoint = ulPoint;
        renderChunk->lrPoint = lrPoint;

        mChunkVector.push_back(renderChunk);
    }
}

void renderChunks(int threadId)
{
    for (int cId = 0; cId < mChunkVector.size(); cId++)
    {
        mChunkMutex.lock();
        if (mChunkVector.at(cId)->state != 0)
        {
            // Check next chunk
            mChunkMutex.unlock();
            continue;
        }

        mChunkVector.at(cId)->state = 1; // rendering
        mChunkMutex.unlock();

        std::shared_ptr<RenderChunk> renderChunk = mChunkVector.at(cId);

        // todo: separate in another function
        for (int j = renderChunk->ulPoint.second; j >= renderChunk->lrPoint.second; j--)
        {
            for (int i = renderChunk->ulPoint.first; i <= renderChunk->lrPoint.first; i++)
            {
                Color pixel_color(0, 0, 0);
                for (int s = 0; s < mRenderData.samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (mRenderData.image_width-1);
                    auto v = (j + random_double()) / (mRenderData.image_height-1);
                    Ray r = mRenderData.cam->get_Ray(u, v);
                    pixel_color += rayColor(r, *(mRenderData.world), mRenderData.max_depth);
                }
                writeColor((*renderChunk->ss), pixel_color, mRenderData.samples_per_pixel);
            }
        }

        mChunkMutex.lock();
        renderChunk->state = 2; // finished rendering
        mChunkMutex.unlock();
    }
}

void createThreads(int nThreads, std::vector<std::thread> &threadVector)
{
    // Create the threads
    for (int i = 0; i < nThreads; i++)
    {
        std::thread t(renderChunks, i);
        threadVector.push_back(std::move(t));
    }
}

void writeRenderProgress()
{
    bool allChunksFinished = false;

    // While still rendering, output the state of each chunk
    while (!allChunksFinished)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        bool accumulatedChunksFinished = true;

        std::cerr << "\rWorking: ";

        for (int i = 0; i < mChunkVector.size(); i++)
        {
            mChunkMutex.lock();
            int chunkState = mChunkVector.at(i)->state;
            mChunkMutex.unlock();
            accumulatedChunksFinished &= chunkState == 2;
            std::cerr << chunkState;
        }
        std::cerr << std::flush;

        allChunksFinished = accumulatedChunksFinished;
    }
}

void joinThreads(std::vector<std::thread> &threadVector)
{
    // Make sure threads are finished
    for (int i = 0; i < threadVector.size(); i++)
    {
        threadVector.at(i).join();
    }
}

void writeOutputImage()
{
    // Write all stringstreams into cout to output the image
    for (int i = 0; i < mChunkVector.size(); i++)
    {
        std::cout << mChunkVector.at(i)->ss->str();
    }
}

int main()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 20;

    // World    
    auto world = create3BallsScene();

    // Camera
    Point3 lookfrom(0, 1, 4);
    Point3 lookat(0, 0, -1);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 5.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Rendering data in a struct to bundle them up a bit
    mRenderData.cam = &cam;
    mRenderData.world = &world;
    mRenderData.image_width = image_width;
    mRenderData.image_height = image_height;
    mRenderData.samples_per_pixel = samples_per_pixel;
    mRenderData.max_depth = max_depth;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int nThreads = 6;
    std::cerr << "Rendering with " << nThreads << " threads." << std::endl;

    // Define render chunks
    int vChunks = 64;
    createRenderChunks(vChunks);

    std::vector<std::thread> threadVector;
    createThreads(nThreads, threadVector);
    writeRenderProgress();
    joinThreads(threadVector);
    writeOutputImage();

    std::cout << "End" << std::endl;
    std::cerr << "\nDone.\n";
}