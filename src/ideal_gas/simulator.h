//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file simulator.h
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../core/containers/list.h"
#include "../core/math/_core_math.h"
#include "phys_entity.h"

typedef List<PhysEntity*>::Iterator EntitiesIterator;

struct Collision
{
    EntitiesIterator first;
    EntitiesIterator second;

    Collision(const EntitiesIterator& first, const EntitiesIterator& second);
};

class Simulator
{
public:
    List<PhysEntity*> entities;

    ~Simulator();

    void simulate(float deltaTime);
    void updateGraphics(Renderer& renderer, const Viewport& viewport);

private:
    bool collisionDetect(EntitiesIterator first, EntitiesIterator second, 
                         float deltaTime, Collision* collision);

    void collisionRespond(Collision& collision);

    bool chemicalReaction(Collision& collision);
};

bool collisionDetectEleEle(EntitiesIterator first, EntitiesIterator second, 
                           float deltaTime, Collision* collision); 

bool collisionDetectEleWal(EntitiesIterator first, EntitiesIterator second, 
                           float deltaTime, Collision* collision);

void collisionRespondEleEle(Collision& collision);
void collisionRespondEleWal(Collision& collision);

bool chemicalReactionEleEle(List<PhysEntity*>& entities, Collision& collision);

#endif // SIMULATOR_H