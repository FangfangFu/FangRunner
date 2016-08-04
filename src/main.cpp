#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "world.hpp"
#include "utility.hpp"

const int ROW = 20;
const int COLUMN = 40;
const Direction START_DIRECTION = Direction::NONE;
const float START_POSITION_X = 5.0f;
const float START_POSITION_Y = 10.0f;
// 1 unit is 16 pixel; 80x45
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1280, 768), "FangRunner Window");
    window.setFramerateLimit(144);
    // Create a rectangle window to display
    sf::RectangleShape rectangle(sf::Vector2f(1280, 768));
    rectangle.setFillColor(sf::Color::Transparent);
    // Create a green square element of a snake
    sf::RectangleShape squareGround(sf::Vector2f(32.0f, 32.0f));
    squareGround.setFillColor(sf::Color::Green);
    // Create a red square player
    sf::RectangleShape squarePlayer(sf::Vector2f(32.0f, 32.0f));
    squarePlayer.setFillColor(sf::Color::Red);
    // Declare the world
    World world(ROW, COLUMN, START_DIRECTION, START_POSITION_X, START_POSITION_Y);
    sf::Clock clock;

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
                // we don't process other types of events
                default:
                    break;  
            }    
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            world.SetPlayerDirection(Direction::UP);
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            world.SetPlayerDirection(Direction::DOWN);
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            world.SetPlayerDirection(Direction::LEFT);
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            world.SetPlayerDirection(Direction::RIGHT);
        }else {
            world.SetPlayerDirection(Direction::NONE);
        }

        // Update the world for each loop
        world.UpdateWorld(clock.restart().asMilliseconds());
        auto worldMap = world.GetWorldMap();
        float x = world.GetPlayerX();
        float y = world.GetPlayerY();
        squarePlayer.setPosition(x * 32.0f, (23-y) * 32.0f);

        // Clear screen
        window.clear(sf::Color::White);
        // Draw the rectangle
        window.draw(rectangle);
        // Draw the map
        for (int i = 0; i < 20; ++i){
            for(int n = 0; n < 40; ++n){
                if(worldMap[i][n] == 1){
                    squareGround.setPosition(n * 32.0f, 23 * 32.0f);
                    window.draw(squareGround);
                }
            }
        }
        window.draw(squarePlayer);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}