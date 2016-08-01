// This program is about FangRunner getting points
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <deque>
#include <string>

int main(int nNumberofArgs, char* args[])
{
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1280, 960), "FangRunner window");
    // Create a rectangle window to display
    sf::RectangleShape rectangle(sf::Vector2f(1280, 960));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineThickness(-64);
    rectangle.setOutlineColor(sf::Color::Blue);
   
    //Window loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;

                // key pressed
                case sf::Event::KeyPressed:
                    break;

                // we don't process other types of events
                default:
                    break;
            }
        }
        // Clear screen
        window.clear(sf::Color::White);
        // Draw the rectangle
        window.draw(rectangle);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}