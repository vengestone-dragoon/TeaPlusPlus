//
// Created by venge on 3/26/25.
//

#ifndef SPRITE_H
#define SPRITE_H
#include <string>
#include <vector>
#include <fstream>


class sprite {
public:
    std::vector<std::vector<std::string>> frames;
    int width;
    int height;
    sprite();

    sprite(int entity_width, int entity_height);

    sprite(std::string filePath);

    void add_frame(const std::vector<std::string>& frame);
};


#endif //SPRITE_H
