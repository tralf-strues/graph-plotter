//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file entity.cpp
//! @date 2021-09-16
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "entity.h"
#include "scene.h"

//------------------------------------Entity------------------------------------
Entity::Entity(const Material* material) : material(material) {}

const Vec3<float>& Entity::getPos() const
{
    return pos;
}

void Entity::setPos(const Vec3<float>& pos)
{
    this->pos = pos;
}

float Entity::getScale() const
{
    return scale;
}

void Entity::setScale(float scale)
{
    this->scale = scale;
}

const Material* Entity::getMaterial() const
{
    return material;
}

void Entity::setMaterial(const Material* material)
{
    this->material = material;
}
//------------------------------------------------------------------------------

//------------------------------------Sphere------------------------------------
Sphere::Sphere(const Material* material, float radius) : Entity(material), radius(radius) {}

const SpaceDepValue<float>& Sphere::getRadius() const
{
    return radius;
}

void Sphere::setRadius(float modelRadius)
{
    radius.modelSpace = modelRadius;
}

const SpaceDepValue<Vec3<float>>& Sphere::getCenter() const
{
    return center;
}

void Sphere::setCenter(const Vec3<float>& modelCenter)
{
    center.modelSpace = modelCenter;
}

void Sphere::toWorldSpace()
{
    radius.worldSpace = radius.modelSpace * scale;
    center.worldSpace =  pos;
}

void Sphere::toCameraSpace(const Camera& camera)
{
    radius.cameraSpace = radius.worldSpace; 
    center.cameraSpace = camera.getViewMatrix() * center.worldSpace;
}

bool Sphere::intersect(const Ray& ray, Hit* hit)
{
    /** 
     * Solving a quadratic equation 
     * t^2 * |D|^2 - 2 * t * (D, C) + |C|^2 - |R|^2 = 0,
     * 
     * where 
     * D = ray.direction
     * C = center.cameraSpace
     * R = radius.cameraSpace
     */

    float rayParameter1 = 0;
    float rayParameter2 = 0;

    float directionLen  = length(ray.direction);
    float a             = directionLen * directionLen;

    float b             = -2 * dotProduct(ray.direction, center.cameraSpace);

    float centerLen     = length(center.cameraSpace);
    float c             = centerLen * centerLen - radius.cameraSpace * radius.cameraSpace;

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
    hit->normal   = normalize(hit->pos - center.cameraSpace);
    hit->material = material;

    return true;
}
//------------------------------------------------------------------------------

//-------------------------------------Mesh-------------------------------------
Mesh::Mesh(const Material* material, size_t verticesCount) : 
           Entity(material), vertices(verticesCount) {}

void Mesh::setVertices(const Vec3<float>* vertices, size_t count)
{
    assert(vertices);

    this->vertices.~Array();
    this->vertices = Array<SpaceDepValue<Vec3<float>>>(count);

    for (size_t i = 0; i < count; ++i)
    {
        this->vertices[i].modelSpace = vertices[i];
    }
}

const Array<SpaceDepValue<Vec3<float>>>& Mesh::getVerticies() const
{
    return vertices;
}

void Mesh::toWorldSpace()
{
    Mat4<float> worldMatrix = createTranslationMatrix(pos) *
                              createRotationMatrix(rotation.x, rotation.y, rotation.z) *
                              createScaleMatrix(scale * Vec3<float>{1, 1, 1});

    for (size_t i = 0; i < vertices.getSize(); ++i)
    {
        vertices[i].worldSpace = worldMatrix * vertices[i].worldSpace;
    }
}

void Mesh::toCameraSpace(const Camera& camera)
{
    for (size_t i = 0; i < vertices.getSize(); ++i)
    {
        vertices[i].worldSpace = camera.getViewMatrix() * vertices[i].worldSpace;
    }
}

bool Mesh::intersect(const Ray& ray, Hit* hit)
{

}
//------------------------------------------------------------------------------