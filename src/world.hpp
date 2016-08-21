#ifndef WORLD_HPP
#define WORLD_HPP

#include <Box2D/Box2D.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

// Enum class for direction types
enum class Direction {NONE, UP, DOWN, LEFT, RIGHT};

class MyQueryCallback : public b2QueryCallback {
  public:
      std::vector<b2Body*> foundBodies;
      
      bool ReportFixture(b2Fixture* fixture) {
          foundBodies.push_back( fixture->GetBody() ); 
          return true;//keep going to find all fixtures in the query area
      }
  };

// A world map class
class World {
private:
    std::vector<std::vector<std::vector<int>>> levelVectors;
    std::vector<std::vector<int>> worldVector;
    Direction xDirection;
    Direction yDirection;
    float mapWidth;
    float mapHeight;
    bool playerJump;
    int level;
    
    // Simulated Physics
    b2World* world;
    std::vector<b2Body*> blocks; // Might not need this
    std::vector<b2Body*> players;
    
    /**
     * Returns true if the player is touching the ground.
     */
    bool TouchingGround() {
        int maxShapes = 10;
        auto playerX = GetPlayerX();
        auto playerY = GetPlayerY();
        
        b2AABB aabb;
        aabb.upperBound = b2Vec2(playerX, playerY - 0.51f);
        aabb.lowerBound = b2Vec2(playerX, playerY - 0.52f);
        
        MyQueryCallback queryCallback;
        world->QueryAABB(&queryCallback, aabb);
        
        for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
            for (int j = 0; j < blocks.size(); ++j) {
                if (queryCallback.foundBodies[i] == blocks[j]) {
                    return true;
                }
            }
        }
        
        return false;
    }

    void BuildBox(int boxX, int boxY, int boxWidth, b2World* paraWorld){
        b2BodyDef groundDefinition;
        groundDefinition.position.Set(static_cast<float>(boxX)+static_cast<float>(boxWidth)/2.0f, static_cast<float>(boxY));
        b2Body* groundBody = paraWorld->CreateBody(&groundDefinition);
        // Define the shape 
        b2PolygonShape groundBox;
        groundBox.SetAsBox(static_cast<float>(boxWidth)/2.0f, 0.5f);
        groundBody->CreateFixture(&groundBox, 0.0f);
        blocks.push_back(groundBody);
    }
    // Build the blocks and put the continuous blocks in same row into one box;
    // Create the ground from the provided worldVector map
    void BuildBlock(const int row, const int column){
        for (int y = 0; y < row; ++y) {
            int count = 0;
            bool emptyBlock = false;
            int boxWidth = 0;
            int boxX = 0;
            int boxY = 0;
            for (int x = 0; x < column; ++x) {
                if (worldVector[y][x] == 1 or worldVector[y][x] == 2) {
                    ++count;
                    
                }else {
                    emptyBlock = true;
                    boxWidth = count;
                    count = 0;
                }

                if(count == 1){
                    boxX = x;
                    boxY = y;
                    emptyBlock = false;
                }

                if (emptyBlock == true and boxWidth != 0){
                    BuildBox(boxX, boxY, boxWidth, world);
                }
            }

            if (count > 0){
                boxWidth = count;
                BuildBox(boxX, boxY, boxWidth, world);
            }

        }
    }
    // Create player
    CreatePlayer (const float playerX, const float playerY){
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(playerX, playerY);
        b2Body* body = world->CreateBody(&bodyDef);
        
        b2PolygonShape dynamicBox;
	    dynamicBox.SetAsBox(0.5f, 0.5f);
        b2FixtureDef fixtureDef;
	    fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 2.9f;
        body->CreateFixture(&fixtureDef);
        
        players.push_back(body);
    }

    // worldVector creater
    std::vector<std::vector<int>> CreateWorld(const int startPlayerX, const int startPlayerY, const int width, const int height, const float holeRate, const int averageHoleWidth, const int holeWidthVariance, 
    const float platformRate, const int averagePlatformWidth, const int platformWidthVariance, const int averagePlatformHeight, const int platformHeightVariance) {
        std::vector<std::vector<int>> createdVector;
        createdVector = std::vector<std::vector<int>>(height, std::vector<int>(width, 0) );
        for (int i = 0; i < width; ++i){
            createdVector[0][i] = 1;    
        }
        for (int i = startPlayerX - 2; i < startPlayerX + 3; ++i){
            createdVector[startPlayerY - 2][i] = 2;
        }
        int holeNum = width * holeRate;
        int holeRange = 1 / holeRate; 
        
        for (int n = 0; n < holeNum; ++n){
            int holePosition = rand() % holeRange + (n * holeRange);
            int actualHoleWidth = rand() % (2 * holeWidthVariance + 1) + (averageHoleWidth - holeWidthVariance);
            for (int m = 0; m < actualHoleWidth; ++m){
                createdVector[0][holePosition + m] = 0;
            }      
        }

        int platformNum = width * platformRate;
        int platformRange = 1 / platformRate;

        for (int j = 0; j < platformNum; ++j){
            if (j != 1){
                int platformPosition = rand() % platformRange + (j * platformRange);
                int actualPlatformWidth = rand() % (2 * platformWidthVariance + 1) + (averagePlatformWidth - holeWidthVariance);
                int actualHeight = rand() % (2 * platformHeightVariance + 1) + (averagePlatformHeight - platformHeightVariance);
                for (int m = 0; m < actualPlatformWidth; ++m){
                    createdVector[actualHeight][platformPosition + m] = 2;
                }
            }
        }

        // Go through each row and make sure the length of each sequence of blocks is an even count
        for (int y = 0; y < height; ++y) {
            int blockCount = 0;
            for (int x = 0; x < width; ++x) {
                if (createdVector[y][x] != 0) {
                    ++blockCount;
                } else {
                    // Reached end of blocks, make sure the count is even, else adjust
                    if (blockCount % 2 != 0) {
                        // Odd number of blocks in sequence
                        if (x == 1) {
                            // One block on left side of map, remove it
                            createdVector[y][0] = 0;
                        } else if (x == width - 1) {
                            // At the end of the map, so just remove block
                            createdVector[y][width-1] = 0;
                        } else {
                            // Just shorten the length of the ground by 1 block
                            createdVector[y][x-1] = 0;
                        }
                    }
                    blockCount = 0;
                }
            }
        }

        return createdVector;
    }

