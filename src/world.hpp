#ifndef WORLD_HPP
#define WORLD_HPP

#include <Box2D/Box2D.h>
#include <cstdlib>
#include <iostream>
#include <vector>

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
    std::vector<std::vector<int>> worldVector;
    Direction xDirection;
    Direction yDirection;
    float mapWidth;
    float mapHeight;
    bool playerJump;
    
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

    /*void BuildBox(){
        

    }*/
    
public:
    // Constuctor
    World(const int row, const int column, const Direction direction, const float playerX, const float playerY) {
        // Initialize the 2D vector
        worldVector = std::vector<std::vector<int>>(row, std::vector<int>(column, 0) );
        for (int i = 0; i < column; ++i){
            if (i != 7 and i != 8)
                worldVector[0][i] = 1;
        }

        mapWidth = static_cast<float>(column);
        mapHeight = static_cast<float>(row);
        playerJump = false;
        
        // Create physics world
        b2Vec2 gravity(0.0f, -50.0f);
        world = new b2World(gravity);
        
        // Create the ground from the provided worldVector map
        /*for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                if (worldVector[y][x] == 1) {
                    // TODO: Instead of doing block by block, create one long block for each continous
                    //       span of blocks. This will avoid the "bumpy" glitch when running
                    b2BodyDef groundDefinition;
                    groundDefinition.position.Set(static_cast<float>(x), static_cast<float>(y));
                    b2Body* groundBody = world->CreateBody(&groundDefinition);
                    
                    // Define the shape (1m long, 1m tall for each block)
                    b2PolygonShape groundBox;
                    groundBox.SetAsBox(0.5f, 0.5f);
                    groundBody->CreateFixture(&groundBox, 0.0f);
                    blocks.push_back(groundBody);
                }
            }
        }*/

        for (int y = 0; y < row; ++y) {
            int count = 0;
            bool emptyBlock = false;
            int boxWidth = 0;
            int boxX = 0;
            int boxY = 0;

            for (int x = 0; x < column; ++x) {
                if (worldVector[y][x] == 1) {
                    //  Instead of doing block by block, create one long block for each continous
                    //       span of blocks. This will avoid the "bumpy" glitch when running
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
                        b2BodyDef groundDefinition;
                        std::cout << "boxX1:" << boxX << std::endl;
                        std::cout << "boxY1:" << boxY << std::endl;
                        groundDefinition.position.Set(static_cast<float>(boxX), static_cast<float>(boxY));
                        b2Body* groundBody = world->CreateBody(&groundDefinition);
                        // Define the shape (1m long, 1m tall for each block)
                        
                        b2PolygonShape groundBox;
                        std::cout << "boxWidth1:" << boxWidth << std::endl;
                        groundBox.SetAsBox(static_cast<float>(boxWidth)/2.0, 0.5f);
                        groundBody->CreateFixture(&groundBox, 0.0f);
                        blocks.push_back(groundBody);
                }
            }

            if (count > 0){
                boxWidth = count;
                b2BodyDef groundDefinition;
                std::cout << "boxX2:" << boxX << std::endl;
                std::cout << "boxY2:" << boxY << std::endl;
                groundDefinition.position.Set(static_cast<float>(boxX), static_cast<float>(boxY));
                b2Body* groundBody = world->CreateBody(&groundDefinition);
                // Define the shape (1m long, 1m tall for each block)
                b2PolygonShape groundBox;
                std::cout << "boxWidth2:" << boxWidth << std::endl;
                groundBox.SetAsBox(static_cast<float>(boxWidth)/2.0, 0.5f);
                groundBody->CreateFixture(&groundBox, 0.0f);
                blocks.push_back(groundBody);
            }

        }

        
        // Create player
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(5.0f, 5.0f);
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
    void UpdateWorld(int timeElapsed) {
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
        
        world->Step(static_cast<float>(timeElapsed)/1000.0f, 6, 2);
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
    
    ~World() {
        for(auto block : blocks) {
            
        }
        delete world;
    }
};

#endif //WORLD_HPP