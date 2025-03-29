//
// Created by venge on 3/26/25.
//

#ifndef DISPLAYBUFFER_H
#define DISPLAYBUFFER_H
#include <string>
#include <vector>
#include <iostream>

#include "customTypes.h"
#include "entityType.h"
#include "tileType.h"
#include "sprite.h"


class displayBuffer {
private:
    int screen_width;
    int screen_height;
    float camX;
    float camY;
    int tileWidth;
    int tileHeight;
    std::vector<std::string> Lines;
    std::vector<std::tuple<entityType,sprite>> Sprites;
    std::vector<std::tuple<entityType,float,float>> entities;//entity,x,y
    tileMap tile_map;
    std::vector<std::tuple<tileType,sprite>> tileSprites;
    bool currentScreen;
    std::string screen0;
    std::string screen1;

public:
    displayBuffer();;

    displayBuffer(int screen_width, int screen_height);;

    void SetCursorPosition(int x, int y);;

    void addSprite(entityType ent,sprite& spr);

    void addTileSprite(tileType type,sprite& spr);

    void setTileMap(tileMap tile_map);

    void setTileSize(int tile_width, int tile_height);

    idNum addEntity(entityType ent, float x, float y);

    void removeEntity(idNum id);

    void moveEntity(idNum id, float x, float y);

    void drawEntities();

    void drawTiles();

    void drawSprite(sprite spr, float x, float y);

    void clearBuffer();

    void drawScreen();;

    void moveCamera(float x, float y);

    coordPair getCameraPosition();
};



#endif //DISPLAYBUFFER_H
