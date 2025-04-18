#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "SnakeFood.h"


int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{ static_cast<unsigned int>(gameSize * moveStep), 
                                                        static_cast<unsigned int>(gameSize * moveStep) }, 
                                                        32U), 
                                                        "Snake", 
                                                        sf::Style::Resize);

    // create a clock to track the elapsed time
    sf::Clock clock;
    const sf::Time updateRate = sf::milliseconds(100);
    sf::Time timeSinceLastMove = sf::Time::Zero;

    Snake snake(10U);
    SnakeFood snakeFood(snake.GetBody());

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // update it
        sf::Time elapsed = clock.restart();
        timeSinceLastMove += elapsed;

        const direction snakeDir = snake.calcRequestedDirection();

        if (timeSinceLastMove >= updateRate)
        {
            snake.move(snakeDir, snakeFood);
            timeSinceLastMove = sf::Time::Zero;
        }

        if (snake.gameOver()) {
            sf::Font font("arial.ttf");

            sf::Text gameOverText(font, "GAME OVER", 8);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(sf::Vector2f(static_cast<float>(gameSize) * moveStep / 2.0F, static_cast<float>(gameSize) * moveStep / 2.0F));

            window.clear();
            window.draw(snake);
            window.draw(snakeFood);
            window.draw(gameOverText);
            window.display();

            sf::sleep(sf::seconds(10));
            window.close();
        }

        // draw it
        window.clear();
        window.draw(snake);
        window.draw(snakeFood);
        window.display();
    }
}