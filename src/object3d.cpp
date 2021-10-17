//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file object3d.cpp
//! @date 2021-10-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "object3d.h"

//-----------------------------------Object3d-----------------------------------
Object3d::Object3d(const Material* material) : material(material) {}
//------------------------------------------------------------------------------

//------------------------------------Sphere------------------------------------
Sphere::Sphere(const Material* material, float radius) : Object3d(material), radius(radius) {}

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
Triangle::Triangle(const Material* material) : Object3d(material) {}

void Triangle::toWorldSpace()
{
    Mat4<float> worldMatrix = createTranslationMatrix(pos.worldSpace) *
                              createScaleMatrix(scale) *
                              createRotationMatrix(rotation.x, rotation.y, rotation.z);

    v0.pos.worldSpace    = worldMatrix * v0.pos.modelSpace;
    v0.normal.worldSpace = worldMatrix * v0.normal.modelSpace;

    v1.pos.worldSpace    = worldMatrix * v1.pos.modelSpace;
    v1.normal.worldSpace = worldMatrix * v1.normal.modelSpace;

    v2.pos.worldSpace    = worldMatrix * v2.pos.modelSpace;
    v2.normal.worldSpace = worldMatrix * v2.normal.modelSpace;
}

void Triangle::toCameraSpace(const Camera& camera)
{
    const Mat4<float>& viewMatrix = camera.getViewMatrix();

    v0.pos.cameraSpace    = viewMatrix * v0.pos.worldSpace;
    v0.normal.cameraSpace = viewMatrix * v0.normal.worldSpace;

    v1.pos.cameraSpace    = viewMatrix * v1.pos.worldSpace;
    v1.normal.cameraSpace = viewMatrix * v1.normal.worldSpace;

    v2.pos.cameraSpace    = viewMatrix * v2.pos.worldSpace;
    v2.normal.cameraSpace = viewMatrix * v2.normal.worldSpace;
}

bool Triangle::intersect(const Ray& ray, Hit* hit)
{
    Vec3<float> direction = ray.direction;
    Vec3<float> side0     = v0.pos.cameraSpace - v1.pos.cameraSpace;
    Vec3<float> side1     = v0.pos.cameraSpace - v2.pos.cameraSpace;

    Mat4<float> matrix    = {{direction.x, side0.x, side1.x, 0,
                              direction.y, side0.y, side1.y, 0,
                              direction.z, side0.z, side1.z, 0,
                              0,           0,       0,       1}};

    float determinant = determinant3x3(matrix);
    
    // determinant == 0 <=> ray is parallel to the triangle's surface
    // determinant <  0 <=> the triangle is back-facing FIXME: WHY? 
    if (cmpFloat(determinant, 0) <= 0)
    {
        return false;
    }

    

    return false;
}
//------------------------------------------------------------------------------