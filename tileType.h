//
// Created by venge on 3/27/25.
//

#ifndef TILETYPE_H
#define TILETYPE_H

enum tileType {
    air,
    ground,
    playerSpawn,
    enemySpawner,
    endGoal,
    camStart,

};

bool tileType_is_solid(tileType type);

#endif //TILETYPE_H
