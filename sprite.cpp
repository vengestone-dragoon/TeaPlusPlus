//
// Created by venge on 3/26/25.
//

#include "sprite.h"

#include <iostream>

sprite::sprite() {
    width=height=0;
}

sprite::sprite(int entity_width, int entity_height) {
    width = entity_width;
    height = entity_height;
}

sprite::sprite(std::string filePath) {
    std::ifstream file(filePath);
    int frameCount;
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filePath << std::endl;
        throw std::runtime_error("Could not open file");
    } else {
        while (file.good() && !file.eof() && (frameCount==0 || width==0 || height==0)) {
            std::string line;
            getline(file, line);
            if (line.substr(0,6) == "width=") {
                width = std::stoi(line.substr(6));
            } else if (line.substr(0,7) == "height=") {
                height = std::stoi(line.substr(7));
            } else if (line.substr(0,7) == "frames=") {
                frameCount = std::stoi(line.substr(7));
            } else {
                std::cerr << "Invalid sprite file: " << filePath << std::endl;
                std::cerr << "here: " << line << std::endl;
                throw std::runtime_error("Invalid sprite file");
            }
        }
        for (int i = 0; i < frameCount; i++) {
            frames.emplace_back();
            for (int j = 0; j < height; j++) {
                frames[i].emplace_back();
            }
        };
        if (frameCount != 0 && width != 0 && height != 0) {
            for (int f = 0; f < frameCount; f++) {
                for (int y = 0; y < height; y++) {
                    if (file.good() && !file.eof()) {
                        std::string line;
                        getline(file, line);
                        if (line.size() == width+2) {
                            frames[f][y] = line.substr(1,width);
                        } else {
                            std::cerr << "Invalid sprite file: " << filePath << std::endl;
                            std::cerr << "line width: " << line.size() << std::endl;
                            std::cerr << "required width: " << width << std::endl;
                            std::cerr << "here: " << line << std::endl;
                            throw std::invalid_argument("Invalid sprite file");
                        }
                    } else {
                        std::cerr << "Invalid sprite file: " << filePath << std::endl;
                        std::cerr << "reached end of file early" << std::endl;
                        std::cerr << "width: " << width << "  height: " << height << "  frames: " << frameCount << std::endl;
                        std::cerr << "failed at frame:" << f << "line: " << y << std::endl;
                        throw std::invalid_argument("Invalid sprite file");
                    }

                }
            }
        }else {
            std::cerr << "Invalid sprite file: " << filePath << std::endl;
            std::cerr << "couldn't find sprite info." << std::endl;
            throw std::invalid_argument("Invalid sprite file");
        }

    }
}

void sprite::add_frame(const std::vector<std::string> &frame) {
    frames.push_back(frame);
}