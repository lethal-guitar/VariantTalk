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

#include <array>


namespace variant_talk
{

constexpr auto PLAY_AREA_WIDTH = 800;
constexpr auto INFO_AREA_WIDTH = 400;
constexpr auto PLAY_AREA_HEIGHT = 600;

constexpr auto VIEW_WIDTH = PLAY_AREA_WIDTH + INFO_AREA_WIDTH;
constexpr auto VIEW_HEIGHT = PLAY_AREA_HEIGHT;

constexpr auto BAR_WIDTH = 320;
constexpr auto BAR_HEIGHT = 32;
constexpr auto BAR_GAP = 80;
constexpr auto BAR_PADDING = 3;

constexpr auto TEXT_START_Y = 8;
constexpr auto BAR_START_Y = 32;
constexpr auto TOTAL_WIDTH = BAR_WIDTH * 2 + BAR_GAP;

constexpr auto PLAYER_BAR_START_X = (PLAY_AREA_WIDTH - TOTAL_WIDTH) / 2;
constexpr auto ENEMY_BAR_START_X =
  PLAYER_BAR_START_X + BAR_WIDTH + BAR_GAP;

constexpr auto UI_AREA_HEIGHT = 72;

constexpr auto PLAYER_WIDTH = 72;
constexpr auto PLAYER_HEIGHT = 24;
constexpr auto PLAYER_SPEED = 350.0f;

constexpr auto PROJECTILE_SIZE = 8.0f;
constexpr auto PROJECTILE_RADIUS = PROJECTILE_SIZE / 2.0f;
constexpr auto PLAYER_PROJECTILE_SPEED = 900.0f;
constexpr auto ENEMY_PROJECTILE_SPEED = 250.0f;

constexpr auto GAME_OVER_SCREEN_LOCK_TIME = 2.0;

constexpr auto MAX_HEALTH = 30;

constexpr auto ENEMY_SHOT_DELAY = 1.2;
constexpr auto ENEMY_SPEED = 400.0f;

constexpr auto CIRCLE_POSITIONS = std::array<Vec2, 4>{
  Vec2{48, 48 + 60},
  Vec2{48, PLAY_AREA_HEIGHT - (48 + 24)},
  Vec2{PLAY_AREA_WIDTH - (48 + 72), PLAY_AREA_HEIGHT - (48 + 24)},
  Vec2{PLAY_AREA_WIDTH - (48 + 72), 48 + 60},
};

constexpr auto INNER_ZONE_SIZE = Vec2{
  360 - PLAYER_WIDTH, 240 - PLAY_AREA_HEIGHT};
constexpr auto INNER_ZONE = Rect{
  {(PLAY_AREA_WIDTH - INNER_ZONE_SIZE.x) / 2,
  (PLAY_AREA_HEIGHT - INNER_ZONE_SIZE.y) / 2},
  INNER_ZONE_SIZE};

} // namespace variant_talk
