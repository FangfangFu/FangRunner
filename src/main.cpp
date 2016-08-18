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
    // Create a green square to build ground
    sf::RectangleShape squareGround(sf::Vector2f(32.0f, 32.0f));
    squareGround.setFillColor(sf::Color::Green);
    // Create a red square player
    sf::RectangleShape squarePlayer(sf::Vector2f(32.0f, 32.0f));
    squarePlayer.setFillColor(sf::Color::Red);
    // // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("../data/arial.ttf"))
        return EXIT_FAILURE;
    sf::Text text("Starting World " + std::to_string(level), font, 50);
    text.setColor(sf::Color::Magenta);
    text.setPosition(500.0f, 128.0f);
    sf::Text wonText("You won!", font, 70);
    wonText.setColor(sf::Color::Magenta);
    wonText.setPosition(500.0f, 200.0f);

    // Declare the world
    World world(ROW, COLUMN, START_DIRECTION, START_POSITION_X, START_POSITION_Y, MAX_LEVEL);
    sf::Clock clock;
    sf::Clock time;
    // Record last loop's x position
    float lastX = START_POSITION_X;
    
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
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            world.SetPlayerXDirection(Direction::RIGHT);
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

        if (world.GetPlayerY() < 0){
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
        squarePlayer.setPosition(x * 32.0f - 16.0f, (23-y) * 32.0f);
        //float rangeLeft = x - 20.0f;
        // Clear screen
        window.clear(sf::Color::White);
        // Draw the rectangle
        window.draw(rectangle);
        // Draw the map
        for (int i = 0; i < 20; ++i){
            for (int n = 0; n < 400; ++n){
                float squareGroundX = n;
                if(worldMap[i][n] == 1){
                    squareGroundX += x - originalX;
                    if (squareGroundX > -1.0f and squareGroundX < 40.0f){
                        squareGround.setPosition(squareGroundX * 32.0f, (23.0f-i) * 32.0f);
                        window.draw(squareGround);
                    }
                }
            }     
        }
        window.draw(squarePlayer);
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