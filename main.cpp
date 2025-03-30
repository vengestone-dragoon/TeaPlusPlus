#include <iostream>
#include <cstdio>
#include <cstring>
#include <thread>
#include <vector>
#include <tuple>
#include <termios.h>
#include <unistd.h>

#include "entityType.h"
#include "sprite.h"
#include "physicsEntity.h"
#include "displayBuffer.h"
#include "physicsEngine.h"
#include "customTypes.h"
#include "levelLoader.h"

int main() {
    int screenWidth = 200;
    auto fscreenWidth = static_cast<float>(screenWidth);
    int screenHeight = 60;
    auto fscreenHeight = static_cast<float>(screenHeight);

    struct termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char keyPressed;

    displayBuffer buffish(screenWidth,screenHeight);
    physicsEngine world(0.6,&buffish);
    sprite player("player.txt");
    //sprite player(9,7);
    // std::vector<std::string> frame;
    // frame.emplace_back("   ___   ");
    // frame.emplace_back("  /*U*\\  ");
    // frame.emplace_back("  \\ - /  ");
    // frame.emplace_back(" /[---]\\ ");
    // frame.emplace_back("/ [---] \\");
    // frame.emplace_back("  |   |  ");
    // frame.emplace_back("  |   |  ");
    // player.add_frame(frame);
    world.initLevel("level_0.txt");
    buffish.addSprite(entityType::player,player);

    sprite ground("ground.txt");
    buffish.addTileSprite(tileType::ground,ground);


    idNum player_character = world.spawnEntity(entityType::player,20,20,30,50,9,7,100,100);

    int jumpTimer = 0;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));//non windows(therefore better) Sleep(16)
        if (jumpTimer > 0) {
            --jumpTimer;
        };

        buffish.drawScreen();
        world.runPhysics();
        coordPair position = world.getEntityPosition(player_character);
        float X = std::get<0>(position);
        float Y = std::get<1>(position);
        coordPair velocity = world.getEntityVelocity(player_character);
        float vX = std::get<0>(velocity);
        float vY = std::get<1>(velocity);
        coordPair acceleration = world.getEntityAcceleration(player_character);
        float aX = std::get<0>(acceleration);
        float aY = std::get<1>(acceleration);

        if (std::get<1>(position)+7 > 60) {
            world.setEntityPosition(player_character,std::make_tuple(X,60-7));
            coordPair new_velocity = std::make_tuple(vX,0);
            world.setEntityVelocity(player_character,new_velocity);
            coordPair new_acceleration = std::make_tuple(aX,0);
            world.setEntityAccel(player_character,new_acceleration);

        }

        coordPair cameraPosition = buffish.getCameraPosition();
        float camX = std::get<0>(cameraPosition);
        float camY = std::get<1>(cameraPosition);
        float newcamX = camX;
        float newcamY = camY;

        if (X > camX + fscreenWidth - 27) {
            newcamX = X + 27 - fscreenWidth;
        } else if (X < camX + 27) {
            newcamX = X - 27;
        }
        if (Y > camY + fscreenHeight - 27) {
            newcamY = Y + 27 - fscreenHeight;
        } else if (Y < camY + 27) {
            newcamY = Y - 27;
        }

        buffish.moveCamera(newcamX,newcamY);


        buffish.SetCursorPosition(0,61);
        for (int i=0; i<200;i++) {
            std::cout<<" ";
        };
        buffish.SetCursorPosition(0,61);
        std::cout<<" X="<<X;
        buffish.SetCursorPosition(30,61);
        std::cout<<"Y="<<Y;
        buffish.SetCursorPosition(60,61);
        std::cout<<"VX="<<vX;
        buffish.SetCursorPosition(90,61);
        std::cout<<"VY="<<vY;
        buffish.SetCursorPosition(120,61);
        std::cout<<"CX"<<camX;
        buffish.SetCursorPosition(150,61);
        std::cout<<"CY"<<camY;

        if (read(STDIN_FILENO,&keyPressed,1) == 1) {
            if (keyPressed == 'q') {
                break;
            }
            if (keyPressed == 'w'|| keyPressed == 'p') {
                if (aY > 0) {
                    world.setEntityAccel(player_character,std::make_tuple(aX,0));
                }
                if (jumpTimer == 0) {
                    world.accelerateEntity(player_character,std::make_tuple(0,-100));
                    jumpTimer = 100;
                }
            }else if (keyPressed == 'a') {
                if (aX > 0) {
                    world.setEntityAccel(player_character,std::make_tuple(aY,0));
                }
                world.accelerateEntity(player_character,std::make_tuple(-30,0));
            }else if (keyPressed == 's') {
                if (aY < 0) {
                    world.setEntityAccel(player_character,std::make_tuple(aX,0));
                }
                world.accelerateEntity(player_character,std::make_tuple(0,1));
            }else if (keyPressed == 'd') {
                if (aX < 0) {
                    world.setEntityAccel(player_character,std::make_tuple(aY,0));
                }
                world.accelerateEntity(player_character,std::make_tuple(30,0));
            }
        };

    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
