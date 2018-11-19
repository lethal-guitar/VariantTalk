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

#include "components.hpp"

#include "config.hpp"
#include "sfml_interop.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <algorithm>


namespace variant_talk
{

MovingObject::MovingObject(Vec2 position, Vec2 size, Vec2 velocity)
  : mPosition(position)
  , mSize(size)
  , mVelocity(velocity)
{
}


const Vec2& MovingObject::position() const
{
  return mPosition;
}


const Vec2& MovingObject::velocity() const
{
  return mVelocity;
}


const Vec2& MovingObject::size() const
{
  return mSize;
}


Rect MovingObject::bbox() const
{
  return Rect{mPosition, mSize};
}


Vec2& MovingObject::position()
{
  return mPosition;
}


Vec2& MovingObject::velocity()
{
  return mVelocity;
}


void MovingObject::moveTo(const Vec2& target, float speed)
{
  mTargetPosition = target;

  const auto vecToTarget = normalize(target - mPosition);
  mVelocity = vecToTarget * speed;
}


bool MovingObject::isMovingToPosition() const
{
  return mTargetPosition.has_value();
}


void MovingObject::update(const double timeDelta)
{
  const auto scaledVelocity = mVelocity * timeDelta;
  const auto newPosition = mPosition + scaledVelocity;

  if (mTargetPosition)
  {
    if (length(scaledVelocity) >= distance(mPosition, *mTargetPosition))
    {
      mPosition = *mTargetPosition;
      mVelocity = {};
      mTargetPosition = std::nullopt;
      return;
    }
  }

  mPosition = newPosition;
}


DestructibleObject::DestructibleObject(
  const int health,
  const Projectile::Type affectingType
)
  : mHealth(health)
  , mAffectingType(affectingType)
{
}


void DestructibleObject::takeDamage()
{
  mHealth = std::clamp(mHealth - 1, 0, MAX_HEALTH);
}


int DestructibleObject::health() const
{
  return mHealth;
}


Projectile::Type DestructibleObject::affectingType() const
{
  return mAffectingType;
}


void AnimatedSprite::update(const double timeDelta)
{
  mElapsedSinceAnimationSwitch += timeDelta;
  if (mElapsedSinceAnimationSwitch >= mDelay)
  {
    mElapsedSinceAnimationSwitch -= mDelay;
    ++mAnimationIndex;
    if (mAnimationIndex >= mNumFrames) {
      mAnimationIndex = 0;
    }
  }
}


void AnimatedSprite::render(
  sf::RenderWindow& window,
  const Vec2& position,
  const bool flipHorizontally)
{
  sf::Sprite sprite;
  sprite.setTexture(mpFrames[mAnimationIndex]);
  sprite.setPosition(toSfml(position));

  if (flipHorizontally)
  {
    sprite.setScale(-1.0f, 1.0f);
  }

  window.draw(sprite);
}

} // namespace variant_talk
