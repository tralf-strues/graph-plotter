//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file simulator.h
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../core/containers/list.h"
#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../core/math/_core_math.h"
#include "phys_entity.h"

typedef List<PhysEntity*>::Iterator EntitiesIterator;

static const float ATOM_DENSITY                      = 1e-3;
static const float ENERGY_THRESHOLD_ELECTRON_CAPTURE = 1e5;
static const float MASS_THRESHOLD_ATOMS_COMBINE      = 5e-3;
static const float MASS_THRESHOLD_ATOMS_BREAK        = 1e-2;
static const float RADIUS_BROKEN_ATOM                = 0.7f;
static const float VOLUME_BROKEN_ATOM                = calculateSphereVolume(RADIUS_BROKEN_ATOM);

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

bool collisionDetectElectronElectron(EntitiesIterator first, EntitiesIterator second, 
                                     float deltaTime, Collision* collision); 

bool collisionDetectElectronWall(EntitiesIterator first, EntitiesIterator second, 
                                 float deltaTime, Collision* collision);

bool collisionDetectWallAtom(EntitiesIterator first, EntitiesIterator second, 
                             float deltaTime, Collision* collision);

bool collisionDetectAtomAtom(EntitiesIterator first, EntitiesIterator second, 
                             float deltaTime, Collision* collision);

bool collisionDetectElectronAtom(EntitiesIterator first, EntitiesIterator second, 
                                 float deltaTime, Collision* collision);


void collisionRespondElectronElectron(Collision& collision);
void collisionRespondElectronWall(Collision& collision);
void collisionRespondWallAtom(Collision& collision);
void collisionRespondAtomAtom(Collision& collision);
void collisionRespondElectronAtom(Collision& collision);

bool chemicalReactionElectronAtom(List<PhysEntity*>& entities, Collision& collision);
bool chemicalReactionAtomAtom(List<PhysEntity*>& entities, Collision& collision);

#endif // SIMULATOR_H