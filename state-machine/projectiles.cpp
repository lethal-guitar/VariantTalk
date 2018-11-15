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

#include "projectiles.hpp"

#include "config.hpp"
#include "sfml_interop.hpp"

#include <SFML/Graphics/CircleShape.hpp>


namespace variant_talk
{

namespace
{

auto findCollidingProjectile(
  const MovingObject& object,
  const Projectile::Type affectingType,
  std::vector<Projectile>& projectiles)
{
  using namespace std;

  const auto objectRect = toSfml(object.bbox());

  return find_if(begin(projectiles), end(projectiles),
    [&](const Projectile& projectile) -> bool
    {
      if (projectile.mType == affectingType && !projectile.mIsDestroyed)
      {
        const auto projectileRect = toSfml(projectile.mMovingObject.bbox());
        return objectRect.intersects(projectileRect);
      }

      return false;
    });
}

} // namespace


bool Projectile::isOffScreen() const
{
  const sf::FloatRect screenRect(0, 0, PLAY_AREA_WIDTH, PLAY_AREA_HEIGHT);
  const auto bboxRect = toSfml(mMovingObject.bbox());

  return !screenRect.intersects(bboxRect);
}


void ProjectileManager::spawnProjectile(
  const Projectile::Type type,
  const Vec2& position,
  const Vec2& movementVec)
{
  const auto size = Vec2{PROJECTILE_SIZE, PROJECTILE_SIZE};
  const auto speed = type == Projectile::Type::Player
    ? PLAYER_PROJECTILE_SPEED
    : ENEMY_PROJECTILE_SPEED;
  mProjectiles.push_back(Projectile{
    MovingObject{position, size, movementVec * speed},
    type});
}


void ProjectileManager::maybeApplyDamage(
  DestructibleObject& destructible,
  const MovingObject& movingObject)
{
  const auto iCollidingProjectile = findCollidingProjectile(
    movingObject, destructible.affectingType(), mProjectiles);

  if (iCollidingProjectile != std::end(mProjectiles))
  {
    iCollidingProjectile->mIsDestroyed = true;
    destructible.takeDamage();
  }
}


void ProjectileManager::update(const double timeDelta)
{
  using namespace std;

  for (auto& projectile : mProjectiles)
  {
    projectile.update(timeDelta);
  }

  mProjectiles.erase(
    remove_if(begin(mProjectiles), end(mProjectiles),
      [](const Projectile& projectile)
      {
        return projectile.isOffScreen() || projectile.mIsDestroyed;
      }),
    end(mProjectiles));
}


void ProjectileManager::render(sf::RenderWindow& window)
{
  for (auto& projectile : mProjectiles)
  {
    if (projectile.mIsDestroyed)
    {
      continue;
    }

    sf::CircleShape shape{PROJECTILE_RADIUS};
    shape.setFillColor(projectile.mType == Projectile::Type::Player
      ? sf::Color{0, 255, 0}
      : sf::Color{255, 0, 0});
    shape.setPosition(toSfml(projectile.mMovingObject.bbox().topLeft));

    window.draw(shape);
  }
}

} // namespace variant_talk
