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
#include "math.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>


namespace variant_talk
{

class ProjectileManager
{
public:
  void spawnProjectile(
    const Projectile::Type type,
    const Vec2& position,
    const Vec2& movementVec);

  // Tests if the given MovingObject intersects any currently active projectile
  // of the right type, and deals damage to the given DestructibleObject if
  // that's the case. The intersecting projectile (if any) will be destroyed.
  void maybeApplyDamage(
    DestructibleObject& destructible,
    const MovingObject& movingObject);

  void update(const double timeDelta);
  void render(sf::RenderWindow& window);

private:
  std::vector<Projectile> mProjectiles;
};

} // namespace variant_talk
