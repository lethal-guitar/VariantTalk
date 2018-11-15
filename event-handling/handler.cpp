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

#include "handler.hpp"

#include "match.hpp"

#include <iostream>


namespace variant_talk
{

namespace
{

std::ostream& operator<<(std::ostream& os, const MouseButton button)
{
  switch (button)
  {
    case MouseButton::Left:
      os << "Left";
      break;

    case MouseButton::Middle:
      os << "Middle";
      break;

    case MouseButton::Right:
      os << "Right";
      break;

    case MouseButton::Unknown:
      os << "Unknown";
      break;
  }

  return os;
}

}


void ExampleEventHandler::onEvent(const Event& event)
{
  using namespace event;

  match(event,
    [this](const MouseMoved& mouseMove)
    {
      if (mShouldPrintMouseMoves)
      {
        std::cout << "mouse moved to " <<
          mouseMove.x << ", " << mouseMove.y << "\n";
      }
    },

    [this](const MouseButtonDown& buttonDown)
    {
      std::cout << "mouse button down (" << buttonDown.button << ")\n";

      if (buttonDown.button == MouseButton::Right)
      {
        mShouldPrintMouseMoves = !mShouldPrintMouseMoves;
        std::cout << "\nprinting mouse moves: " << std::boolalpha <<
          mShouldPrintMouseMoves << "\n\n";
      }
    },

    [](const MouseButtonUp& buttonUp)
    {
      std::cout << "mouse button up (" << buttonUp.button << ")\n";
    },

    [](const WindowResized& resized)
    {
      std::cout << "window resized to " <<
        resized.newWidth << ", " << resized.newHeight << "\n";
    });
}

} // namespace variant_talk
