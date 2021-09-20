//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

// #include "app.h"
#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../3d_graphics/ray_tracer.h"

static const size_t      WINDOW_WIDTH            = 1200;
static const size_t      WINDOW_HEIGHT           = 800;
static const char*       WINDOW_TITLE            = "Ray-tracing";
static const size_t      MAX_WINDOW_TITLE_LENGTH = 128;
static const Color       BACKGROUND_COLOR        = 0x2F'69'AA'FF; 

static const float       FOV                     = 0.78f; // Approx 45 degrees
static const float       ASPECT                  = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
static const float       NEAR                    = 5;
static const float       FAR                     = 600;
static const ViewFrustum VIEW_FRUSTUM            = {FOV, ASPECT, NEAR, FAR};

static const Vec3<float> LIGHT1_DIFFUSE          = {0.6f, 0.6f, 0.6f};
static const Vec3<float> LIGHT1_SPECULAR         = {0.9f, 0.9f, 0.9f};

static const Vec3<float> LIGHT2_DIFFUSE          = {0.6f, 0.6f, 1.0f};
static const Vec3<float> LIGHT2_SPECULAR         = {0.4f, 0.4f, 0.8f};

static const Vec3<float> AMBIENT_COLOR           = {0.5f, 0.5f, 0.5f};

static const Material    SPHERE_MATERIAL         = {{0.3f, 0.1f, 0.1f},
                                                    {0.7f, 0.4f, 0.4f},
                                                    {0.8f, 0.5f, 0.5f},
                                                    50};

void updateFpsTitle(Window& window, uint32_t frameTime);

int main()
{
    initGraphics();

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);

    /* ================ Entities ================ */
    Sphere sphere(&SPHERE_MATERIAL);
    sphere.setPos({25, 0, 0});
    sphere.setScale(5);

    /* ================ Scene ================ */
    Camera camera(VIEW_FRUSTUM);
    camera.setForward({1, 0, 0});
    
    Light light1;
    light1.pos.worldSpace = {0, 50, 10};
    light1.brightness     = 1;
    light1.diffuse        = LIGHT1_DIFFUSE;
    light1.specular       = LIGHT1_SPECULAR;
    Mat4<float> light1Rot = createRotationMatrixYZ(0.1f);

    Light light2;
    light2.pos.worldSpace = {50, 0, 0};
    light2.brightness     = 1;
    light2.diffuse        = LIGHT2_DIFFUSE;
    light2.specular       = LIGHT2_SPECULAR;
    Mat4<float> light2Rot = createRotationMatrixZX(0.05f);

    Scene scene(camera);
    scene.lightSources.insert(&light1);
    scene.lightSources.insert(&light2);
    scene.entities.insert(&sphere);
    scene.ambientColor = AMBIENT_COLOR;

    /* ================ Main loop ================ */
    SDL_Event event   = {};
    bool      running = true;

    while (running)
    {
        uint32_t frameStartTime = SDL_GetTicks();

        /* ================ Process events ================ */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: 
                { 
                    running = false; 
                    break; 
                }

                default: { break; }
            }
        }

        /* ================ Update objects ================ */
        light1.pos.worldSpace = light1Rot * light1.pos.worldSpace;
        light2.pos.worldSpace = light2Rot * light2.pos.worldSpace;

        /* ================ Rendering ================ */
        renderer.setColor(BACKGROUND_COLOR);
        renderer.clear();

        scene.updateWorldSpaceValues();
        scene.updateCameraSpaceValues();

        renderSceneRayTracing(renderer, scene);

        renderer.present();

        /* ================ Update fps title ================ */
        updateFpsTitle(window, SDL_GetTicks() - frameStartTime);
    }

    quitGraphics();

    return 0;
}

void updateFpsTitle(Window& window, uint32_t frameTime)
{
    static char windowTitle[MAX_WINDOW_TITLE_LENGTH] = {};

    // Time is in milliseconds, that's why use 1e3 - to convert into seconds
    uint32_t fps = 1e3 / frameTime;
    snprintf(windowTitle, MAX_WINDOW_TITLE_LENGTH, "%s [%" PRIu32 " fps]", WINDOW_TITLE, fps);

    window.updateTitle(windowTitle);
}