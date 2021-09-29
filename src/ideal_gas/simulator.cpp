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
        {collisionDetectMolMol, collisionRespondMolMol, chemicalReactionMolMol},
        {collisionDetectMolWal, collisionRespondMolWal, nullptr}
    },

    {
        {collisionDetectMolWal, collisionRespondMolWal, nullptr},
        {nullptr,               nullptr,                nullptr}
    }
};

Collision::Collision(const EntitiesIterator& first, const EntitiesIterator& second) : 
                     first(first), second(second) {}

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

bool collisionDetectMolMol(EntitiesIterator first, EntitiesIterator second, 
                           float deltaTime, Collision* collision)
{
    Molecule* firstMolecule  = (Molecule*) *first;
    Molecule* secondMolecule = (Molecule*) *second;

    float distanceSquare  = lengthSquare(firstMolecule->getPos() - secondMolecule->getPos());
    float sumRadiusSquare = firstMolecule->getRadius() + secondMolecule->getRadius();
    sumRadiusSquare *= sumRadiusSquare;

    if (cmpFloat(distanceSquare, sumRadiusSquare) <= 0)
    {
        collision->first  = first;
        collision->second = second;
        return true;
    }

    return false;
}

bool collisionDetectMolWal(EntitiesIterator first, EntitiesIterator second, 
                           float deltaTime, Collision* collision)
{
    Molecule* molecule  = (Molecule*) *first;
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

void collisionRespondMolMol(Collision& collision)
{
    Molecule* firstMolecule  = (Molecule*) *(collision.first);
    Molecule* secondMolecule = (Molecule*) *(collision.second);

    Vec2<float> along = normalize(secondMolecule->getPos() - firstMolecule->getPos());
    
    float v1 = dotProduct(along, firstMolecule->getVelocity());
    float v2 = dotProduct(along, secondMolecule->getVelocity());
    float m1 = firstMolecule->getMass();
    float m2 = secondMolecule->getMass();

    float newV1 = (2 * m2 * v2 + v1 * (m1 - m2)) / (m1 + m2);
    float newV2 = (2 * m1 * v1 + v2 * (m2 - m1)) / (m1 + m2);

    Vec2<float> perpendicularVelocity1 = firstMolecule->getVelocity()  - v1 * along;
    Vec2<float> perpendicularVelocity2 = secondMolecule->getVelocity() - v2 * along;

    firstMolecule->setVelocity(perpendicularVelocity1  + newV1 * along);
    secondMolecule->setVelocity(perpendicularVelocity2 + newV2 * along);
}

void collisionRespondMolWal(Collision& collision)
{
    Molecule* molecule = (Molecule*) *(collision.first);
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

bool chemicalReactionMolMol(List<PhysEntity*>& entities, Collision& collision)
{
    Molecule* firstMolecule  = (Molecule*) *(collision.first);
    Molecule* secondMolecule = (Molecule*) *(collision.second);

    Molecule* newMolecule = new Molecule(firstMolecule->getRadius() + secondMolecule->getRadius());
    newMolecule->setPos(firstMolecule->getPos());
    newMolecule->setMass(firstMolecule->getMass() + secondMolecule->getMass());
    newMolecule->setVelocity(firstMolecule->getVelocity() + secondMolecule->getVelocity());

    entities.pushBack(newMolecule);
    entities.remove(collision.first);
    entities.remove(collision.second);

    return true;
}
//------------------------------------------------------------------------------

// bool collide(const Molecule& molecule, const Line& line, float deltaTime, float* collisionTime)
// {
//     assert(collisionTime);

//     Vec2<float> normal = line.calculateNormal();

//     float dotVelocityNormal = dotProduct(molecule.velocity, normal);
//     float dotCenterNormal   = dotProduct(molecule.pos,      normal);
//     float dotFromNormal     = dotProduct(line.from,         normal);
//     float radiusSquared     = molecule.radius * molecule.radius;

//     float a = dotVelocityNormal * dotVelocityNormal;
//     float b = -2 * dotFromNormal * dotVelocityNormal;
//     float c = dotFromNormal * dotFromNormal - 2 * dotFromNormal * dotCenterNormal - radiusSquared;

//     float time1 = 0;
//     float time2 = 0;

//     int32_t solutions = solveQuadraticEquation(a, b, c, &time1, &time2);

//     if (solutions != 1 && solutions != 2)
//     {
//         return false;
//     }

//     *collisionTime = (solutions == 2 && time2 < time1) ? time2 : time1;
//     return cmpFloat(*collisionTime, deltaTime) <= 0;
// }

// void collisionRespond(Molecule& molecule, Line& line, float collisionTime)
// {
//     Vec2<float> normal = line.calculateNormal();

//     float velocityPerpendicular = dotProduct(molecule.velocity, normal);
//     float velocityAlong         = dotProduct(molecule.velocity, normalize(line.direction));

//     molecule.pos      = collisionTime * molecule.velocity;
//     molecule.velocity = velocityAlong * normalize(line.direction) - velocityPerpendicular * normal;
// }

// void drawMolecule(Renderer& renderer, const Viewport& viewport, const Molecule& molecule)
// {
//     Vec2<float> center = toPixels(renderer, viewport, molecule.pos);
//     float       radius = molecule.radius * 
//                          renderer.getWindow().getWidth() /
//                          viewport.getRelativeWidth();

//     Circle circle = {center, (int32_t) radius};
//     renderCircle(renderer, circle);
// }