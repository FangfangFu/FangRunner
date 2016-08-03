#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cstdlib>
#include <iostream>

// A player class
class Player {
public:
    float x;
    float y;
    float xSpeed;
    float ySpeed;

    Player (){
        x = 5.0;
        y = 10.0;
        xSpeed = 0;
        ySpeed = 0;
    }

};
#endif //PLAYER_HPP