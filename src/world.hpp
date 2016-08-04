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
    SetPlayerDirection(const Direction direction) {
        startDirection = direction;
    }
    // Update the world
    UpdateWorld(int timeElapsed) {
        if (startDirection == Direction::RIGHT) {
            player.xSpeed += 200.0 * static_cast<float>(timeElapsed) / 1000.0;
            /*newX += player.xSpeed * timeElapsed / 1000.0;
            if (newX > mapWidth) {
                player.xSpeed = 0;
            }*/
        }else if (startDirection == Direction::LEFT) {
            player.xSpeed -= 200.0 * static_cast<float>(timeElapsed) / 1000.0;
            /*newX += player.xSpeed * timeElapsed / 1000.0;
            if (newX < 0) {
                player.xSpeed = 0;
            }*/
        }else if (startDirection == Direction::UP) {
            player.ySpeed += 200.0 * static_cast<float>(timeElapsed) / 1000.0;
            /*newY += player.ySpeed * timeElapsed / 1000.0;
            if (newY > mapHeight) {
                player.ySpeed = 0;
            }*/
        }else if (startDirection == Direction::DOWN) {
            player.ySpeed -= 200.0 * static_cast<float>(timeElapsed) / 1000.0;
            /*newY += player.ySpeed * timeElapsed / 1000.0;
            if (newY < 0) {
                player.ySpeed = 0;
            }*/
        } 
        
        newX += player.xSpeed * timeElapsed / 1000.0;
        if (newX < 0 or newX > mapWidth - 1){
            player.xSpeed = 0;
        }else {
            player.x = newX;
        }
        newY += player.ySpeed * timeElapsed / 1000.0;
        if (newY < 0 or newY > mapHeight - 1 ){
            player.ySpeed = 0;
        }else {
            player.y = newY;
        }
       /* auto lineVectorPair = Line(player.x, player.y, newX, newY);
        int lineX = 0;
        int lineY = 0;
        for (int i = 0; i < lineVectorPair.size(); ++i){
            lineX = lineVectorPair[i].first;
            lineY = lineVectorPair[i].second;
            if (worldVector[lineY][lineX] == 0){
                player.x = newX;
                player.y = newY;
            }else{
                if (worldVector[player.y][player.x + 1] == 1 or worldVector[player.y][player.x - 1] == 1 ){
                    player.xSpeed = 0;
                }
                if (worldVector[player.y + 1][player.x] == 1 or worldVector[player.y - 1][player.x] == 1){
                    player.ySpeed = 0;
                }
            }
        }*/
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