//
// Created by venge on 3/26/25.
//

#include "physicsEngine.h"

#include <strings.h>


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


idNum physicsEngine::spawnEntity(entityType type, float x, float y, float accel, float vLimit, int width, int height, int health, float weight) {
    idNum displayId=display->addEntity(type,x,y);
    physicsEntity e(x,y,accel,vLimit,width,height,health,weight);
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
    for (int index=0; index<entities.size(); index++) {
        auto id = static_cast<idNum>(index);
        auto& e = entities[index];
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
        entityTileCollision(id);
        display->moveEntity(displayId,ent.X,ent.Y);
    }
}

void physicsEngine::entityTileCollision(idNum id) {
    physicsEntity& ent = std::get<0>(entities[id]);
    tileMap map = std::get<0>(level);
    auto ftilewidth = static_cast<float>(std::get<1>(level));
    auto ftileheight = static_cast<float>(std::get<2>(level));
    auto entLeftI = static_cast<int>(ent.X/ftilewidth);
    auto entRightI = static_cast<int>((ent.X+ent.width)/ftilewidth);
    auto entTopI = static_cast<int>(ent.Y/ftileheight);
    auto entBottomI = static_cast<int>((ent.Y+ent.height)/ftileheight);
    auto mapY = static_cast<float>(map.size());
    auto mapX = static_cast<float>(map[0].size());
    if (entRightI >= 0 && entBottomI >= 0 && entLeftI <= mapX && entTopI <= mapY) {
        for (int y=entTopI-1; y<=entBottomI+1; y++) {
            for (int x=entLeftI-1; x<=entRightI+1; x++) {
                if (y>mapY-1||x>mapX-1||y<0||x<0) {
                    break;
                }
                tileType tile = map[y][x];
                if (tileType_is_solid(tile)) {
                    auto tileTop = static_cast<float>(y)*ftileheight;
                    auto tileBottom = static_cast<float>(std::get<2>(level)) + tileTop;
                    auto tileCenterY = (static_cast<float>(std::get<2>(level))/2) + tileTop;
                    auto tileLeft = static_cast<float>(x)*ftilewidth;
                    auto tileRight = static_cast<float>(std::get<1>(level)) + tileLeft;
                    auto tileCenterX = (static_cast<float>(std::get<1>(level))/2) + tileLeft;

                    float entTop = ent.Y;
                    float entBottom = ent.Y+ent.height;
                    float entCenterY = (ent.height/2) + ent.Y;
                    float entLeft = ent.X;
                    float entRight = ent.X+ent.width;
                    float entCenterX = (ent.width/2) + ent.X;

                    float intersectTop = entBottom - tileTop;
                    float intersectBottom = tileBottom - entTop;
                    float intersectLeft = entRight - tileLeft;
                    float intersectRight = tileRight - entLeft;

                    if (entCenterY>tileCenterY && entCenterX>tileCenterX) {
                        if (intersectBottom > 0 && intersectRight > 0) {
                            if (intersectBottom < intersectRight) {
                                ent.Y += intersectBottom;
                                setEntityVelocity(id,std::make_tuple(ent.vX,0));
                            } else if (intersectRight < intersectBottom) {
                                ent.X += intersectRight;
                                setEntityVelocity(id,std::make_tuple(0,ent.vY));
                            }
                        }
                    } else if (entCenterY<tileCenterY && entCenterX<tileCenterX) {
                        if (intersectTop > 0 && intersectLeft > 0) {
                            if (intersectTop < intersectLeft) {
                                ent.Y -= intersectTop;
                                setEntityVelocity(id,std::make_tuple(ent.vX,0));
                            } else if (intersectLeft < intersectTop) {
                                ent.X -= intersectLeft;
                                setEntityVelocity(id,std::make_tuple(0,ent.vY));
                            }
                        }
                    } else if (entCenterX>tileCenterX && entCenterY<tileCenterY) {
                        if (intersectTop > 0 && intersectRight > 0) {
                            if (intersectTop < intersectRight) {
                                ent.Y -= intersectTop;
                                setEntityVelocity(id,std::make_tuple(ent.vX,0));
                            } else if (intersectRight < intersectTop) {
                                ent.X += intersectRight;
                                setEntityVelocity(id,std::make_tuple(0,ent.vY));
                            }
                        }
                    } else if (entCenterX<tileCenterX && entCenterY>tileCenterY) {
                        if (intersectBottom > 0 && intersectLeft > 0) {
                            if (intersectBottom < intersectLeft) {
                                ent.Y += intersectBottom;
                                setEntityVelocity(id,std::make_tuple(ent.vX,0));
                            } else if (intersectLeft < intersectBottom) {
                                ent.X -= intersectLeft;
                                setEntityVelocity(id,std::make_tuple(0,ent.vY));
                            }
                        }
                    } else if (entCenterY==tileCenterY) {
                        if (intersectLeft > 0) {
                            ent.X -= intersectLeft;
                            setEntityVelocity(id,std::make_tuple(0,ent.vY));
                        } else if (intersectRight > 0) {
                            ent.X += intersectRight;
                            setEntityVelocity(id,std::make_tuple(0,ent.vY));
                        }
                    } else if (entCenterX==tileCenterX) {
                        if (intersectTop > 0) {
                            ent.Y -= intersectTop;
                            setEntityVelocity(id,std::make_tuple(ent.vX,0));
                        } else if (intersectBottom > 0) {
                            ent.Y += intersectBottom;
                            setEntityVelocity(id,std::make_tuple(ent.vX,0));
                        }
                    }
                }
            }
        }
    }
}
