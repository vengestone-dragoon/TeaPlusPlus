//
// Created by venge on 3/26/25.
//

#include "displayBuffer.h"

displayBuffer::displayBuffer() {
    screen_width = 200;
    screen_height = 30;
    tileWidth = 0;
    tileHeight = 0;
    camX = 0;
    camY = 0;
    currentScreen = false;
    screen0 = "";
    screen1 = "";
    clearBuffer();
}

displayBuffer::displayBuffer(int screen_width, int screen_height) {
    this->screen_width=screen_width;
    this->screen_height=screen_height;
    tileWidth = 0;
    tileHeight = 0;
    camX = 0;
    camY = 0;
    currentScreen = false;
    screen0 = "";
    screen1 = "";
    clearBuffer();
}

void displayBuffer::SetCursorPosition(int x, int y) {
    std::cout<<"\033["<<y<<";"<<x<<"f";//linux set cursor position to 0,0
}

void displayBuffer::addSprite(entityType ent, sprite &spr) {
    Sprites.emplace_back(ent,spr);
}

void displayBuffer::addTileSprite(tileType type,sprite& spr) {
    if (spr.height == tileHeight && spr.width == tileWidth) {
    tileSprites.emplace_back(type,spr);
    }
};

void displayBuffer::setTileMap(tileMap tile_map) {
    this->tile_map = tile_map;
};

void displayBuffer::setTileSize(int tile_width, int tile_height) {
    this->tileWidth = tile_width;
    this->tileHeight = tile_height;
}

idNum displayBuffer::addEntity(entityType ent, float x, float y) {
    entities.emplace_back(ent,x,y);
    return entities.size()-1;
}

void displayBuffer::removeEntity(idNum id) {
    entities.erase(entities.begin()+static_cast<long>(id));
}

void displayBuffer::moveEntity(idNum id, float x, float y) {
    entities[id]={std::get<0>(entities[id]),x,y};
}

void displayBuffer::drawEntities() {
    auto bWidth = static_cast<float>(screen_width);
    auto bHeight = static_cast<float>(screen_height);
    float topLeftX = camX;
    float topLeftY = camY;
    float bottomRightX = camX + bWidth;
    float bottomRightY = camY + bHeight;

    for (std::tuple<entityType,float,float> e : entities) {
        entityType en = std::get<0>(e);
        float x = std::get<1>(e);
        float y = std::get<2>(e);
        sprite sp;

        auto sprWidth = static_cast<float>(sp.width);
        auto sprHeight = static_cast<float>(sp.height);


        for(std::tuple<entityType,sprite> spr : Sprites) {
            if (std::get<0>(spr) == en) {
                sp = std::get<1>(spr);
                break;
            };
        };
        drawSprite(sp,x,y);
        // auto nx = static_cast<unsigned long>(x);
        // auto ny = static_cast<unsigned long>(y);
        // auto cx = static_cast<unsigned long>(camX);
        // auto cy = static_cast<unsigned long>(camY);
        //
        // if (x >= topLeftX && y >= topLeftY && x + sprWidth < bottomRightX && y + sprHeight < bottomRightY) {
        //     for (int i = 0; i < sp.height; i++) {
        //         auto ni = static_cast<unsigned long>(i);
        //         Lines[ny + ni - cy].replace(nx-cx, sp.width, sp.frames[0][i]);
        //     }
        // } else if (x + sprWidth >= topLeftX && x <= bottomRightX && y + sprHeight >= topLeftY && y <= bottomRightY) {
        //     for (int i = 0; i < sp.height; i++) {
        //         auto ni = static_cast<unsigned long>(i);
        //         auto fi = static_cast<float>(i);
        //         if (y + fi >= topLeftY && y + fi < bottomRightY) {
        //             for (int j = 0; j < sp.width; j++) {
        //                 auto nj = static_cast<unsigned long>(j);
        //                 auto fj = static_cast<float>(j);
        //                 if (x + fj >= topLeftX && x + fj < bottomRightX) {
        //                     Lines[ny + ni - cy].replace(nx + nj - cx, 1, sp.frames[0][i].substr(nj,1));
        //                 }
        //             }
        //         }
        //     }
        // }


    }
}

