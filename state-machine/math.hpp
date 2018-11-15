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

#include <cmath>


namespace variant_talk
{

struct Vec2
{
  float x = 0;
  float y = 0;

  constexpr Vec2& operator+=(const Vec2& vec)
  {
    x += vec.x;
    y += vec.y;
    return *this;
  }

  constexpr Vec2& operator-=(const Vec2& vec)
  {
    x -= vec.x;
    y -= vec.y;
    return *this;
  }

  constexpr Vec2& operator*=(const float scalar)
  {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  constexpr Vec2& operator/=(const float scalar)
  {
    x /= scalar;
    y /= scalar;
    return *this;
  }
};


constexpr Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
  auto result = lhs;
  result += rhs;
  return result;
}


constexpr Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
  auto result = lhs;
  result -= rhs;
  return result;
}


constexpr Vec2 operator*(const Vec2& vec, const float scalar)
{
  auto result = vec;
  result *= scalar;
  return result;
}


constexpr Vec2 operator/(const Vec2& vec, const float scalar)
{
  auto result = vec;
  result /= scalar;
  return result;
}


inline float length(const Vec2& vec)
{
  return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}


inline Vec2 normalize(const Vec2& vec)
{
  return vec / length(vec);
}


inline float distance(const Vec2& lhs, const Vec2& rhs)
{
  return length(lhs - rhs);
}


struct Rect
{
  Vec2 topLeft;
  Vec2 size;

  float top() const
  {
    return topLeft.y;
  }

  float bottom() const
  {
    return top() + size.y - 1;
  }

  float left() const
  {
    return topLeft.x;
  }

  float right() const
  {
    return left() + size.x - 1;
  }

  Vec2 center() const
  {
    return topLeft + size / 2 - Vec2{1.0f, 1.0f};
  }
};


template <typename Value>
int roundToInt(Value value)
{
  return static_cast<int>(std::round(value));
}

} // namespace variant_talk
