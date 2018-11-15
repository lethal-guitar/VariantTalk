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

#include "enemy.hpp"

#include "player.hpp"
#include "projectiles.hpp"
#include "sfml_interop.hpp"

#include "match.hpp"

#include <SFML/Graphics/RectangleShape.hpp>


namespace variant_talk
{

namespace
{

constexpr double MAX_TIME_IN_CENTER = 2.3;
constexpr auto SCREEN_CENTER = Vec2{PLAY_AREA_WIDTH, PLAY_AREA_HEIGHT} / 2;
constexpr auto HALF_ENEMY_SIZE = Vec2{PLAYER_WIDTH, PLAYER_HEIGHT} / 2;

int cornerIndexWithMaxDistanceTo(const Vec2& referencePos)
{
  using namespace std;

  using IndexAndDistance = tuple<size_t, float>;
  array<IndexAndDistance, CIRCLE_POSITIONS.size()> distances;

  transform(begin(CIRCLE_POSITIONS), end(CIRCLE_POSITIONS), begin(distances),
    [&, i = 0u](const Vec2& position) mutable
    {
      return make_tuple(i++, distance(position, referencePos));
    });

  auto iMaxDistanceCorner = max_element(begin(distances), end(distances),
    [](const auto& lhs, const auto& rhs)
    {
      return get<1>(lhs) < get<1>(rhs);
    });

  auto [chosenIndex, _] = *iMaxDistanceCorner;
  return static_cast<int>(chosenIndex);
};


} // namespace


Enemy::Enemy(
  ProjectileManager* pProjectiles,
  const Player* pPlayer,
  ResourceBundle* pResources)
  : mpProjectiles(pProjectiles)
  , mpPlayer(pPlayer)
  , mMovingObject{
      {CIRCLE_POSITIONS[3]},
      {PLAYER_WIDTH, PLAYER_HEIGHT},
      {}}
  , mSprite(pResources->mOpponentTextures, 0.08)
  , mDestructible(MAX_HEALTH, Projectile::Type::Player)
{
  flyTo(CIRCLE_POSITIONS[0]);
}


void Enemy::update(const double timeDelta)
{
  const auto oldXVel = mMovingObject.velocity().x;

  auto updateMovement = [=]()
  {
    const auto wasMovingBeforeUpdate = mMovingObject.isMovingToPosition();
    mMovingObject.update(timeDelta);
    const auto isMovingAfterUpdate = mMovingObject.isMovingToPosition();

    const auto reachedCurrentTarget = wasMovingBeforeUpdate &&
      !isMovingAfterUpdate;
    return reachedCurrentTarget;
  };


  const auto reachedCurrentTarget = updateMovement();

  updateLogic(timeDelta, reachedCurrentTarget);

  mpProjectiles->maybeApplyDamage(mDestructible, mMovingObject);

  const auto newXVel = mMovingObject.velocity().x;
  if (oldXVel != newXVel && newXVel != 0.0f)
  {
    mFacingLeft = newXVel < 0.0f;
  }

  mSprite.update(timeDelta);
}


void Enemy::render(sf::RenderWindow& window)
{
  const auto offset = mFacingLeft
    ? Vec2{-(50.0f + PLAYER_WIDTH), 17.0f}
    : Vec2{50.0f, 17.0f};
  const auto position = mMovingObject.bbox().topLeft - offset;
  const auto drawFlipped = mFacingLeft;

  mSprite.render(window, position, drawFlipped);
}


bool Enemy::playerInInnerZone() const
{
  return toSfml(mpPlayer->bbox()).intersects(toSfml(INNER_ZONE));
}


bool Enemy::playerInOuterZone() const
{
  return !playerInInnerZone();
}


void Enemy::updateLogic(const double timeDelta, const bool reachedCurrentTarget)
{
  using MaybeNextState = std::optional<State>;
  auto maybeNextState = match(mState,
    [=](Circling& state) -> MaybeNextState
    {
      if (reachedCurrentTarget)
      {
        ++state.mNextCirclePosIndex;
        if (state.mNextCirclePosIndex >= CIRCLE_POSITIONS.size())
        {
          state.mNextCirclePosIndex = 0;
        }

        const auto& newTarget = CIRCLE_POSITIONS[state.mNextCirclePosIndex];
        flyTo(newTarget);
      }

      shootAtPlayer(state, timeDelta);

      if (playerInOuterZone() && reachedCurrentTarget)
      {
        flyTo(SCREEN_CENTER - HALF_ENEMY_SIZE);
        return State{FlyToCenter{}};
      }

      return std::nullopt;
    },

    [=](const FlyToCenter&) -> MaybeNextState
    {
      if (reachedCurrentTarget)
      {
        return State{ShootingFromCenter{}};
      }

      return std::nullopt;
    },

    [=](ShootingFromCenter& state) -> MaybeNextState
    {
      state.mTimeSpentInCenter += timeDelta;
      const auto spentEnoughTimeInCenter =
        state.mTimeSpentInCenter >= MAX_TIME_IN_CENTER;

      if (playerInInnerZone() || spentEnoughTimeInCenter)
      {
        const auto chosenIndex = cornerIndexWithMaxDistanceTo(
          mpPlayer->bbox().center());

        flyTo(CIRCLE_POSITIONS[chosenIndex]);
        const auto nextIndex = (chosenIndex + 1) %
          static_cast<int>(CIRCLE_POSITIONS.size());

        return State{FlyOut{nextIndex}};
      }

      shootAtPlayer(state, timeDelta);

      return std::nullopt;
    },

    [=](const FlyOut& state) -> MaybeNextState
    {
      if (reachedCurrentTarget)
      {
        flyTo(CIRCLE_POSITIONS[state.mTargetCornerIndex]);
        return State{Circling{state.mTargetCornerIndex}};
      }

      return std::nullopt;
    }
  );

  if (maybeNextState)
  {
    mState = *maybeNextState;
  }
}


void Enemy::flyTo(const Vec2& position)
{
  mMovingObject.moveTo(position, ENEMY_SPEED);
}


template <typename StateWithShooting>
void Enemy::shootAtPlayer(StateWithShooting& state, const double timeDelta)
{
  state.mTimeSinceLastShot += timeDelta;
  if (state.mTimeSinceLastShot >= ENEMY_SHOT_DELAY)
  {
    spawnProjectile();
    state.mTimeSinceLastShot = 0.0;
  }
}


void Enemy::spawnProjectile()
{
  const auto offsetToProjectileCenter =
    Vec2{PROJECTILE_SIZE, PROJECTILE_SIZE} / 2;
  const auto origin = mMovingObject.bbox().center() -
    offsetToProjectileCenter;
  const auto target = mpPlayer->bbox().center() - offsetToProjectileCenter;
  const auto vecToPlayer = normalize(target - origin);

  mpProjectiles->spawnProjectile(
    Projectile::Type::Enemy,
    origin,
    vecToPlayer);
}

} // namespace variant_talk
