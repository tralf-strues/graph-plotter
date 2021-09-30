//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file entity.h
//! @date 2021-09-15
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef ENTITY_H
#define ENTITY_H

#include "../core/math/_core_math.h"
#include "../core/containers/_core_containers.h"
#include "space_dep_value.h"
#include "hit.h"
#include "material.h"

struct Scene;

class Entity : public SpaceDependent, public Hittable
{
public:
    Entity(const Material* material);

    const Vec3<float>& getPos      () const;
    void               setPos      (const Vec3<float>& pos);

    float              getScale    () const;
    void               setScale    (float scale);

    const Material*    getMaterial () const;
    void               setMaterial (const Material* material);

protected:
    Vec3<float>     pos      = {0, 0, 0}; ///< Position in world space.
    float           scale    = 1;         ///< Scale in world space.
    const Material* material = nullptr;   ///< Entity's material used for shading. 
};

class Sphere : public Entity
{
public:
    Sphere(const Material* material, float radius = 1);

    const SpaceDepValue<float>&       getRadius() const;
    void                              setRadius(float modelRadius);

    const SpaceDepValue<Vec3<float>>& getCenter() const;
    void                              setCenter(const Vec3<float>& modelCenter);

    /* SpaceDependent */
    virtual void toWorldSpace() override;
    virtual void toCameraSpace(const Camera& camera) override;

    /* Hittable */
    virtual bool intersect(const Ray& ray, Hit* hit) override;

private:
    SpaceDepValue<float>       radius;
    SpaceDepValue<Vec3<float>> center;
};

class Mesh : public Entity
{
public:
    Mesh(const Material* material, size_t verticesCount = 1);

    void setVertices(const Vec3<float>* vertices, size_t count);
    const Array<SpaceDepValue<Vec3<float>>>& getVerticies() const;

    void setRotation(const Vec3<float>& rotation);
    const Vec3<float>& getRotation() const;

    /* SpaceDependent */
    virtual void toWorldSpace() override;
    virtual void toCameraSpace(const Camera& camera) override;

    /* Hittable */
    virtual bool intersect(const Ray& ray, Hit* hit) override;

private:
    Array<SpaceDepValue<Vec3<float>>> vertices;

    /** pitchXY, yawZX, rollYZ */
    Vec3<float> rotation;
};

#endif // ENTITY_H