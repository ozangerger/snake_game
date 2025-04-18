#ifndef SNAKEFOOD_H
#define SNAKEFOOD_H
#include <SFML/Graphics.hpp>
#include <deque>
#include <random>
#include <chrono>

static std::mt19937 gen{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_int_distribution<> xDistrib{ 0U, gameSize - 1U };
std::uniform_int_distribution<> yDistrib{ 0U, gameSize - 1U };

class SnakeFood : public sf::Drawable {
public:
    SnakeFood() = delete;

    SnakeFood(const std::deque<sf::CircleShape>& body) {
        food.setPosition(sf::Vector2f(static_cast<float>(xDistrib(gen)) * moveStep, static_cast<float>(yDistrib(gen)) * moveStep));

        food.setFillColor(sf::Color::Green);


        UpdateFoodLocation(body);
    }

    void UpdateFoodLocation(const std::deque<sf::CircleShape>& body) {
        while ( SearchBody(body, this->food.getPosition()) )
        {
            food.setPosition(sf::Vector2f(static_cast<float>(xDistrib(gen)) * moveStep, static_cast<float>(yDistrib(gen)) * moveStep));
        }
    }

    bool    SearchBody(const std::deque<sf::CircleShape>& body,
                       const sf::Vector2f&  foodPosition) const
    {
        for (const sf::CircleShape& segment : body)
        {
            if (segment.getPosition() == foodPosition)
            {
                return true;
            }
        }
        
        return false;
    }

    sf::Vector2f getPosition() const {
        return food.getPosition();
    }


private:
    void draw(sf::RenderTarget& target, 
              sf::RenderStates  states) const override
    {
        // our particles don't use a texture
        states.texture = nullptr;
        target.draw(this->food, states);

    }

    sf::CircleShape food{ segmentRadius };
};
#endif