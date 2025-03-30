//
// Created by venge on 3/26/25.
//

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H
#include <vector>

#include "displayBuffer.h"
#include "entityType.h"
#include "physicsEntity.h"
#include "customTypes.h"
#include <tuple>
#include "levelLoader.h"

class physicsEngine {
private:
    float gravity;
    displayBuffer* display;
    std::vector<std::tuple<physicsEntity,idNum>> entities;
    levelMap level;
public:
    physicsEngine();

    physicsEngine (float gravity,displayBuffer* display);

    void initLevel(std::string fileName);

    idNum spawnEntity(entityType type, float x, float y, float accel, float vLimit, int width, int height, int health, float weight);

    void despawnEntity(idNum id);

    coordPair getEntityPosition(idNum id);

    coordPair getEntityVelocity(idNum id);

    coordPair getEntityAcceleration(idNum id);

    void setEntityPosition(idNum id, coordPair position);

    void setEntityVelocity(idNum id, coordPair vel);

    void accelerateEntity(idNum id, coordPair acc);

    void setEntityAccel(idNum id, coordPair acc);

    void runPhysics();

    void entityTileCollision(idNum id);
};



#endif //PHYSICSENGINE_H
