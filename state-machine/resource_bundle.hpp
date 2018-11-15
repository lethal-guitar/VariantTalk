#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <array>


namespace variant_talk
{

struct ResourceBundle
{
  std::array<sf::Texture, 2> mPlayerTextures;
  std::array<sf::Texture, 2> mOpponentTextures;
};

} // namespace variant_talk
