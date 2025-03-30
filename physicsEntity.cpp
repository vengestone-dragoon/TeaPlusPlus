//
// Created by venge on 3/26/25.
//

#include "physicsEntity.h"

physicsEntity::physicsEntity(float X, float Y, float accel, float vLimit, int width, int height, int health_max, float weight) {
    this->X = X;
    this->Y = Y;
    this->accel = accel;
    this->vLimit = vLimit;
    this->width = static_cast<float>(width);
    this->height = static_cast<float>(height);
    this->health_max = health_max;
    this->weight = weight;
    vX=0;
    vY=0;
    aX=0;
    aY=0;
    health=health_max;
}