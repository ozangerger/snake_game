#include <SFML/Graphics.hpp>
#include <deque>
#include <random>
#include <chrono>


constexpr static float segmentRadius = 0.5F;
constexpr static float moveStep = segmentRadius * 2.0F;
constexpr static unsigned int gameSize = 25;
constexpr static unsigned int xSize = gameSize * static_cast<unsigned int>(moveStep);
constexpr static unsigned int ySize = gameSize * static_cast<unsigned int>(moveStep);
static std::mt19937 gen{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_int_distribution<> xDistrib{ 1U, xSize };
std::uniform_int_distribution<> yDistrib{ 1U, ySize };


enum direction {
    none,
    left,
    right,
    up,
    down
};


class Food : public sf::Drawable {
public:
    Food() = delete;

    Food(const sf::Vector2f& headPos) {
        food.setPosition(sf::Vector2f(static_cast<float>(xDistrib(gen)), static_cast<float>(yDistrib(gen))));

        UpdateFoodLocation(headPos);
    }

    void UpdateFoodLocation(const sf::Vector2f& headPos) {
        while (headPos == food.getPosition())
        {
            food.setPosition(sf::Vector2f(static_cast<float>(xDistrib(gen)), static_cast<float>(yDistrib(gen))));
        }
    }

    sf::Vector2f getPosition() const {
        return food.getPosition();
    }


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // our particles don't use a texture
        states.texture = nullptr;
        target.draw(this->food, states);

    }

    sf::CircleShape food{ segmentRadius };
};


class Snake : public sf::Drawable
{
    public:
        Snake(unsigned int numberOfSections)
        {
            for (std::size_t ind = 0U; ind <= numberOfSections; ++ind) {
                sf::CircleShape circle(segmentRadius);
                circle.setFillColor(sf::Color::White);
                circle.setPosition(sf::Vector2{ std::roundf(static_cast<int>(xSize) / 2.0F) + moveStep * ind, 
                                                std::roundf(static_cast<float>(ySize) / 2.0F) });
                m_vertices.emplace(m_vertices.end(), circle);
            }
            m_prev_direction = direction::right;
        }

        sf::Vector2f getHeadPosition() {
            return m_vertices.back().getPosition();
        }
    
        void move(direction direction)
        {
            const sf::Vector2f newDirVec = getDirectionVector(direction);
            m_vertices.pop_front();
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
            m_prev_direction = direction;
        }

        bool gameOver() const {
            return isGameOver; 
        }

        sf::Vector2f getDirectionVector(direction dir)
        {
            sf::Vector2f direction{};
            switch (dir) {
            case direction::left:
                direction = { -moveStep, 0.0F };
                break;
            case direction::right:
                direction = {  moveStep, 0.0F };
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

       direction calcRequestedDirection() const
        {
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

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
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

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({ xSize, ySize }), "Snake");

    // create a clock to track the elapsed time
    sf::Clock clock;
    const sf::Time updateRate = sf::milliseconds(100);
    sf::Time timeSinceLastMove = sf::Time::Zero;

    Snake snake(10U);
    Food snakeFood{snake.getHeadPosition()};

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
            snake.move(snakeDir);
            snakeFood.UpdateFoodLocation(snake.getHeadPosition());
            timeSinceLastMove = sf::Time::Zero;
        }

        if (snake.gameOver()) {
            sf::Font font("arial.ttf");

            sf::Text gameOverText(font, "GAME OVER", 8);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(sf::Vector2f(static_cast<float>(xSize) / 2.0F, static_cast<float>(ySize) / 2.0F));

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