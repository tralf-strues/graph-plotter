//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file primitive3d.cpp
//! @date 2021-10-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/math/_core_math.h"
#include "primitive3d.h"

//----------------------------------Primitive3d---------------------------------
Primitive3d::Primitive3d(const Material* material) : material(material) {}
//------------------------------------------------------------------------------

//------------------------------------Sphere------------------------------------
Sphere::Sphere(const Material* material, float radius) : Primitive3d(material), radius(radius) {}

void Sphere::toWorldSpace()
{
    radius.worldSpace = radius.modelSpace * scale;
}

void Sphere::toCameraSpace(const Camera& camera)
{
    radius.cameraSpace = radius.worldSpace; 
    pos.cameraSpace    = camera.getViewMatrix() * pos.worldSpace;
}

bool Sphere::intersect(const Ray& ray, Hit* hit)
{
    /** 
     * Solving a quadratic equation 
     * t^2 * |D|^2 - 2 * t * (D, C) + |C|^2 - |R|^2 = 0,
     * 
     * where 
     * D = ray.direction
     * C = pos.cameraSpace
     * R = radius.cameraSpace
     */

    float rayParameter1 = 0;
    float rayParameter2 = 0;

    float directionLen  = length(ray.direction);
    float a             = directionLen * directionLen;

    float b             = -2 * dotProduct(ray.direction, pos.cameraSpace);

    float posLen        = length(pos.cameraSpace);
    float c             = posLen * posLen - radius.cameraSpace * radius.cameraSpace;

    int32_t solutions   = solveQuadraticEquation(a, b, c, &rayParameter1, &rayParameter2);

    if (solutions != 1 && solutions != 2)
    {
        return false;
    }

    hit->rayParameter = rayParameter1;

    if (solutions == 2 && rayParameter2 < rayParameter1 && cmpFloat(rayParameter2, 0) >= 0)
    {
        hit->rayParameter = rayParameter2;
    }

    if (cmpFloat(hit->rayParameter, 0) < 0)
    {
        return false;
    }

    hit->pos      = ray.at(hit->rayParameter);
    hit->normal   = normalize(hit->pos - pos.cameraSpace);
    hit->material = material;

    return true;
}
//------------------------------------------------------------------------------

//-----------------------------------Triangle-----------------------------------
Triangle::Triangle(const Material* material) : Primitive3d(material) {}

void Triangle::toWorldSpace()
{
    
}

void Triangle::toCameraSpace(const Camera& camera)
{

}

bool Triangle::intersect(const Ray& ray, Hit* hit)
{
    return false;
}
//------------------------------------------------------------------------------