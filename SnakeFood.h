#ifndef SNAKEFOOD_H
#define SNAKEFOOD_H
#include <SFML/Graphics.hpp>
#include <deque>
#include "base.h"

class Snake;

class SnakeFood : public sf::Drawable {
public:
    SnakeFood() = delete;

    SnakeFood(const Snake& snake) {
        food.setPosition(sf::Vector2f(static_cast<float>(xDistrib(gen)) * moveStep, static_cast<float>(yDistrib(gen)) * moveStep));

        food.setFillColor(sf::Color::Green);


        UpdateFoodLocation(snake);
    }

    void UpdateFoodLocation(const Snake& snake) {
        while ( SearchBody(snake, this->food.getPosition()) )
        {
            food.setPosition(sf::Vector2f(static_cast<float>(xDistrib(gen)) * moveStep, static_cast<float>(yDistrib(gen)) * moveStep));
        }
    }

    bool    SearchBody(const Snake&         snake,
                       const sf::Vector2f&  foodPosition) const
    {
        for (const sf::CircleShape& segment : snake.GetBody())
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