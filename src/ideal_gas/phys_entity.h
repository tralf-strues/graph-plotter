//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file phys_entity.h
//! @date 2021-09-26
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef PHYS_ENTITY_H
#define PHYS_ENTITY_H

#include "graphics_object.h"
#include "viewport.h"

//----------------------------------PhysEntity----------------------------------
class PhysEntity
{
public:
    enum Type
    {
        MOLECULE,
        WALL,
        TOTAL_TYPES
    };

public:
    const Type type;

    PhysEntity(Type type, float mass = 0, const Vec2<float>& pos = {0.0f, 0.0f}, 
               const Vec2<float>& velocity = {0.0f, 0.0f});

    void setPos(const Vec2<float> pos);
    Vec2<float> getPos() const;

    void setVelocity(const Vec2<float> velocity);
    Vec2<float> getVelocity() const;

    void setMass(float mass);
    float getMass() const;

    virtual void move(float deltaTime) = 0;
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) = 0;

protected:
    Vec2<float> pos;
    Vec2<float> velocity;
    float       mass;
};
//------------------------------------------------------------------------------

//-----------------------------------Molecule-----------------------------------
class Molecule : public PhysEntity
{
public:
    Molecule(float radius = 1);

    virtual void move(float deltaTime) override;
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) override;

    void setRadius(float radius);
    float getRadius() const;

private:
    float radius;
};
//------------------------------------------------------------------------------

//-------------------------------------Wall-------------------------------------
class Wall : public PhysEntity
{
public:
    Wall(const Vec2<float>& direction = Vec2<float>{1, 1});

    virtual void move(float deltaTime) override;
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) override;

    void setDirection(const Vec2<float>& direction);
    const Vec2<float>& getDirection() const;

private:
    Vec2<float> direction;
};
//------------------------------------------------------------------------------

#endif // PHYS_ENTITY_H