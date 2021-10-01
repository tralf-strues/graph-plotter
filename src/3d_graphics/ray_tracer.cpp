//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file ray_tracer.cpp
//! @date 2021-09-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "ray_tracer.h"
#include "hit.h"

const Vec3<float> CAMERA_POS = {0, 0, 0};

void renderPrimitive(RayTracer& rayTracer, Primitive3d& primitive);
bool capNormalized(Vec3<float>& color);
Color convertToRgba(Vec3<float> rgb);
Vec3<float> calculateColor(Scene& scene, const Hit& hit);

RayTracer::RayTracer(Scene* scene, Texture* targetTexture, ZBuffer* zbuffer) :
                     scene(scene), targetTexture(targetTexture), zbuffer(zbuffer) {}

void RayTracer::renderScene()
{
    targetTexture->clear(COLOR_BLACK);

    for (auto primitive : scene->primitives)
    {
        renderPrimitive(*this, *primitive);
    }
}

void renderPrimitive(RayTracer& rayTracer, Primitive3d& primitive)
{
    size_t width  = rayTracer.targetTexture->getWidth();
    size_t height = rayTracer.targetTexture->getHeight();

    float fwidth  = (float) width;
    float fheight = (float) height;

    Scene& scene = *rayTracer.scene;
    Camera& camera = scene.camera;

    const ViewFrustum& frustum = camera.getViewFrustum();
    
    for (size_t xScreen = 0; xScreen < width; xScreen++)
    {
        for (size_t yScreen = 0; yScreen < height; yScreen++)
        {
            Vec3<float> curPoint = toViewFrustumPoint({xScreen, yScreen}, 
                                                      fwidth, fheight, 
                                                      frustum);

            Ray ray = {};
            ray.direction = curPoint;

            Vec3<float> color = {0};
            bool updateColor = false;

            Hit hit = {};
            if (primitive.intersect(ray, &hit) &&
                dotProduct(hit.pos - camera.getPos().cameraSpace, hit.normal) <= 0 &&
                rayTracer.zbuffer->setDepth(xScreen, yScreen, hit.pos.z))
            {
                color = calculateColor(scene, hit);
                updateColor = true;
            }

            if (updateColor)
            {
                (*rayTracer.targetTexture)[yScreen][xScreen] = convertToRgba(color);
            }
        }
    }
}

bool capNormalized(Vec3<float>& color)
{
    uint8_t componentsCapped = 0;

    for (size_t i = 0; i < 3; i++)
    {
        if (color.getCoord(i) > 1) 
        {
            color.getCoord(i) = 1;
            componentsCapped++;
        }
    }

    return componentsCapped == 3; 
}

Color convertToRgba(Vec3<float> rgb)
{
    rgb *= 0xFF;
    return rgbaColor(rgb.x, rgb.y, rgb.z, 0xFF);
}

Vec3<float> calculateColor(Scene& scene, const Hit& hit)
{
    assert(hit.material);

    Vec3<float> color       = componentMultiply(scene.ambientColor, hit.material->ambient);
    size_t      lightsCount = scene.lightSources.getSize();
    Vec3<float> toCamera    = normalize(CAMERA_POS - hit.pos);

    capNormalized(color);

    for (size_t i = 0; i < lightsCount; i++)
    {
        Vec3<float> toLight = normalize(scene.lightSources[i]->pos.cameraSpace - hit.pos);

        /* Diffuse component */
        Vec3<float> diffuse = {0};
        float dotNormalLight = dotProduct(hit.normal, toLight);
        if (dotNormalLight > 0)
        {
            diffuse = dotNormalLight * componentMultiply(scene.lightSources[i]->diffuse, 
                                                         hit.material->diffuse);
        }
        capNormalized(diffuse);

        /* Specular component */
        Vec3<float> specular = {0};
        Vec3<float> halfway  = toCamera + toLight;
        
        if (length(halfway) != 0)
        {
            float dotNormalHalfway = dotProduct(hit.normal, normalize(halfway));

            if (dotNormalHalfway)
            {
                specular = powf(dotNormalHalfway, hit.material->shiness) * 
                           componentMultiply(scene.lightSources[i]->specular, 
                                             hit.material->specular);
            }
            capNormalized(specular);
        }

        color += diffuse + specular;

        if (capNormalized(color))
        {
            break;
        }
    }

    return color;
}