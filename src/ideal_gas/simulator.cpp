//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file simulator.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/containers/dynamic_array.h"
#include "../core/utils/random.h"
#include "simulator.h"

typedef void (*FDistantInteract) (PhysEntity* first, PhysEntity* second, float deltaTime);
typedef bool (*FCollisionDetect) (EntitiesIterator first, EntitiesIterator second, 
                                  float deltaTime, Collision* collision);
typedef void (*FCollisionRespond) (Collision& collision);
typedef bool (*FChemicalReaction) (List<PhysEntity*>& entities, Collision& collision);

struct EntityPairInteraction
{
    FDistantInteract  distantInteract;
    FCollisionDetect  collisionDetect;
    FCollisionRespond collisionRespond;
    FChemicalReaction chemicalReaction;
};

const EntityPairInteraction INTERACT_ELECTRON_ELECTRON = {nullptr,
                                                          collisionDetectElectronElectron, 
                                                          collisionRespondElectronElectron, 
                                                          nullptr};

const EntityPairInteraction INTERACT_ELECTRON_WALL = {distantInteractElectronWall,
                                                      collisionDetectElectronWall, 
                                                      collisionRespondElectronWall, 
                                                      nullptr};

const EntityPairInteraction INTERACT_ELECTRON_ATOM = {nullptr,
                                                      collisionDetectElectronAtom, 
                                                      collisionRespondElectronAtom, 
                                                      chemicalReactionElectronAtom};

const EntityPairInteraction INTERACT_WALL_WALL = {nullptr,
                                                  nullptr, 
                                                  nullptr, 
                                                  nullptr};

const EntityPairInteraction INTERACT_WALL_ATOM = {distantInteractWallAtom,
                                                  collisionDetectWallAtom, 
                                                  collisionRespondWallAtom, 
                                                  nullptr};

const EntityPairInteraction INTERACT_ATOM_ATOM = {nullptr,
                                                  collisionDetectAtomAtom, 
                                                  collisionRespondAtomAtom, 
                                                  chemicalReactionAtomAtom};

