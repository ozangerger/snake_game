#ifndef SNAKE_H
#define SNAKE_H
#include <SFML/Graphics.hpp>
#include <deque>
#include <cmath>
#include "SnakeFood.h"


class Snake : public sf::Drawable
{
public:
    Snake(unsigned int numberOfSections)
    {
        for (std::size_t ind = 0U; ind <= numberOfSections; ++ind) {
            sf::CircleShape circle(segmentRadius);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(sf::Vector2{ std::roundf(static_cast<float>(gameSize) / 2.0F) * moveStep + moveStep * ind,
                                            std::roundf(static_cast<float>(gameSize) / 2.0F)* moveStep });
            m_vertices.emplace(m_vertices.end(), circle);
        }
        m_prev_direction = direction::right;
    }

    sf::Vector2f getHeadPosition() {
        return m_vertices.back().getPosition();
    }

    void move(direction direction,
              SnakeFood& snakeFood) {
        const sf::Vector2f newDirVec = getDirectionVector(direction);

        sf::CircleShape circle(m_vertices.back());
        circle.move(newDirVec);
        // check collision
        for (auto& segment : m_vertices) {
            if (segment.getPosition() == circle.getPosition())
            {
                // game over
                isGameOver = true;
            }
        }

        m_vertices.emplace(m_vertices.end(), circle);

        const bool isFoodConsumed = snakeFood.getPosition() == this->getHeadPosition();

        if (!isFoodConsumed)
        {
            m_vertices.pop_front();
        }
        else {
            snakeFood.UpdateFoodLocation(*this);
        }

        m_prev_direction = direction;
    }

    bool gameOver() const {
        return isGameOver;
    }

    sf::Vector2f getDirectionVector(direction dir) {
        sf::Vector2f direction{};
        switch (dir) {
        case direction::left:
            direction = { -moveStep, 0.0F };
            break;
        case direction::right:
            direction = { moveStep, 0.0F };
            break;
        case direction::up:
            direction = { 0.0F, -moveStep };
            break;
        case direction::down:
            direction = { 0.0F, moveStep };
            break;
        default:
            // should never happen
            direction = {};
            break;
        }

        return direction;
    }

    direction calcRequestedDirection() const {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) and !(m_prev_direction == direction::right)) {
            return direction::left;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) and !(m_prev_direction == direction::left)) {
            return direction::right;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) and !(m_prev_direction == direction::down)) {
            return direction::up;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) and !(m_prev_direction == direction::up)) {
            return direction::down;
        }
        else {
            return m_prev_direction;
        }
    }

    std::deque<sf::CircleShape>    GetBody() const {
        return m_vertices;
    }

private:
    void draw(sf::RenderTarget& target, 
              sf::RenderStates states) const override
    {
        // our particles don't use a texture
        states.texture = nullptr;

        for (const auto& segment : m_vertices)
        {
            target.draw(segment, states);

        }
    }

    std::deque<sf::CircleShape>       m_vertices{};
    direction                         m_prev_direction{};
    bool                              isGameOver{};

};

#endif

