//
// Created by venge on 3/26/25.
//

#include "physicsEngine.h"


physicsEngine::physicsEngine() {
    gravity = 1;
    display = new displayBuffer();
}

physicsEngine::physicsEngine (float gravity,displayBuffer* display) {
    this->gravity = gravity;
    this->display = display;
};

void physicsEngine::initLevel(std::string fileName) {
    level = loadLevel(fileName);
    display->setTileMap(std::get<0>(level));
    display->setTileSize(std::get<1>(level),std::get<2>(level));
}


idNum physicsEngine::spawnEntity(entityType type, float x, float y, float accel, float vLimit, int health, float weight) {
    idNum displayId=display->addEntity(type,x,y);
    physicsEntity e(x,y,accel,vLimit,health,weight);
    entities.emplace_back(e,displayId);
    return entities.size()-1;
}

void physicsEngine::despawnEntity(idNum id) {
    idNum displayId= std::get<1>(entities[id]);
    display->removeEntity(displayId);
    entities.erase(entities.begin()+static_cast<long>(id));
}

coordPair physicsEngine::getEntityPosition(idNum id) {
    float x = std::get<0>(entities[id]).X;
    float y = std::get<0>(entities[id]).Y;
    return std::make_tuple(x,y);
}

coordPair physicsEngine::getEntityVelocity(idNum id) {
    float vx = std::get<0>(entities[id]).vX;
    float vy = std::get<0>(entities[id]).vY;
    return std::make_tuple(vx,vy);
}

coordPair physicsEngine::getEntityAcceleration(idNum id) {
    float ax = std::get<0>(entities[id]).aX;
    float ay = std::get<0>(entities[id]).aY;
    return std::make_tuple(ax,ay);
}

void physicsEngine::setEntityPosition(idNum id, coordPair position) {
    physicsEntity& ent = std::get<0>(entities[id]);
    idNum displayId = std::get<1>(entities[id]);
    ent.X = std::get<0>(position);
    ent.Y = std::get<1>(position);
    display->moveEntity(displayId,std::get<0>(position),std::get<1>(position));
}

void physicsEngine::setEntityVelocity(idNum id, coordPair vel) {
    physicsEntity& ent = std::get<0>(entities[id]);
    ent.vX = std::get<0>(vel);
    ent.vY = std::get<1>(vel);
}

void physicsEngine::accelerateEntity(idNum id, coordPair acc) {
    physicsEntity& ent = std::get<0>(entities[id]);
    float aX = std::get<0>(acc);
    float aY = std::get<1>(acc);
    if (ent.vX < ent.vLimit && aX>0) {
        ent.aX += aX * ent.accel;
    } else if (ent.vX > (-ent.vLimit) && aX<0) {
        ent.aX += aX * ent.accel;
    }
    if (ent.vY < ent.vLimit && aY>0) {
        ent.aY += aY * ent.accel;
    } else if (ent.vY > (-ent.vLimit) && aY<0) {
        ent.aY += aY * ent.accel;
    }
}

void physicsEngine::setEntityAccel(idNum id, coordPair acc) {
    physicsEntity& ent = std::get<0>(entities[id]);
    ent.aX = std::get<0>(acc);
    ent.aY = std::get<1>(acc);
}

void physicsEngine::runPhysics() {
    for (std::tuple<physicsEntity,idNum>& e : entities) {
        physicsEntity& ent = std::get<0>(e);
        idNum displayId = std::get<1>(e);
        float accel = ent.accel;
        float vLimit = ent.vLimit;
        float X = ent.X;
        float Y = ent.Y;
        float vX = ent.vX;
        float vY = ent.vY;
        float aX = ent.aX;
        float aY = ent.aY;
        float weight = ent.weight;
        //calculate physics
        aY += weight * gravity;
        vY += aY/60;
        vX += aX/60;
        X += vX/60;
        Y += vY/60;
        //decelerate
        aX = 0;
        aY = 0;
        if (vX>0) {
            vX -= 0.0001 * weight;
        } else if (vX<0) {
            vX += 0.0001 * weight;
        };
        if (vY>0) {
            vY -= 0.0001 * weight;
        } else if (vY<0) {
            vY += 0.0001 * weight;
        }
        ent.X = X;
        ent.Y = Y;
        ent.vX = vX;
        ent.vY = vY;
        ent.aX = aX;
        ent.aY = aY;
        display->moveEntity(displayId,X,Y);
    }
}

void physicsEngine::entityTileCollision(idNum id) {
    physicsEntity& ent = std::get<0>(entities[id]);

}
