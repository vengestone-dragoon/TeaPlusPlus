//
// Created by venge on 3/27/25.
//
#include "levelLoader.h"

#include <algorithm>


levelMap loadLevel(std::string fileName) {
    std::ifstream file(fileName);
    int width=0, height=0;
    int tileWidth=0, tileHeight=0;
    std::string nextLevel="";
    tileMap tmap;
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << fileName << std::endl;
    } else {
        while (file.good() && !file.eof() && (width==0 || height==0 || nextLevel=="" || tileWidth==0 || tileHeight==0)) {
            std::string line;
            getline(file, line);
            if (line.substr(0,6) == "width=") {
                width = std::stoi(line.substr(6));
            } else if (line.substr(0,7) == "height=") {
                height = std::stoi(line.substr(7));
            } else if (line.substr(0,10) == "nextLevel=") {
                nextLevel = line.substr(10);
            } else if (line.substr(0,10) == "tileWidth=") {
                tileWidth = std::stoi(line.substr(10));
            } else if (line.substr(0,11) == "tileHeight=") {
                tileHeight = std::stoi(line.substr(11));
            } else {
                std::cerr << "Invalid level file: " << fileName << std::endl;
                std::cerr << "here: " << line << std::endl;
                throw std::runtime_error("Invalid level file");
            }
        }
        if (width != 0 && height != 0) {
            for (int y = 0; y < height; y++) {
                if (file.good() && !file.eof()) {
                    std::string line;
                    std::string subline;
                    getline(file, line);
                    std::stringstream ss(line.substr(1,line.length()-2));
                    std::vector<tileType> idLine;
                    while (std::getline(ss, subline, ',')) {
                        try {
                            int id = std::stoi(subline);
                            idLine.emplace_back(static_cast<tileType>(id));
                        } catch (...) {
                            std::cerr << "Invalid level file: " << fileName << std::endl;
                        }
                    };
                    if (idLine.size() == width) {
                        tmap.emplace_back(idLine);
                    }
                } else {
                    std::cerr << "Invalid sprite file: " <<  fileName << std::endl;
                    std::cerr << "reached end of file early" << std::endl;
                    throw std::invalid_argument("Invalid sprite file");
                }
            }
        }else {
            std::cerr << "Invalid sprite file: " <<  fileName << std::endl;
            std::cerr << "couldn't find sprite info." << std::endl;
            throw std::invalid_argument("Invalid sprite file");
        }
    }
    return std::make_tuple(tmap,tileWidth,tileHeight,nextLevel);
}