const EntityPairInteraction INTERACTIONS[PhysEntity::TOTAL_TYPES][PhysEntity::TOTAL_TYPES] =
{
    {INTERACT_ELECTRON_ELECTRON, INTERACT_ELECTRON_WALL, INTERACT_ELECTRON_ATOM},
    {INTERACT_ELECTRON_WALL,     INTERACT_WALL_WALL,     INTERACT_WALL_ATOM    },
    {INTERACT_ELECTRON_ATOM,     INTERACT_WALL_ATOM,     INTERACT_ATOM_ATOM    },
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

    for (EntitiesIterator first = entities.begin(); first != entities.end(); ++first)
    {
        for (EntitiesIterator second = first; (++second) != entities.end();)
        {
            distantInteract(*first, *second, deltaTime);
        }
    }

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

//-------------------------------Distant interact------------------------------
void Simulator::distantInteract(PhysEntity* first, PhysEntity* second, float deltaTime)
{
    FDistantInteract function = INTERACTIONS[first->type][second->type].distantInteract;
    if (function == nullptr)
    {
        return;
    }

    if (first->type > second->type)
    {
        function(second, first, deltaTime);
    }
    else
    {
        function(first, second, deltaTime);
    }
}

void distantInteractElectronWall(PhysEntity* first, PhysEntity* second, float deltaTime)
{
    Electron* electron = (Electron*) first;
    Wall*     wall     = (Wall*)     second;

    Vec2<float> toWall = wall->getPos()     -
                         electron->getPos() -
                         wall->getDirection() * dotProduct(wall->getPos() - electron->getPos(), 
                                                           wall->getDirection());

    Vec2<float> force = -wall->getElectricField() * toWall * (float) ELECTRON_CHARGE;
    Vec2<float> acceleration = force / electron->getMass();

    electron->setVelocity(electron->getVelocity() + acceleration * deltaTime);
}

void distantInteractWallAtom(PhysEntity* first, PhysEntity* second, float deltaTime)
{
    Wall* wall = (Wall*) first;
    Atom* atom = (Atom*) second;

    Vec2<float> toWall = wall->getPos() -
                         atom->getPos() -
                         wall->getDirection() * dotProduct(wall->getPos() - atom->getPos(), 
                                                           wall->getDirection());

    Vec2<float> force = -wall->getElectricField() * toWall * (float) atom->getCharge();
    Vec2<float> acceleration = force / atom->getMass();

    atom->setVelocity(atom->getVelocity() + acceleration * deltaTime);
}
//------------------------------------------------------------------------------

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

bool collisionDetectElectronElectron(EntitiesIterator first, EntitiesIterator second, 
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

bool collisionDetectElectronWall(EntitiesIterator first, EntitiesIterator second, 
                                 float deltaTime, Collision* collision)
{
    Electron* electron  = (Electron*) *first;
    Wall*     wall      = (Wall*)     *second;

    Vec2<float> point    = electron->getPos();
    Vec2<float> lineFrom = wall->getPos();
    Vec2<float> along    = normalize(wall->getDirection());

    float distance = length((point - lineFrom) - (dotProduct(point - lineFrom, along) * along));

    if (cmpFloat(distance, electron->getRadius()) <= 0)
    {
        collision->first  = first;
        collision->second = second;

        return true;
    }

    return false;
}

bool collisionDetectWallAtom(EntitiesIterator first, EntitiesIterator second, 
                             float deltaTime, Collision* collision)
{
    Wall* wall  = (Wall*) *first;
    Atom* atom  = (Atom*) *second;

    Vec2<float> point    = atom->getPos();
    Vec2<float> lineFrom = wall->getPos();
    Vec2<float> along    = normalize(wall->getDirection());

    float distance = length((point - lineFrom) - (dotProduct(point - lineFrom, along) * along));

    if (cmpFloat(distance, atom->getSize() / 2) <= 0)
    {
        collision->first  = first;
        collision->second = second;

        return true;
    }

    return false;
}

bool collisionDetectAtomAtom(EntitiesIterator first, EntitiesIterator second, 
                             float deltaTime, Collision* collision)
{
    Atom* firstAtom  = (Atom*) *first;
    Atom* secondAtom = (Atom*) *second;

    float firstLeft  = firstAtom->getPos().x  - firstAtom->getSize()  / 2;
    float secondLeft = secondAtom->getPos().x - secondAtom->getSize() / 2;

    float firstRight  = firstAtom->getPos().x  + firstAtom->getSize()  / 2;
    float secondRight = secondAtom->getPos().x + secondAtom->getSize() / 2;

    float firstTop  = firstAtom->getPos().y  - firstAtom->getSize()  / 2;
    float secondTop = secondAtom->getPos().y - secondAtom->getSize() / 2;

    float firstBottom  = firstAtom->getPos().y  + firstAtom->getSize()  / 2;
    float secondBottom = secondAtom->getPos().y + secondAtom->getSize() / 2;

    if (!(cmpFloat(secondLeft, firstRight) > 0 ||
          cmpFloat(secondRight, firstLeft) < 0 || 
          cmpFloat(secondTop, firstBottom) > 0 ||
          cmpFloat(secondBottom, firstTop) < 0))
    {
        collision->first  = first;
        collision->second = second;
        return true;
    }

    return false;
}

bool collisionDetectElectronAtom(EntitiesIterator first, EntitiesIterator second, 
                                 float deltaTime, Collision* collision)
{
    Electron* electron = (Electron*) *first;
    Atom*     atom     = (Atom*)     *second;

    float distanceSquare = lengthSquare(electron->getPos() - atom->getPos());

    float electronRadius = electron->getRadius();
    float atomRadius     = atom->getSize() * (1 + sqrtf(2)) / 4;
    float sumRadiusSquare = electronRadius + atomRadius;
    sumRadiusSquare *= sumRadiusSquare;

    if (cmpFloat(distanceSquare, sumRadiusSquare) <= 0)
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

void collisionRespondElectronElectron(Collision& collision)
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

void collisionRespondElectronWall(Collision& collision)
{
    Electron* electron = (Electron*) *(collision.first);
    Wall*     wall     = (Wall*)     *(collision.second);

    Vec2<float> along = normalize(wall->getDirection());

    float       vAlong         = dotProduct(electron->getVelocity(), along);
    Vec2<float> vPerpendicular = electron->getVelocity() - vAlong * along;

    electron->setVelocity(vAlong * along - vPerpendicular);
}

void collisionRespondWallAtom(Collision& collision)
{
    Wall* wall = (Wall*) *(collision.first);
    Atom* atom = (Atom*) *(collision.second);

    Vec2<float> along = normalize(wall->getDirection());

    float       vAlong         = dotProduct(atom->getVelocity(), along);
    Vec2<float> vPerpendicular = atom->getVelocity() - vAlong * along;

    atom->setVelocity(vAlong * along - vPerpendicular);
}

void collisionRespondAtomAtom(Collision& collision)
{
    Atom* firstAtom  = (Atom*) *(collision.first);
    Atom* secondAtom = (Atom*) *(collision.second);

    Vec2<float> along = normalize(secondAtom->getPos() - firstAtom->getPos());
    
    float v1 = dotProduct(along, firstAtom->getVelocity());
    float v2 = dotProduct(along, secondAtom->getVelocity());
    float m1 = firstAtom->getMass();
    float m2 = secondAtom->getMass();

    float newV1 = (2 * m2 * v2 + v1 * (m1 - m2)) / (m1 + m2);
    float newV2 = (2 * m1 * v1 + v2 * (m2 - m1)) / (m1 + m2);

    Vec2<float> perpendicularVelocity1 = firstAtom->getVelocity()  - v1 * along;
    Vec2<float> perpendicularVelocity2 = secondAtom->getVelocity() - v2 * along;

    firstAtom->setVelocity(perpendicularVelocity1  + newV1 * along);
    secondAtom->setVelocity(perpendicularVelocity2 + newV2 * along);
}

void collisionRespondElectronAtom(Collision& collision)
{
    Electron* electron = (Electron*) *(collision.first);
    Atom*     atom     = (Atom*)     *(collision.second);

    Vec2<float> along = normalize(atom->getPos() - electron->getPos());
    
    float v1 = dotProduct(along, electron->getVelocity());
    float v2 = dotProduct(along, atom->getVelocity());
    float m1 = electron->getMass();
    float m2 = atom->getMass();

    float newV1 = (2 * m2 * v2 + v1 * (m1 - m2)) / (m1 + m2);
    float newV2 = (2 * m1 * v1 + v2 * (m2 - m1)) / (m1 + m2);

    Vec2<float> perpendicularVelocityElectron = electron->getVelocity()  - v1 * along;
    Vec2<float> perpendicularVelocityAtom     = atom->getVelocity() - v2 * along;

    electron->setVelocity(perpendicularVelocityElectron  + newV1 * along);
    atom->setVelocity(perpendicularVelocityAtom + newV2 * along);
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

bool chemicalReactionElectronAtom(List<PhysEntity*>& entities, Collision& collision)
{
    Electron* electron = (Electron*) *(collision.first);
    Atom*     atom     = (Atom*)     *(collision.second);

    if (atom->getEnergy() + electron->getEnergy() < ENERGY_THRESHOLD_ELECTRON_CAPTURE)
    {
        return false;
    }

    atom->setCharge(atom->getCharge() + ELECTRON_CHARGE);
    entities.remove(collision.first);

    return true;
}

bool chemicalReactionAtomAtom(List<PhysEntity*>& entities, Collision& collision)
{
    Atom* firstAtom  = (Atom*) *(collision.first);
    Atom* secondAtom = (Atom*) *(collision.second);

    if (firstAtom->getCharge() == 0 && secondAtom->getCharge() == 0)
    {
        float sumMass = firstAtom->getMass() + secondAtom->getMass();

        if (sumMass >= MASS_THRESHOLD_ATOMS_BREAK)
        {
            float sumVolume = sumMass / ATOM_DENSITY;
            int32_t atomsCount = sumVolume / VOLUME_BROKEN_ATOM;

            float brokenAtomMass = sumMass / atomsCount;

            float mass1     = firstAtom->getMass();
            float mass2     = secondAtom->getMass();
            float velocity1 = length(firstAtom->getVelocity());
            float velocity2 = length(secondAtom->getVelocity());

            float brokenAtomVelocity = sqrtf((mass1 * velocity1 * velocity1 +
                                              mass2 * velocity2 * velocity2) /
                                             (atomsCount * brokenAtomMass));

            for (int32_t i = 0; i < atomsCount; ++i)
            {
                Atom* newAtom = new Atom{RADIUS_BROKEN_ATOM};
                newAtom->setMass(brokenAtomMass);

                float angle = randomFromInterval<float>(0, 2 * M_PI);
                newAtom->setVelocity(Vec2<float>{cosf(angle), sinf(angle)});
                newAtom->setPos(firstAtom->getPos() +
                                2.0f * newAtom->getVelocity() * firstAtom->getSize());

                newAtom->setVelocity(brokenAtomVelocity * newAtom->getVelocity());

                entities.pushBack(newAtom);
            }

            entities.remove(collision.first);
            entities.remove(collision.second);

            return true;
        }
        else if (sumMass >= MASS_THRESHOLD_ATOMS_COMBINE)
        {
            float volume = sumMass / ATOM_DENSITY;

            Atom* newAtom = new Atom{calculateSphereRadiusFromVolume(volume)};
            newAtom->setMass(sumMass);
            newAtom->setPos(firstAtom->getPos());
            newAtom->setVelocity((firstAtom->getMass() * firstAtom->getVelocity() + 
                                  secondAtom->getMass() * secondAtom->getVelocity()) / sumMass);
            entities.pushBack(newAtom);

            entities.remove(collision.first);
            entities.remove(collision.second);

            return true;
        }
    }

    Charge sumCharge = firstAtom->getCharge() + secondAtom->getCharge();

    if (abs(sumCharge) % 2 == 0 && (firstAtom->getCharge() != 0 || secondAtom->getCharge() != 0))
    {
        firstAtom->setCharge(sumCharge  / 2);
        secondAtom->setCharge(sumCharge / 2);
    }

    return false;
}
//------------------------------------------------------------------------------