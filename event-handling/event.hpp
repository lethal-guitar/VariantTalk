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

#include <variant>


namespace variant_talk
{

enum class MouseButton
{
  Left,
  Middle,
  Right,
  Unknown
};


namespace event
{

struct MouseMoved
{
  int x = 0;
  int y = 0;
};


struct MouseButtonDown
{
  MouseButton button;
};


struct MouseButtonUp
{
  MouseButton button;
};


struct WindowResized
{
  int newWidth = 0;
  int newHeight = 0;
};

} // namespace event


using Event = std::variant<
  event::MouseMoved,
  event::MouseButtonDown,
  event::MouseButtonUp,
  event::WindowResized>;

} // namespace variant_talk
