#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "world.hpp"
#include "string"
#include <iostream>

const int MAX_LEVEL = 2;
const int ROW = 20;
const int COLUMN = 400;
const Direction START_DIRECTION = Direction::NONE;
const float START_POSITION_X = 18.0f;
const float START_POSITION_Y = 10.0f;
const float LEFTBOUNDARY = 18.0f; // 45% boundary on both sides 
const float RIGHTBOUNDARY = 22.0f; // Right boundary would become 55% of 40(the window width)

void DrawTile(sf::Sprite& sprite, sf::RenderWindow& window, float x, float y) {
    sprite.setPosition(x * 32.0f, (23.0f-y) * 32.0f);
    window.draw(sprite);
}

// 1 unit is 16 pixel; 80x45
int main()
{
    int level = 1;
    int timeElapsed = 0;
    bool win = false;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1280, 768), "FangRunner Window");
    window.setFramerateLimit(144);
    // Create a rectangle window to display
    sf::RectangleShape rectangle(sf::Vector2f(1280, 768));
    rectangle.setFillColor(sf::Color::Transparent);
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("../data/nice_music.ogg"))
        return EXIT_FAILURE;
    // Play the music
    music.play();
    // Create a pig player
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../data/pig.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite pigPlayerRight(playerTexture);
    pigPlayerRight.setScale(0.057658f, 0.085333f);
    pigPlayerRight.setOrigin(16.0f, 16.0f);
    sf::Sprite pigPlayerLeft(playerTexture);
    pigPlayerLeft.setScale(0.057658f, 0.085333f);
    pigPlayerRight.setOrigin(160.0f*3.2f, 0.0f);
    pigPlayerLeft.scale(-1.0f,1.0f);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("../data/arial.ttf")){
        return EXIT_FAILURE;
    }  
    sf::Text text("Starting World " + std::to_string(level), font, 50);
    text.setColor(sf::Color::Magenta);
    text.setPosition(500.0f, 128.0f);
    sf::Text wonText("You won!", font, 70);
    wonText.setColor(sf::Color::Magenta);
    wonText.setPosition(500.0f, 200.0f);
    // Create a background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../data/background.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite background(backgroundTexture);
    background.setScale(0.38f, 0.32f);

    // Create the floor
    sf::Texture floorMiddleTexture;
    if (!floorMiddleTexture.loadFromFile("../data/middle.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite floorMiddle(floorMiddleTexture);
    floorMiddle.setScale(0.125f*2.0f, 0.2162f);
    // Create the floor left corner
    sf::Texture floorLeftTexture;
    if (!floorLeftTexture.loadFromFile("../data/floor_left_corner.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite floorLeft(floorLeftTexture);
    floorLeft.setScale(0.1839f, 0.2162f);
    // Create the floor right corner
    sf::Texture floorRightTexture;
    if (!floorRightTexture.loadFromFile("../data/floor_right_corner.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite floorRight(floorRightTexture);
    floorRight.setScale(0.1839f, 0.2162f);

    // Create the platform
    sf::Texture platformMiddleTexture;
    if (!platformMiddleTexture.loadFromFile("../data/middle_edge.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite platformMiddle(platformMiddleTexture);
    platformMiddle.setScale(0.125f*2.0f, 0.2162f);
    // Create the floor left corner
    sf::Texture platformLeftTexture;
    if (!platformLeftTexture.loadFromFile("../data/left_edge.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite platformLeft(platformLeftTexture);
    platformLeft.setScale(0.1839f, 0.2162f);
    // Create the floor right corner
    sf::Texture platformRightTexture;
    if (!platformRightTexture.loadFromFile("../data/right_edge.png")){
        return EXIT_FAILURE;
    } 
    sf::Sprite platformRight(platformRightTexture);
    platformRight.setScale(0.1839f, 0.2162f);
    // Declare the world
    World world(ROW, COLUMN, START_DIRECTION, START_POSITION_X, START_POSITION_Y, MAX_LEVEL);
    sf::Clock clock;
    sf::Clock time;
    // Record last loop's x position
    float lastX = START_POSITION_X;
    bool turnBack = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
             switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Z){
                        world.JumpPlayer();
                    }
                // we don't process other types of events
                default:
                    break;  
            }    
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            world.SetPlayerYDirection(Direction::UP);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            world.SetPlayerYDirection(Direction::DOWN);
        } else {
            world.SetPlayerYDirection(Direction::NONE);
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            world.SetPlayerXDirection(Direction::LEFT);
            turnBack = true;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            world.SetPlayerXDirection(Direction::RIGHT);
            turnBack = false;
        } else {
            world.SetPlayerXDirection(Direction::NONE);
        }

        // Update the world for each loop
        float deltaX = world.UpdateWorld(clock.restart().asMilliseconds());
        
        timeElapsed += time.restart().asMilliseconds();
        // Update world level
        if (world.GetPlayerX() >= COLUMN){
            auto levelUp = world.UpdateLevel(ROW, COLUMN, START_POSITION_X, START_POSITION_Y, MAX_LEVEL);
            if (levelUp){
                ++level;
                text.setString("Starting World " + std::to_string(level));
                timeElapsed = 0;
                world.SetPlayerYDirection(START_DIRECTION);
                deltaX = world.UpdateWorld(clock.restart().asMilliseconds());
                lastX = START_POSITION_X;
            } else {
                win = true;
            }
        }

        if (world.GetPlayerY() < 0 and win == false){
            world.RestartLevel(START_POSITION_X, START_POSITION_Y);
            world.SetPlayerXDirection(START_DIRECTION);
            deltaX = world.UpdateWorld(clock.restart().asMilliseconds());
            timeElapsed = 0;
            lastX = START_POSITION_X;
        }
        auto worldMap = world.GetWorldMap();
        
        float x = lastX + deltaX;
        float originalX = world.GetPlayerX();
        float y = world.GetPlayerY();

        
        if (x < LEFTBOUNDARY) {
            x += LEFTBOUNDARY - x;
        }else if (x > RIGHTBOUNDARY) {
            x -= x - RIGHTBOUNDARY;
        }else if (x < 0){
            x = 0;
        }
        lastX = x;
        
        
        // Clear screen
        window.clear(sf::Color::White);
        // Draw background
        window.draw(background);
        // Draw the rectangle
        window.draw(rectangle);
        // Draw the map
        for (int i = 0; i < ROW; ++i){
            bool startingCornerIsOddX = false;
            for (int n = 0; n < COLUMN; ++n){
                float squareGroundX = n;
                squareGroundX += x - originalX;
                if (squareGroundX > -1.0f and squareGroundX < 40.0f){
                    if (worldMap[i][n] != 0) {
                        // Ground present
                        if (n == 0 or (n > 0 and worldMap[i][n-1] == 0)) {
                            // Left corner
                            startingCornerIsOddX = n % 2 == 1;
                            if (i == 0) {
                                DrawTile(floorLeft, window, squareGroundX, i);
                            } else {
                                DrawTile(platformLeft, window, squareGroundX, i);
                            }
                        } else if (n == COLUMN-1 or (n < COLUMN-1 and worldMap[i][n+1] == 0)) {
                            // right corner
                            if (i == 0) {
                                DrawTile(floorRight, window, squareGroundX, i);
                            } else {
                                DrawTile(platformRight, window, squareGroundX, i);
                            }
                        } else if ((startingCornerIsOddX and n % 2 == 0) or (!startingCornerIsOddX and n % 2 != 0)) {
                            // Middle
                            if (i == 0) {
                                DrawTile(floorMiddle, window, squareGroundX, i);
                            } else {
                                DrawTile(platformMiddle, window, squareGroundX, i);
                            }
                        }
                    }
                }
            }     
        }
        // Checking if there is turning
        if (turnBack == true){
            pigPlayerLeft.setPosition(x * 32.0f + 16.0f, (23-y) * 32.0f);
            window.draw(pigPlayerLeft);
        } else {
            pigPlayerRight.setPosition(x * 32.0f + 16.0f, (23-y) * 32.0f);
            window.draw(pigPlayerRight);
        }
        // Draw the string
        if (timeElapsed <= 3000.0f){
            window.draw(text);
        } 
        if (win){
            window.draw(wonText);
        }
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}