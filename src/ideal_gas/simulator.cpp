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

typedef void (*FDistantInteract) (PhysEntity* first, PhysEntity* secondEntity, float deltaTime);
typedef bool (*FCollisionDetect) (EntitiesIterator first, EntitiesIterator secondEntity, 
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

Collision::Collision(const EntitiesIterator& firstEntity, const EntitiesIterator& secondEntity) : 
                     firstEntity(firstEntity), secondEntity(secondEntity) {}

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

    for (EntitiesIterator firstEntity = entities.begin(); firstEntity != entities.end(); ++firstEntity)
    {
        for (EntitiesIterator secondEntity = firstEntity; (++secondEntity) != entities.end();)
        {
            Collision collision{entities.begin(), entities.begin()};
            
            if (collisionDetect(firstEntity, secondEntity, deltaTime, &collision))
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

    for (EntitiesIterator firstEntity = entities.begin(); firstEntity != entities.end(); ++firstEntity)
    {
        for (EntitiesIterator secondEntity = firstEntity; (++secondEntity) != entities.end();)
        {
            distantInteract(*firstEntity, *secondEntity, deltaTime);
        }
    }

    for (PhysEntity* entity : entities)
    {
        entity->move(deltaTime);
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
void Simulator::distantInteract(PhysEntity* firstEntity, PhysEntity* secondEntity, float deltaTime)
{
    FDistantInteract function = INTERACTIONS[firstEntity->type][secondEntity->type].distantInteract;
    if (function == nullptr)
    {
        return;
    }

    if (firstEntity->type > secondEntity->type)
    {
        function(secondEntity, firstEntity, deltaTime);
    }
    else
    {
        function(firstEntity, secondEntity, deltaTime);
    }
}

void distantInteractElectronWall(PhysEntity* firstEntity, PhysEntity* secondEntity, float deltaTime)
{
    Electron* electron = (Electron*) firstEntity;
    Wall*     wall     = (Wall*)     secondEntity;

    Vec2<float> toWall = wall->getPos()     -
                         electron->getPos() -
                         wall->getDirection() * dotProduct(wall->getPos() - electron->getPos(), 
                                                           wall->getDirection());

    Vec2<float> force = -wall->getElectricField() * toWall * (float) ELECTRON_CHARGE;
    Vec2<float> acceleration = force / electron->getMass();

    electron->setVelocity(electron->getVelocity() + acceleration * deltaTime);
}

void distantInteractWallAtom(PhysEntity* firstEntity, PhysEntity* secondEntity, float deltaTime)
{
    Wall* wall = (Wall*) firstEntity;
    Atom* atom = (Atom*) secondEntity;

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
bool Simulator::collisionDetect(EntitiesIterator firstEntity, EntitiesIterator secondEntity, 
                                float deltaTime, Collision* collision)
{
    FCollisionDetect function = INTERACTIONS[(*firstEntity)->type][(*secondEntity)->type].collisionDetect;
    if (function == nullptr)
    {
        return false;
    }

    if ((*firstEntity)->type > (*secondEntity)->type)
    {
        return function(secondEntity, firstEntity, deltaTime, collision);
    }

    return function(firstEntity, secondEntity, deltaTime, collision);
}

bool collisionDetectElectronElectron(EntitiesIterator firstEntity, EntitiesIterator secondEntity, 
                                     float deltaTime, Collision* collision)
{
    Electron* firstElectron  = (Electron*) *firstEntity;
    Electron* secondElectron = (Electron*) *secondEntity;

    float distanceSquare  = lengthSquare(firstElectron->getPos() - secondElectron->getPos());
    float sumRadiusSquare = firstElectron->getRadius() + secondElectron->getRadius();
    sumRadiusSquare *= sumRadiusSquare;

    if (cmpFloat(distanceSquare, sumRadiusSquare) <= 0)
    {
        collision->firstEntity  = firstEntity;
        collision->secondEntity = secondEntity;
        return true;
    }

    return false;
}

bool collisionDetectElectronWall(EntitiesIterator firstEntity, EntitiesIterator secondEntity, 
                                 float deltaTime, Collision* collision)
{
    Electron* electron  = (Electron*) *firstEntity;
    Wall*     wall      = (Wall*)     *secondEntity;

    Vec2<float> point    = electron->getPos();
    Vec2<float> lineFrom = wall->getPos();
    Vec2<float> along    = normalize(wall->getDirection());

    float distance = length((point - lineFrom) - (dotProduct(point - lineFrom, along) * along));

    if (cmpFloat(distance, electron->getRadius()) <= 0)
    {
        collision->firstEntity  = firstEntity;
        collision->secondEntity = secondEntity;

        return true;
    }

    return false;
}

bool collisionDetectWallAtom(EntitiesIterator firstEntity, EntitiesIterator secondEntity, 
                             float deltaTime, Collision* collision)
{
    Wall* wall  = (Wall*) *firstEntity;
    Atom* atom  = (Atom*) *secondEntity;

    Vec2<float> point    = atom->getPos();
    Vec2<float> lineFrom = wall->getPos();
    Vec2<float> along    = normalize(wall->getDirection());

    float distance = length((point - lineFrom) - (dotProduct(point - lineFrom, along) * along));

    if (cmpFloat(distance, atom->getSize() / 2) <= 0)
    {
        collision->firstEntity  = firstEntity;
        collision->secondEntity = secondEntity;

        return true;
    }

    return false;
}

bool collisionDetectAtomAtom(EntitiesIterator firstEntity, EntitiesIterator secondEntity, 
                             float deltaTime, Collision* collision)
{
    Atom* firstAtom  = (Atom*) *firstEntity;
    Atom* secondAtom = (Atom*) *secondEntity;

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
        collision->firstEntity  = firstEntity;
        collision->secondEntity = secondEntity;
        return true;
    }

    return false;
}

bool collisionDetectElectronAtom(EntitiesIterator firstEntity, EntitiesIterator secondEntity, 
                                 float deltaTime, Collision* collision)
{
    Electron* electron = (Electron*) *firstEntity;
    Atom*     atom     = (Atom*)     *secondEntity;

    float distanceSquare = lengthSquare(electron->getPos() - atom->getPos());

    float electronRadius = electron->getRadius();
    float atomRadius     = atom->getSize() * (1 + sqrtf(2)) / 4;
    float sumRadiusSquare = electronRadius + atomRadius;
    sumRadiusSquare *= sumRadiusSquare;

    if (cmpFloat(distanceSquare, sumRadiusSquare) <= 0)
    {
        collision->firstEntity  = firstEntity;
        collision->secondEntity = secondEntity;
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------

//-------------------------------Collision respond------------------------------
void Simulator::collisionRespond(Collision& collision)
{
    FCollisionRespond function = INTERACTIONS[(*collision.firstEntity)->type][(*collision.secondEntity)->type]
                                             .collisionRespond;
    if (function != nullptr)
    {
        if ((*collision.firstEntity)->type > (*collision.secondEntity)->type)
        {
            EntitiesIterator tmp = collision.firstEntity;
            collision.firstEntity = collision.secondEntity;
            collision.secondEntity = tmp;
        }

        function(collision);
    }
}

void collisionRespondElectronElectron(Collision& collision)
{
    Electron* firstElectron  = (Electron*) *(collision.firstEntity);
    Electron* secondElectron = (Electron*) *(collision.secondEntity);

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
    Electron* electron = (Electron*) *(collision.firstEntity);
    Wall*     wall     = (Wall*)     *(collision.secondEntity);

    Vec2<float> along = normalize(wall->getDirection());

    float       vAlong         = dotProduct(electron->getVelocity(), along);
    Vec2<float> vPerpendicular = electron->getVelocity() - vAlong * along;

    // Vec2<float> toWall = electron->getPos() - wall->getPos() - 
    //                      dotProduct(electron->getPos() - wall->getPos(), along) * along;

    // vPerpendicular += toWall * length(electron->getVelocity()) * WALL_BOUNCE_VELOCITY_PERCENTAGE;

    electron->setVelocity(vAlong * along - vPerpendicular);
}

void collisionRespondWallAtom(Collision& collision)
{
    Wall* wall = (Wall*) *(collision.firstEntity);
    Atom* atom = (Atom*) *(collision.secondEntity);

    Vec2<float> along = normalize(wall->getDirection());

    float       vAlong         = dotProduct(atom->getVelocity(), along);
    Vec2<float> vPerpendicular = atom->getVelocity() - vAlong * along;

    // Vec2<float> toWall = atom->getPos() - wall->getPos() - 
    //                      dotProduct(atom->getPos() - wall->getPos(), along) * along;

    // vPerpendicular += toWall * length(atom->getVelocity()) * WALL_BOUNCE_VELOCITY_PERCENTAGE;

    atom->setVelocity(vAlong * along - vPerpendicular);
}

void collisionRespondAtomAtom(Collision& collision)
{
    Atom* firstAtom  = (Atom*) *(collision.firstEntity);
    Atom* secondAtom = (Atom*) *(collision.secondEntity);

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
    Electron* electron = (Electron*) *(collision.firstEntity);
    Atom*     atom     = (Atom*)     *(collision.secondEntity);

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
    FChemicalReaction function = INTERACTIONS[(*collision.firstEntity)->type][(*collision.secondEntity)->type]
                                             .chemicalReaction;
    if (function == nullptr)
    {
        return false;
    }

    if ((*collision.firstEntity)->type > (*collision.secondEntity)->type)
    {
        EntitiesIterator tmp = collision.firstEntity;
        collision.firstEntity = collision.secondEntity;
        collision.secondEntity = tmp;
    }

    return function(entities, collision); 
}

bool chemicalReactionElectronAtom(List<PhysEntity*>& entities, Collision& collision)
{
    Electron* electron = (Electron*) *(collision.firstEntity);
    Atom*     atom     = (Atom*)     *(collision.secondEntity);

    if (atom->getEnergy() + electron->getEnergy() < ENERGY_THRESHOLD_ELECTRON_CAPTURE)
    {
        return false;
    }

    atom->setCharge(atom->getCharge() + ELECTRON_CHARGE);
    entities.remove(collision.firstEntity);

    return true;
}

bool chemicalReactionAtomAtom(List<PhysEntity*>& entities, Collision& collision)
{
    Atom* firstAtom  = (Atom*) *(collision.firstEntity);
    Atom* secondAtom = (Atom*) *(collision.secondEntity);

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

            entities.remove(collision.firstEntity);
            entities.remove(collision.secondEntity);

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

            entities.remove(collision.firstEntity);
            entities.remove(collision.secondEntity);

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