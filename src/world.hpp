#ifndef WORLD_HPP
#define WORLD_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
class Player;
// Enum class for direction types
enum class Direction {NONE, UP, DOWN, LEFT, RIGHT};

// A world map class
class World {
private:
    std::vector<std::vector<int>> worldVector;
    Direction startDirection;
    Player player;
    
public:
    // Constuctor
    World(const int row, const int column, const Direction direction) {
        // Initialize the 2D vector
        worldVector = std::vector<std::vector<int>>(row, std::vector<int>(column, 0) );
        for (int i = 0; i < column; ++i){
            worldVector[0][i] = 1;
        }
        startDirection = direction;  
    }
    // Set up player direction function 
    SetPlayerDirection(const Direction direction) {
        startDirection = direction;
    }
    // Update the world
    UpdateWorld(int timeElapsed) {
        if (startDirection == Direction::RIGHT) {
            player.xSpeed += 200.0 * static_cast<float>(timeElapsed) / 1000.0;
        }else if (startDirection == Direction::LEFT) {
            player.xSpeed -= 200.0 * static_cast<float>(timeElapsed) / 1000.0;
        }else if (startDirection == Direction::UP) {
            player.ySpeed -= 200.0 * static_cast<float>(timeElapsed) / 1000.0;
        }else if (startDirection == Direction::DOWN) {
            player.ySpeed += 200.0 * static_cast<float>(timeElapsed) / 1000.0;
        } 
        player.x += player.xSpeed * timeElapsed / 1000.0;
        player.y += player.ySpeed * timeElapsed / 1000.0;
    }
    // Return a 2D world map
    const std::vector<std::vector<int>> GetWorldMap(){
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