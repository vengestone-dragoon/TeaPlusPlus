//
// Created by venge on 3/26/25.
//

#ifndef COORDPAIR_H
#define COORDPAIR_H
#include <tuple>



typedef std::tuple<float,float> coordPair;

#endif //COORDPAIR_H

#ifndef IDNUM_H
#define IDNUM_H

typedef unsigned long idNum;

#endif //IDNUM_H

#ifndef TILEMAP_H
#define TILEMAP_H

#include "tileType.h"
#include <vector>

typedef std::vector<std::vector<tileType>> tileMap;

#endif //IDNUM_H

#ifndef LEVELMAP_H
#define LEVELMAP_H

typedef std::tuple<tileMap,int,int,std::string> levelMap;

#endif  //LEVELMAP_H