//
// Created by venge on 3/30/25.
//
#include "tileType.h"

bool tileType_is_solid(tileType type) {
    switch (type) {
        case tileType::ground: return true;
        default : return false;
    }
};