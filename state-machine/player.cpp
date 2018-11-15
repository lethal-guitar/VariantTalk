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

#include "player.hpp"

#include "config.hpp"
#include "projectiles.hpp"
#include "sfml_interop.hpp"

#include <algorithm>
#include <array>


namespace variant_talk
{

Player::Player(
  ProjectileManager* pProjectiles,
  ResourceBundle* pResources)
  : mpProjectiles(pProjectiles)
  , mSprite(pResources->mPlayerTextures, 0.08)
  , mMovingObject{{125, 280}, {PLAYER_WIDTH, PLAYER_HEIGHT}, {}}
  , mDestructible(MAX_HEALTH, Projectile::Type::Enemy)
{
}


void Player::update(const InputState& input, const double timeDelta)
{
  updateMovement(input, timeDelta);
  updateShooting(input);

  mpProjectiles->maybeApplyDamage(mDestructible, mMovingObject);

  mSprite.update(timeDelta);
}


void Player::render(sf::RenderWindow& window)
{
  const auto offset = mOrientation == Orientation::Left
    ? Vec2{-(50.0f + PLAYER_WIDTH), 17.0f}
    : Vec2{50.0f, 17.0f};
  const auto position = mMovingObject.bbox().topLeft - offset;
  const auto drawFlipped = mOrientation == Orientation::Left;

  mSprite.render(window, position, drawFlipped);
}


void Player::updateMovement(const InputState& input, const double timeDelta)
{
  mMovingObject.velocity() = input.direction * PLAYER_SPEED;

  if (input.direction.x != 0)
  {
    mOrientation = input.direction.x > 0
      ? Orientation::Right
      : Orientation::Left;
  }

  mMovingObject.update(timeDelta);

  constexpr float minPosX = 0.0f;
  constexpr float minPosY = UI_AREA_HEIGHT;
  constexpr float maxPosX = PLAY_AREA_WIDTH - PLAYER_WIDTH;
  constexpr float maxPosY = PLAY_AREA_HEIGHT - PLAYER_HEIGHT;
  mMovingObject.position().x =
    std::clamp(mMovingObject.position().x, minPosX, maxPosX);
  mMovingObject.position().y =
    std::clamp(mMovingObject.position().y, minPosY, maxPosY);
}


void Player::updateShooting(const InputState& input)
{
  auto fire = [this]()
  {
    const auto xOffset = mOrientation == Orientation::Left
      ? -PROJECTILE_SIZE
      : PLAYER_WIDTH;
    const auto orientationFactor =
      mOrientation == Orientation::Left ? -1.0f : 1.0f;
    const auto position = mMovingObject.position() +
      Vec2{xOffset, PLAYER_HEIGHT - PROJECTILE_SIZE};
    const auto movementVec = Vec2{orientationFactor, 0.0f};

    mpProjectiles->spawnProjectile(
      Projectile::Type::Player, position, movementVec);
  };

  if (input.firePressed)
  {
    if (!mHasFired)
    {
      fire();
      mHasFired = true;
    }
  }
  else
  {
    mHasFired = false;
  }
}

} // namespace variant_talk
