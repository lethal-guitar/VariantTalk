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

#include <SFML/Window.hpp>

#include <optional>


using namespace variant_talk;

namespace
{

MouseButton convert(const sf::Mouse::Button sfmlButton)
{
  switch (sfmlButton)
  {
    case sf::Mouse::Left:
      return MouseButton::Left;

    case sf::Mouse::Middle:
      return MouseButton::Middle;

    case sf::Mouse::Right:
      return MouseButton::Right;

    default:
      break;
  }

  return MouseButton::Unknown;
}


std::optional<Event> platformToGeneric(const sf::Event& event)
{
  using namespace event;

  switch (event.type)
  {
    case sf::Event::MouseMoved:
      return Event{MouseMoved{event.mouseMove.x, event.mouseMove.y}};

    case sf::Event::MouseButtonPressed:
      return Event{MouseButtonDown{convert(event.mouseButton.button)}};

    case sf::Event::MouseButtonReleased:
      return Event{MouseButtonUp{convert(event.mouseButton.button)}};

    case sf::Event::Resized:
      return Event{WindowResized{
        static_cast<int>(event.size.width),
        static_cast<int>(event.size.height)}};

    default:
      break;
  }

  return {};
}

} // namespace


int main()
{
  sf::Window window{sf::VideoMode{500, 500}, "Event Handler example"};

  ExampleEventHandler handler;

  while (window.isOpen())
  {
    sf::Event platformEvent;

    while (window.waitEvent(platformEvent))
    {
      if (platformEvent.type == sf::Event::Closed)
      {
        window.close();
        break;
      }

      if (const auto genericEvent = platformToGeneric(platformEvent))
      {
        handler.onEvent(*genericEvent);
      }
    }
  }
}