public:
    // Constuctor
    World(const int row, const int column, const Direction direction, const float playerX, const float playerY, const int maxLevel) {
        // Set time seed for generate random numbers
        srand(time(NULL));
        level = 0; // game level
        // Initialize the 2D vector
        // Parameter: startPlayerX, startPlayerY, width, height, 
        // holeRate, averageHoleWidth, holeWidthVariance 
        // platformRate, averagePlatformWidth, platformWidthVariance, averagePlatformHeight, platformHeightVariance
        float holeRate = 0.1;
        int averageHoleWidth = 3;
        int holeWidthVariance = 1;
        float platformRate = 0.1;
        int averagePlatformWidth = 5;
        int platformWidthVariance = 3;
        int averagePlatformHeight = 7;
        int platformHeightVariance = 4;
        for (int i = 0; i < maxLevel; ++i){
            levelVectors.push_back(CreateWorld(static_cast<int>(playerX), static_cast<int>(playerY), column, row, holeRate, averageHoleWidth, holeWidthVariance, 
            platformRate, averagePlatformWidth, platformWidthVariance, averagePlatformHeight, platformHeightVariance));
        }
        worldVector = levelVectors[level];

        mapWidth = static_cast<float>(column);
        mapHeight = static_cast<float>(row);
        playerJump = false;
        
        // Create physics world
        b2Vec2 gravity(0.0f, -50.0f);
        world = new b2World(gravity);
        
        // Build the blocks and put the continuous blocks in same row into one box;
        // Create the ground from the provided worldVector map
        BuildBlock (row, column);

        // Create player
        CreatePlayer (playerX, playerY);
    }
    
    // Set up player direction function 
    void SetPlayerXDirection(const Direction direction) {
        xDirection = direction;
    }
    
    void SetPlayerYDirection(const Direction direction) {
        // TODO: This might be useful if he had a jetpack haha
        //yDirection = direction;
    }
    
    void JumpPlayer() {
        playerJump = true;
    }
    
    // Update the world
    float UpdateWorld(int timeElapsed) {
        float xForce = 0.0f;
        float yForce = 0.0f;
        if (xDirection == Direction::RIGHT) {
            xForce += 10.0f;
        } else if (xDirection == Direction::LEFT) {
            xForce -= 10.0f;
        }
        
        if (yDirection == Direction::UP) {
            yForce += 10.0f;
        } else if (yDirection == Direction::DOWN) {
            yForce -= 10.0f;
        }
        
        auto force = b2Vec2(xForce*10.0f, yForce*10.0f);
        auto playerCenterOfGravity = players[0]->GetWorldCenter();
        players[0]->ApplyForce(force, playerCenterOfGravity, true);
        
        if (playerJump) {
            // Only jump if on the ground
            if (TouchingGround()) {
                players[0]->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), playerCenterOfGravity, true);
            }
            playerJump = false;
        }
        
        float oldX = GetPlayerX();
        world->Step(static_cast<float>(timeElapsed)/1000.0f, 6, 2);
        float deltaX = GetPlayerX() - oldX;
        return deltaX;
        
    }
    // Return a 2D world map
    const std::vector<std::vector<int>>& GetWorldMap(){
        return worldVector;
    }
    // Return the player's x-position
    float GetPlayerX(){
        return players[0]->GetPosition().x;
    }
    // Return the player's y-position
    float GetPlayerY(){
        return players[0]->GetPosition().y;
    }
    // Return X direction
    Direction GetDirectionX(){
        return xDirection;
    }
    // Update world map level
    bool UpdateLevel(const int row, const int column, const float playerX, const float playerY, const int maxLevel){
        ++level;
        if (level < maxLevel){
            worldVector = levelVectors[level];  
            delete world;
            playerJump = false;
            players.clear();
            
            // Create physics world
            b2Vec2 gravity(0.0f, -50.0f);
            world = new b2World(gravity);
            
            // Build the blocks and put the continuous blocks in same row into one box;
            // Create the ground from the provided worldVector map
            BuildBlock (row, column);

            // Create player
            CreatePlayer (playerX, playerY);
            return true;
        }  
        return false;
    }

    void RestartLevel (const float playerX, const float playerY){
        players.clear();
        CreatePlayer(playerX, playerY);
    }

    ~World() {
        delete world;
    }
};

#endif //WORLD_HPP