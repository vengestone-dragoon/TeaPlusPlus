//
// Created by venge on 3/26/25.
//

#ifndef PHYSICSENTITY_H
#define PHYSICSENTITY_H



class physicsEntity {
public:
    physicsEntity(float X, float Y, float accel, float vLimit, int width, int height, int health_max, float weight);

    int health;
    int health_max;
    float accel;
    float vLimit;
    float width;
    float height;
    float X;
    float Y;
    float vX;
    float vY;
    float aX;
    float aY;
    float weight;
};




#endif //PHYSICSENTITY_H
