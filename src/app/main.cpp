//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../3d_graphics/ray_tracer.h"

static const size_t      WINDOW_WIDTH              = 1200;
static const size_t      WINDOW_HEIGHT             = 800;
static const char*       WINDOW_TITLE              = "Ray-tracing";
static const size_t      MAX_WINDOW_TITLE_LENGTH   = 128;
static const Color       BACKGROUND_COLOR          = 0x2F'69'AA'FF; 

static const float       FOV                       = 0.78f; // Approx 45 degrees
static const float       ASPECT                    = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
static const float       NEAR                      = 1;
static const float       FAR                       = 600;
static const ViewFrustum VIEW_FRUSTUM              = {FOV, ASPECT, NEAR, FAR};

static const Vec3<float> LIGHT1_DIFFUSE            = {0.9f, 0.9f, 0.6f};
static const Vec3<float> LIGHT1_SPECULAR           = {0.1f, 0.1f, 0.1f};

static const Vec3<float> LIGHT2_DIFFUSE            = {0.6f, 0.6f, 1.0f};
static const Vec3<float> LIGHT2_SPECULAR           = {0.4f, 0.4f, 0.8f};

static const Vec3<float> AMBIENT_COLOR             = {0.5f, 0.5f, 0.5f};

static const Material    SPHERE_MATERIAL           = {{0.3f, 0.1f, 0.1f},
                                                      {0.9f, 0.7f, 0.7f},
                                                      {0.5f, 0.5f, 0.5f},
                                                      50};

static const float       CAMERA_VELOCITY           = 10.0f;
static const float       MOUSE_SENSITIVITY         = 1e-2f;
static const float       CAMERA_VERTICAL_ANGLE_MAX = 1.2f;

void processKeyboard(const Window& window, Scene& scene, uint32_t deltaTime);
void processMouse(const SDL_Event& event, Scene& scene, uint32_t deltaTime);
void updateFpsTitle(Window& window, uint32_t frameTime);

int main()
{
    initGraphics();

    if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0)
    {
        return -1;
    }

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);
    Texture texture(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    ZBuffer zbuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* ================ Entities ================ */
    Sphere sphere1(&SPHERE_MATERIAL);
    sphere1.setPos({25, 0, 0});
    sphere1.setScale(5);

    Sphere sphere2(&SPHERE_MATERIAL);
    sphere2.setPos({25, 3, 10});
    sphere2.setScale(3);

    /* ================ Scene ================ */
    Camera camera(VIEW_FRUSTUM);
    
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
    scene.entities.insert(&sphere1);
    scene.entities.insert(&sphere2);
    scene.ambientColor = AMBIENT_COLOR;

    /* ================ Main loop ================ */
    SDL_Event event     = {};
    bool      running   = true;
    uint32_t  deltaTime = 0;

    while (running)
    {
        // SDL_WarpMouseInWindow(window.getNativeWindow(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        uint32_t frameStartTime = SDL_GetTicks();

        /* ================ Process events ================ */
        processKeyboard(window, scene, deltaTime);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: 
                { 
                    running = false; 
                    break; 
                }

                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    {
                        running = false;
                    }

                    break;
                }

                case SDL_MOUSEMOTION:
                {
                    processMouse(event, scene, deltaTime);
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

        zbuffer.reset();
        renderSceneRayTracing(texture, zbuffer, scene);
        texture.update();

        renderer.renderTexture(texture, {0, 0});
        renderer.present();

        /* ================ Update fps title ================ */
        deltaTime = SDL_GetTicks() - frameStartTime;
        updateFpsTitle(window, deltaTime);
    }

    quitGraphics();

    return 0;
}

void processKeyboard(const Window& window, Scene& scene, uint32_t deltaTime)
{
    const uint8_t* keystate = SDL_GetKeyboardState(nullptr);
    assert(keystate);

    Vec3<float> velocityDirection = {0};
    Vec3<float> forward = createRotationMatrix(scene.camera.getPitchVertical(), 
                                               scene.camera.getYawHorizontal(), 
                                               0) * 
                          Vec3{1.0f, 0.0f, 0.0f};

    if (keystate[SDL_SCANCODE_W])
    {
        velocityDirection += normalize(forward);
    } 
    
    if (keystate[SDL_SCANCODE_S])
    {
        velocityDirection -= normalize(forward);
    }

    if (keystate[SDL_SCANCODE_D])
    {
        velocityDirection += crossProduct(Vec3<float>{0, 1, 0}, 
                                          normalize(forward));
    }

    if (keystate[SDL_SCANCODE_A])
    {
        velocityDirection -= crossProduct(Vec3<float>{0, 1, 0}, 
                                          normalize(forward));
    }

    scene.camera.setPos(scene.camera.getPos().worldSpace + 
                        velocityDirection * ((float) deltaTime / 1e3f) * CAMERA_VELOCITY);
}

void processMouse(const SDL_Event& event, Scene& scene, uint32_t deltaTime)
{
    Camera& camera = scene.camera;

    Vec2<float> mouseDelta = {event.motion.xrel, -event.motion.yrel};
    mouseDelta *= MOUSE_SENSITIVITY * ((float) deltaTime / 1e3f);

    float newPitch = camera.getPitchVertical() + mouseDelta.y;
    float newYaw   = camera.getYawHorizontal() + mouseDelta.x;

    if (newPitch > CAMERA_VERTICAL_ANGLE_MAX)
    {
        newPitch = CAMERA_VERTICAL_ANGLE_MAX;
    }
    else if (newPitch < -CAMERA_VERTICAL_ANGLE_MAX)
    {
        newPitch = -CAMERA_VERTICAL_ANGLE_MAX;
    }

    camera.setPitchVertical(newPitch);
    camera.setYawHorizontal(newYaw);
}

void updateFpsTitle(Window& window, uint32_t frameTime)
{
    static char windowTitle[MAX_WINDOW_TITLE_LENGTH] = {};

    // Time is in milliseconds, that's why use 1e3 - to convert into seconds
    uint32_t fps = 1e3 / frameTime;
    snprintf(windowTitle, MAX_WINDOW_TITLE_LENGTH, "%s [%" PRIu32 " fps]", WINDOW_TITLE, fps);

    window.updateTitle(windowTitle);
}