#ifndef WORLD_HPP
#define WORLD_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include "utility.hpp"

class Player;
// Enum class for direction types
enum class Direction {NONE, UP, DOWN, LEFT, RIGHT};

// A world map class
class World {
private:
    std::vector<std::vector<int>> worldVector;
    Direction startDirection;
    Player player;
    float newX;
    float newY;
    float mapWidth;
    float mapHeight;
    
public:
    // Constuctor
    World(const int row, const int column, const Direction direction, const float playerX, const float playerY) {
        // Initialize the 2D vector
        worldVector = std::vector<std::vector<int>>(row, std::vector<int>(column, 0) );
        for (int i = 0; i < column; ++i){
            worldVector[0][i] = 1;
        }
        startDirection = direction;
        player.x = playerX;
        player.y = playerY;
        newX = player.x;
        newY = player.y;
        mapWidth = static_cast<float>(column);
        mapHeight = static_cast<float>(row);
    }
    // Set up player direction function 
    void SetPlayerDirection(const Direction direction) {
        startDirection = direction;
    }
    // Update the world
    void UpdateWorld(int timeElapsed) {
        if (startDirection == Direction::RIGHT) {
            player.xSpeed += 100.0 * static_cast<float>(timeElapsed) / 1000.0;
        }else if (startDirection == Direction::LEFT) {
            player.xSpeed -= 100.0 * static_cast<float>(timeElapsed) / 1000.0;
        }else if (startDirection == Direction::UP) {
            player.ySpeed += 100.0 * static_cast<float>(timeElapsed) / 1000.0;
        }else if (startDirection == Direction::DOWN) {
            player.ySpeed -= 100.0 * static_cast<float>(timeElapsed) / 1000.0;
        }

        float moveLengthX = player.xSpeed * timeElapsed/1000.0;
        newX += moveLengthX;
        if (newX < 0 or newX > mapWidth - 1){
            newX -= moveLengthX;
            player.xSpeed = 0;
        }

        float moveLengthY = player.ySpeed * timeElapsed/1000.0;
        newY += moveLengthY;
        if (newY < 0 or newY > mapHeight - 1 ){
            newY -= moveLengthY;
            player.ySpeed = 0;
        }
        
        auto line = DrawLine(static_cast<int>(player.x), static_cast<int>(player.y), static_cast<int>(newX), static_cast<int>(newY));
        if(line.size() != 1){
            for(auto& entry : line) {
                if(worldVector[entry.second][entry.first] == 1){
                    if (entry.second == static_cast<int>(player.y)){
                        player.xSpeed = 0;
                        startDirection = Direction::NONE;
                        return;
                    }else if(entry.first == static_cast<int>(player.x)){
                        player.ySpeed = 0;
                        startDirection = Direction::NONE;
                        return;
                    }else {
                        player.xSpeed = 0;
                        player.ySpeed = 0;
                        startDirection = Direction::NONE;
                        return;
                    }
                }
            }
        }

        player.x = newX;
        player.y = newY;

    }
    // Return a 2D world map
    const std::vector<std::vector<int>>& GetWorldMap(){
        return worldVector;
    }
    // Return the player's x-position
    float GetPlayerX(){
        return player.x;
    }
    // Return the player's y-position
    float GetPlayerY(){
        return player.y;
    }
};

#endif //WORLD_HPP