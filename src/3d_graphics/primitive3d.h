//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file primitive3d.h
//! @date 2021-10-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef PRIMITIVE_3D_H
#define PRIMITIVE_3D_H

#include "space_dep_value.h"
#include "hit.h"
#include "material.h"
#include "camera.h"

struct Vertex
{
    SpaceDepValue<Vec3<float>> pos;
    SpaceDepValue<Vec3<float>> normal;
    Vec2<float> uv;
};

struct Primitive3d : public SpaceDependent, public Hittable
{
    Primitive3d(const Material* material);

    SpaceDepValue<Vec3<float>> pos;      ///< Position in world space.
    float                      scale;    ///< Scale in world space.
    const Material*            material; ///< Primitive's material used for shading. 
};

struct Sphere : public Primitive3d
{
    SpaceDepValue<float> radius;

    Sphere(const Material* material, float radius = 1);

    /* SpaceDependent */
    virtual void toWorldSpace() override;
    virtual void toCameraSpace(const Camera& camera) override;

    /* Hittable */
    virtual bool intersect(const Ray& ray, Hit* hit) override;
};

struct Triangle : public Primitive3d
{
    Vertex v0;
    Vertex v1;
    Vertex v2;

    Vec3<float> rotation;

    Triangle(const Material* material);

    /* SpaceDependent */
    virtual void toWorldSpace() override;
    virtual void toCameraSpace(const Camera& camera) override;

    /* Hittable */
    virtual bool intersect(const Ray& ray, Hit* hit) override;
};

#endif // PRIMITIVE_3D_H