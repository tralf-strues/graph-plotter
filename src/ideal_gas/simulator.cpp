//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file simulator.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/containers/dynamic_array.h"
#include "simulator.h"

typedef bool (*FCollisionDetect) (EntitiesIterator first, EntitiesIterator second, 
                                 float deltaTime, Collision* collision);
typedef void (*FCollisionRespond) (Collision& collision);
typedef bool (*FChemicalReaction) (List<PhysEntity*>& entities, Collision& collision);

struct EntityPairInteraction
{
    FCollisionDetect  collisionDetect;
    FCollisionRespond collisionRespond;
    FChemicalReaction chemicalReaction;
};

const EntityPairInteraction INTERACTIONS[PhysEntity::TOTAL_TYPES][PhysEntity::TOTAL_TYPES] = {
    {
        {collisionDetectEleEle, collisionRespondEleEle, chemicalReactionEleEle},
        {collisionDetectEleWal, collisionRespondEleWal, nullptr}
    },

    {
        {collisionDetectEleWal, collisionRespondEleWal, nullptr},
        {nullptr,               nullptr,                nullptr}
    }
};

Collision::Collision(const EntitiesIterator& first, const EntitiesIterator& second) : 
                     first(first), second(second) {}

Simulator::~Simulator()
{
    for (auto entity : entities)
    {
        delete entity;
    }
}

void Simulator::simulate(float deltaTime)
{
    DynamicArray<Collision> collisions = {};

    for (PhysEntity* entity : entities)
    {
        entity->move(deltaTime);
    }

    for (EntitiesIterator first = entities.begin(); first != entities.end(); ++first)
    {
        for (EntitiesIterator second = first; (++second) != entities.end();)
        {
            Collision collision{entities.begin(), entities.begin()};
            
            if (collisionDetect(first, second, deltaTime, &collision))
            {
                collisions.insert(collision);
            }
        }
    }

    for (size_t i = 0; i < collisions.getSize(); i++)
    {
        if (!chemicalReaction(collisions[i]))
        {
            collisionRespond(collisions[i]);
        }
    }
}

void Simulator::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    for (PhysEntity* entity : entities)
    {
        entity->updateGraphics(renderer, viewport);
    }
}

//------------------------------Collision detection-----------------------------
bool Simulator::collisionDetect(EntitiesIterator first, EntitiesIterator second, 
                                float deltaTime, Collision* collision)
{
    FCollisionDetect function = INTERACTIONS[(*first)->type][(*second)->type].collisionDetect;
    if (function == nullptr)
    {
        return false;
    }

    if ((*first)->type > (*second)->type)
    {
        return function(second, first, deltaTime, collision);
    }

    return function(first, second, deltaTime, collision);
}

bool collisionDetectEleEle(EntitiesIterator first, EntitiesIterator second, 
                           float deltaTime, Collision* collision)
{
    Electron* firstElectron  = (Electron*) *first;
    Electron* secondElectron = (Electron*) *second;

    float distanceSquare  = lengthSquare(firstElectron->getPos() - secondElectron->getPos());
    float sumRadiusSquare = firstElectron->getRadius() + secondElectron->getRadius();
    sumRadiusSquare *= sumRadiusSquare;

    if (cmpFloat(distanceSquare, sumRadiusSquare) <= 0)
    {
        collision->first  = first;
        collision->second = second;
        return true;
    }

    return false;
}

bool collisionDetectEleWal(EntitiesIterator first, EntitiesIterator second, 
                           float deltaTime, Collision* collision)
{
    Electron* molecule  = (Electron*) *first;
    Wall*     wall      = (Wall*)     *second;

    Vec2<float> point    = molecule->getPos();
    Vec2<float> lineFrom = wall->getPos();
    Vec2<float> along    = normalize(wall->getDirection());

    float distance = length((point - lineFrom) - (dotProduct(point - lineFrom, along) * along));

    if (cmpFloat(distance, molecule->getRadius()) <= 0)
    {
        collision->first  = first;
        collision->second = second;

        return true;
    }

    return false;
}
//------------------------------------------------------------------------------

//-------------------------------Collision respond------------------------------
void Simulator::collisionRespond(Collision& collision)
{
    FCollisionRespond function = INTERACTIONS[(*collision.first)->type][(*collision.second)->type]
                                             .collisionRespond;
    if (function != nullptr)
    {
        if ((*collision.first)->type > (*collision.second)->type)
        {
            EntitiesIterator tmp = collision.first;
            collision.first = collision.second;
            collision.second = tmp;
        }

        function(collision);
    }
}

void collisionRespondEleEle(Collision& collision)
{
    Electron* firstElectron  = (Electron*) *(collision.first);
    Electron* secondElectron = (Electron*) *(collision.second);

    Vec2<float> along = normalize(secondElectron->getPos() - firstElectron->getPos());
    
    float v1 = dotProduct(along, firstElectron->getVelocity());
    float v2 = dotProduct(along, secondElectron->getVelocity());
    float m1 = firstElectron->getMass();
    float m2 = secondElectron->getMass();

    float newV1 = (2 * m2 * v2 + v1 * (m1 - m2)) / (m1 + m2);
    float newV2 = (2 * m1 * v1 + v2 * (m2 - m1)) / (m1 + m2);

    Vec2<float> perpendicularVelocity1 = firstElectron->getVelocity()  - v1 * along;
    Vec2<float> perpendicularVelocity2 = secondElectron->getVelocity() - v2 * along;

    firstElectron->setVelocity(perpendicularVelocity1  + newV1 * along);
    secondElectron->setVelocity(perpendicularVelocity2 + newV2 * along);
}

void collisionRespondEleWal(Collision& collision)
{
    Electron* molecule = (Electron*) *(collision.first);
    Wall*     wall     = (Wall*)     *(collision.second);

    Vec2<float> along = normalize(wall->getDirection());

    float       vAlong         = dotProduct(molecule->getVelocity(), along);
    Vec2<float> vPerpendicular = molecule->getVelocity() - vAlong * along;

    molecule->setVelocity(vAlong * along - vPerpendicular);
}
//------------------------------------------------------------------------------

//-------------------------------Chemical reaction------------------------------
bool Simulator::chemicalReaction(Collision& collision)
{
    FChemicalReaction function = INTERACTIONS[(*collision.first)->type][(*collision.second)->type]
                                             .chemicalReaction;
    if (function == nullptr)
    {
        return false;
    }

    if ((*collision.first)->type > (*collision.second)->type)
    {
        EntitiesIterator tmp = collision.first;
        collision.first = collision.second;
        collision.second = tmp;
    }

    return function(entities, collision); 
}

bool chemicalReactionEleEle(List<PhysEntity*>& entities, Collision& collision)
{
    Electron* firstElectron  = (Electron*) *(collision.first);
    Electron* secondElectron = (Electron*) *(collision.second);

    Electron* newElectron = new Electron(firstElectron->getRadius() + secondElectron->getRadius());
    newElectron->setPos(firstElectron->getPos());
    newElectron->setMass(firstElectron->getMass() + secondElectron->getMass());
    newElectron->setVelocity(firstElectron->getVelocity() + secondElectron->getVelocity());

    entities.remove(collision.first);
    entities.remove(collision.second);
    entities.pushBack(newElectron);

    return true;
}
//------------------------------------------------------------------------------