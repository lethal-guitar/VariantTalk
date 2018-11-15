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
#include "resource_bundle.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


namespace variant_talk
{

class ProjectileManager;


struct InputState
{
  Vec2 direction;
  bool firePressed = false;
};


enum class Orientation
{
  Left,
  Right
};


class Player
{
public:
  Player(
    ProjectileManager* pProjectiles,
    ResourceBundle* pResources);

  void update(const InputState& input, const double timeDelta);
  void render(sf::RenderWindow& window);

  int health() const;
  Rect bbox() const;

private:
  void updateMovement(const InputState& input, const double timeDelta);
  void updateShooting(const InputState& input);

  ProjectileManager* mpProjectiles;

  AnimatedSprite mSprite;
  MovingObject mMovingObject;
  DestructibleObject mDestructible;

  Orientation mOrientation = Orientation::Right;
  bool mHasFired = false;
};


inline int Player::health() const
{
  return mDestructible.health();
}


inline Rect Player::bbox() const
{
  return mMovingObject.bbox();
}

} // namespace variant_talk