void displayBuffer::drawTiles() {
    int topTileEdge = static_cast<int>(camY/static_cast<float>(tileHeight));
    int leftTileEdge = static_cast<int>(camX/static_cast<float>(tileWidth));
    int bottomTileEdge = static_cast<int>((camY+static_cast<float>(screen_height))/static_cast<float>(tileHeight));
    int rightTileEdge = static_cast<int>((camX+static_cast<float>(screen_width))/static_cast<float>(tileWidth));
    for (int y = topTileEdge; y <= bottomTileEdge; y++) {
        if (tile_map.size()-1 >= y && y >= 0) {
            for (int x = leftTileEdge; x <= rightTileEdge; x++) {
                if (tile_map[y].size()-1 >= x && x >= 0) {
                    tileType tile = tile_map[y][x];
                    sprite sp;
                    for(std::tuple<tileType,sprite> spr : tileSprites) {
                        if (std::get<0>(spr) == tile) {
                            sp = std::get<1>(spr);
                            break;
                        };
                    };
                    float X = static_cast<float>(x*tileWidth);
                    float Y = static_cast<float>(y*tileHeight);
                    drawSprite(sp,X,Y);
                }
            }
        }

    }
}

void displayBuffer::drawSprite(sprite spr, float x, float y) {
    auto bWidth = static_cast<float>(screen_width);
    auto bHeight = static_cast<float>(screen_height);
    float topLeftX = camX;
    float topLeftY = camY;
    float bottomRightX = camX + bWidth;
    float bottomRightY = camY + bHeight;

    auto nx = static_cast<unsigned long>(x);
    auto ny = static_cast<unsigned long>(y);
    auto cx = static_cast<unsigned long>(camX);
    auto cy = static_cast<unsigned long>(camY);

    auto sprWidth = static_cast<float>(spr.width);
    auto sprHeight = static_cast<float>(spr.height);

    if (x >= topLeftX && y >= topLeftY && x + sprWidth < bottomRightX && y + sprHeight < bottomRightY) {
        for (int i = 0; i < spr.height; i++) {
            auto ni = static_cast<unsigned long>(i);
            Lines[ny + ni - cy].replace(nx-cx, spr.width, spr.frames[0][i]);
        }
    } else if (x + sprWidth >= topLeftX && x <= bottomRightX && y + sprHeight >= topLeftY && y <= bottomRightY) {
        for (int i = 0; i < spr.height; i++) {
            auto ni = static_cast<unsigned long>(i);
            auto fi = static_cast<float>(i);
            if (y + fi >= topLeftY && y + fi < bottomRightY) {
                for (int j = 0; j < spr.width; j++) {
                    auto nj = static_cast<unsigned long>(j);
                    auto fj = static_cast<float>(j);
                    if (x + fj >= topLeftX && x + fj < bottomRightX) {
                        Lines[ny + ni - cy].replace(nx + nj - cx, 1, spr.frames[0][i].substr(nj,1));
                    }
                }
            }
        }
    }
}


void displayBuffer::clearBuffer() {
    Lines.clear();
    for (int i=0; i < screen_height; i++) {
        Lines.emplace_back("");
        for (int j=0; j < screen_width; j++) {
            Lines[i].append(" ");
        }
    };
}

void displayBuffer::drawScreen() {
    SetCursorPosition(0,0);
    std::string* cScreen;
    if (currentScreen) {
        cScreen = &screen1;
    } else {
        cScreen = &screen0;
    };
    cScreen->clear();
    drawTiles();
    drawEntities();
    for (const std::string& line: Lines) {
        cScreen->append(line);
        cScreen->append("\n");
    };
    std::cout << cScreen->c_str();
    currentScreen = !currentScreen;
    clearBuffer();
}

void displayBuffer::moveCamera(float x, float y) {
    camX = x;
    camY = y;
}

coordPair displayBuffer::getCameraPosition() {
    coordPair ret = std::make_tuple(camX, camY);
    return ret;
}