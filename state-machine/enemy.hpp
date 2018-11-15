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

#include "components.hpp"
#include "config.hpp"
#include "math.hpp"
#include "resource_bundle.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <variant>


namespace variant_talk
{

class Player;
class ProjectileManager;


struct Circling
{
  Circling() = default;
  explicit Circling(const int startIndex)
    : mNextCirclePosIndex(startIndex)
  {
  }

  double mTimeSinceLastShot = ENEMY_SHOT_DELAY;
  int mNextCirclePosIndex = 0;
};


struct FlyToCenter
{
};


struct ShootingFromCenter
{
  double mTimeSinceLastShot = ENEMY_SHOT_DELAY;
  double mTimeSpentInCenter = 0;
};


struct FlyOut
{
  explicit FlyOut(const int cornerIndex)
    : mTargetCornerIndex(cornerIndex)
  {
  }

  int mTargetCornerIndex;
};


using State = std::variant<
  Circling,
  FlyToCenter,
  ShootingFromCenter,
  FlyOut>;


class Enemy
{
public:
  Enemy(
    ProjectileManager* pProjectiles,
    const Player* pPlayer,
    ResourceBundle* pResources);

  void update(const double timeDelta);

  void render(sf::RenderWindow& window);

  int health() const;
  const State& state() const;

  bool playerInInnerZone() const;
  bool playerInOuterZone() const;

private:
  void updateLogic(double timeDelta, bool reachedCurrentTarget);
  void flyTo(const Vec2& position);

  template <typename StateWithShooting>
  void shootAtPlayer(StateWithShooting& state, const double timeDelta);
  void spawnProjectile();


  ProjectileManager* mpProjectiles;
  const Player* mpPlayer;

  State mState;
  MovingObject mMovingObject;
  AnimatedSprite mSprite;
  bool mFacingLeft = true;
  DestructibleObject mDestructible;
};


inline int Enemy::health() const
{
  return mDestructible.health();
}


inline const State& Enemy::state() const
{
  return mState;
}

} // namespace variant_talk
