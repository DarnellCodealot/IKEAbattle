#include "libs.hpp"
Idk::Idk(sf::Vector2f position, float moveDistance)
    : Entity(), moveSpeed(100.0f), moveDistance(moveDistance), initialX(position.x), gravity(980.0f), movingRight(true)
{
    setPosition(position);
    loadSprite();
}

void Idk::loadSprite()
{
    loadSpritesheet("./imgs/arrow.png", 32, 32);
       addAnimation("default", 0, 1);
       setAnimation("default");
       pause();
}

void Idk::update(float deltaTime, Map& map, const sf::Vector2u& screenres)
{
    velocity.y += gravity * deltaTime;
    if (movingRight) {
        velocity.x = moveSpeed;
        if (position.x >= initialX + moveDistance) {
            movingRight = false;
        }
    } else {
        velocity.x = -moveSpeed;
        if (position.x <= initialX) {
            movingRight = true;
        }
    }
    // Update position
    position += velocity * deltaTime;
    getSprite().setPosition(position);
    manageCollisions(map.getObjectBounds());
    
    // Update animation
    Animation::update(deltaTime);
}

void Idk::draw(sf::RenderWindow& window)
{
    window.draw(getSprite());
}

void Idk::manageCollisions(const std::vector<sf::FloatRect>& objectBounds)
{
    sf::FloatRect platformBounds = getSprite().getGlobalBounds();
    for (const auto& obstacle : objectBounds) {
        CollisionInfo collision = checkCollision(platformBounds, {obstacle});
        if (collision.collided) {
            switch (collision.side) {
                case CollisionSide::Left:
                case CollisionSide::Right:
                    velocity.x = -velocity.x;  // Reverse horizontal direction
                    movingRight = !movingRight;
                    break;
                case CollisionSide::Top:
                    position.y = obstacle.top - platformBounds.height;
                    velocity.y = 0;
                    break;
                case CollisionSide::Bottom:
                    position.y = obstacle.top + obstacle.height;
                    velocity.y = 0;
                    break;
                default:
                    break;
            }
            setPosition(position);
            getSprite().setPosition(position);  // Use getSprite() instead of shape
        }
    }
}