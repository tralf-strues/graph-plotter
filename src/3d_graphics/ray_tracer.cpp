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
        float dotNormalLight = dotProduct(hit.normal, toLight);
        Vec3<float> diffuse;
        if (dotNormalLight > 0)
        {
            diffuse = dotNormalLight * componentMultiply(scene.lightSources[i]->diffuse, 
                                                         hit.material->diffuse);
        }
        capNormalized(diffuse);

        /* Specular component */
        Vec3<float> halfway          = normalize(toCamera + toLight);
        float       dotNormalHalfway = dotProduct(hit.normal, halfway);

        Vec3<float> specular;

        if (dotNormalHalfway)
        {
            specular = powf(dotNormalHalfway, hit.material->shiness) * 
                       componentMultiply(scene.lightSources[i]->specular, 
                                         hit.material->specular);
        }
        capNormalized(specular);

        color += diffuse + specular;

        if (capNormalized(color))
        {
            break;
        }
    }

    return color;
}

void renderSceneRayTracing(Renderer& renderer, Scene& scene)
{
    size_t             width         = renderer.getWindow().getWidth();
    size_t             height        = renderer.getWindow().getHeight();

    float              fwidth        = (float) width;
    float              fheight       = (float) height;

    size_t             entitiesCount = scene.entities.getSize();
    const ViewFrustum& frustum       = scene.camera.getViewFrustum();
    
    for (size_t xScreen = 0; xScreen < width; xScreen++)
    {
        for (size_t yScreen = 0; yScreen < height; yScreen++)
        {
            Vec3<float> curPoint = toViewFrustumPoint({xScreen, yScreen}, 
                                                      fwidth, fheight, 
                                                      frustum);

            Ray ray = {};
            ray.direction = curPoint;

            Vec3<float> color           = {0};
            float       nearestRayParam = 0;
            bool        intersected     = false;

            Hit hit = {};
            for (size_t i = 0; i < entitiesCount; i++)
            {
                if (scene.entities[i]->intersect(ray, &hit) && 
                    (!intersected || (intersected && hit.rayParameter < nearestRayParam)))
                {
                    color           = calculateColor(scene, hit);
                    nearestRayParam = hit.rayParameter;

                    intersected = true;
                }
            }

            if (intersected)
            {
                renderer.setColor(convertToRgba(color));
                renderPoint(renderer, {xScreen, yScreen});
            }
        }
    }
}