/* Copyright (C) 2018, Nikolai Wuttke. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "math.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <optional>


namespace variant_talk
{

class MovingObject
{
public:
  MovingObject(Vec2 position, Vec2 size, Vec2 velocity = {});

  const Vec2& position() const;
  const Vec2& velocity() const;
  const Vec2& size() const;

  Rect bbox() const;

  Vec2& position();
  Vec2& velocity();

  void moveTo(const Vec2& target, float speed);
  bool isMovingToPosition() const;

  void update(const double timeDelta);

private:
  Vec2 mPosition;
  Vec2 mSize;
  Vec2 mVelocity;

  std::optional<Vec2> mTargetPosition;
};


struct Projectile
{
  enum class Type
  {
    Player,
    Enemy
  };

  void update(const double timeDelta);

  bool isOffScreen() const;

  MovingObject mMovingObject;
  Type mType;
  bool mIsDestroyed = false;
};


inline void Projectile::update(const double timeDelta)
{
  mMovingObject.update(timeDelta);
}



class DestructibleObject
{
public:
  DestructibleObject(int health, Projectile::Type affectingType);

  void takeDamage();
  int health() const;
  Projectile::Type affectingType() const;

private:
  int mHealth;
  Projectile::Type mAffectingType;
};


class AnimatedSprite
{
public:
  template<std::size_t numFrames>
  AnimatedSprite(
    const std::array<sf::Texture, numFrames>& frames,
    const double delay
  )
    : mpFrames(frames.data())
    , mNumFrames(numFrames)
    , mDelay(delay)
  {
  }

  void update(const double timeDelta);

  void render(
    sf::RenderWindow& window,
    const Vec2& position,
    bool flipHorizontally);

private:
  const sf::Texture* mpFrames = nullptr;
  std::size_t mNumFrames = 0;
  std::size_t mAnimationIndex = 0;
  double mDelay = 0.0;
  double mElapsedSinceAnimationSwitch = 0.0;
};

} // namespace variant_talk
