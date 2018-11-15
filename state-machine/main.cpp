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

#include "match.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <cmath>
#include <string>
#include <string_view>


using namespace variant_talk;


namespace
{

constexpr auto NUM_STATES = 4u;
constexpr auto STATE_NAMES = std::array<const char*, NUM_STATES>{
  "Circling",
  "FlyToCenter",
  "ShootingFromCenter",
  "FlyOut"
};


struct GameWorld
{
  GameWorld(ResourceBundle* pResources);

  GameWorld(const GameWorld&) = delete;
  GameWorld& operator=(const GameWorld&) = delete;

  void handleEvent(const sf::Event& event);

  void update(double timeDelta)
  {
    mProjectiles.update(timeDelta);
    mPlayer.update(mInputState, timeDelta);
    mEnemy.update(timeDelta);
  }

  void render(sf::RenderWindow& window)
  {
    mPlayer.render(window);
    mEnemy.render(window);
    mProjectiles.render(window);
  }

  bool gameEnded() const
  {
    return mPlayer.health() == 0 || mEnemy.health() == 0;
  }

  InputState mInputState;

  ProjectileManager mProjectiles;
  Player mPlayer;
  Enemy mEnemy;
};


void GameWorld::handleEvent(const sf::Event& event)
{
  const auto isPressed = event.type == sf::Event::KeyPressed;
  const auto isPressedAsFloat = isPressed ? 1.0f : 0.0f;

  switch (event.key.code)
  {
    case sf::Keyboard::Left:
      mInputState.direction.x = -isPressedAsFloat;
      break;

    case sf::Keyboard::Right:
      mInputState.direction.x = isPressedAsFloat;
      break;

    case sf::Keyboard::Up:
      mInputState.direction.y = -isPressedAsFloat;
      break;

    case sf::Keyboard::Down:
      mInputState.direction.y = isPressedAsFloat;
      break;

    case sf::Keyboard::Space:
      mInputState.firePressed = isPressed;
      break;

    default:
      break;
  }
}


class Game
{
public:
  explicit Game(sf::RenderWindow& window);

  void handleEvent(const sf::Event& event);
  void updateAndRender(const double timeDelta);

private:
  enum class Party
  {
    Player,
    Enemy
  };

  using InGame = std::unique_ptr<GameWorld>;

  struct GameOver
  {
    Party mWinningParty;
    double mTimeElapsed = 0.0;
  };

  using State = std::variant<InGame, GameOver>;

  void update(double timeDelta);
  void render();
  void renderBackground();
  void renderUi(int playerHealth, int enemyHealth);
  void renderGameEndMessage(Party winningParty);
  void renderStateMachineVisualization(const GameWorld& world);

  sf::FloatRect measureText(int characterSize, std::string_view string) const;

  void drawText(
    float x,
    float y,
    int characterSize,
    const sf::Color& color,
    std::string_view string);

  void drawRect(
    float x,
    float y,
    float width,
    float height,
    const sf::Color& color);

  sf::RenderWindow& mWindow;

  sf::Font mFont;
  sf::Texture mBackground;
  ResourceBundle mResources;

  std::array<sf::Texture, NUM_STATES> mStateVisualizations;

  State mState;
};


GameWorld::GameWorld(ResourceBundle* pResources)
  : mPlayer(&mProjectiles, pResources)
  , mEnemy(&mProjectiles, &mPlayer, pResources)
{
}


Game::Game(sf::RenderWindow& window)
  : mWindow(window)
  , mState(std::make_unique<GameWorld>(&mResources))
{
  using namespace std::literals;

  const auto kBasePath = "resources/"s;

  mFont.loadFromFile(kBasePath + "DroidSans.ttf");
  mBackground.loadFromFile(kBasePath + "space-bg.jpg");
  mBackground.setSmooth(true);

  for (auto i = 0u; i < NUM_STATES; ++i)
  {
    mStateVisualizations[i].loadFromFile(kBasePath + STATE_NAMES[i] + ".png");
    mStateVisualizations[i].setSmooth(true);
  }

  mResources.mPlayerTextures[0].loadFromFile(kBasePath + "player1.png");
  mResources.mPlayerTextures[1].loadFromFile(kBasePath + "player2.png");

  mResources.mOpponentTextures[0].loadFromFile(kBasePath + "opponent1.png");
  mResources.mOpponentTextures[1].loadFromFile(kBasePath + "opponent2.png");
}


void Game::handleEvent(const sf::Event& event)
{
  const auto isKeyboardEvent =
    event.type == sf::Event::KeyPressed ||
    event.type == sf::Event::KeyReleased;
  if (!isKeyboardEvent)
  {
    return;
  }

  using MaybeNextState = std::optional<State>;
  auto maybeNextState = match(mState,
    [&](InGame& state) -> MaybeNextState
    {
      state->handleEvent(event);
      return std::nullopt;
    },

    [this](const GameOver& state) -> MaybeNextState
    {
      if (state.mTimeElapsed >= GAME_OVER_SCREEN_LOCK_TIME)
      {
        return State{std::make_unique<GameWorld>(&mResources)};
      }

      return std::nullopt;
    });

  if (maybeNextState)
  {
    mState = std::move(*maybeNextState);
  }
}


void Game::updateAndRender(const double timeDelta)
{
  update(timeDelta);
  render();
}


void Game::update(const double timeDelta)
{
  using MaybeNextState = std::optional<State>;
  auto maybeNextState = match(mState,
    [timeDelta](InGame& state) -> MaybeNextState
    {
      state->update(timeDelta);

      if (state->gameEnded())
      {
        const auto winningParty = state->mPlayer.health() > 0
          ? Party::Player
          : Party::Enemy;
        return State{GameOver{winningParty}};
      }
      return std::nullopt;
    },

    [timeDelta](GameOver& state) -> MaybeNextState
    {
      state.mTimeElapsed += timeDelta;
      return std::nullopt;
    });

  if (maybeNextState)
  {
    mState = std::move(*maybeNextState);
  }
}


void Game::render()
{
  renderBackground();

  match(mState,
    [this](const InGame& state)
    {
      state->render(mWindow);
      renderUi(state->mPlayer.health(), state->mEnemy.health());
      renderStateMachineVisualization(*state);
    },

    [this](const GameOver& state)
    {
      // Info area background
      drawRect(
        PLAY_AREA_WIDTH, 0,
        INFO_AREA_WIDTH, PLAY_AREA_HEIGHT,
        sf::Color{180, 180, 180});

      renderUi(0, 0);
      renderGameEndMessage(state.mWinningParty);
    });

  mWindow.display();
}


void Game::renderBackground()
{
  constexpr auto kScale = 0.5f;

  sf::Sprite background;
  background.setTexture(mBackground);
  background.setScale(kScale, kScale);
  background.setPosition(0, UI_AREA_HEIGHT);

  mWindow.draw(background);
}


void Game::renderUi(const int playerHealth, const int enemyHealth)
{
  auto darken = [](const sf::Uint8 color)
  {
    return static_cast<sf::Uint8>(roundToInt(color * 0.8f));
  };

  auto drawHealthBar = [&](
    const float startX,
    const sf::Uint8 r,
    const sf::Uint8 g,
    const sf::Uint8 b,
    const float health)
  {
    auto& d = darken;

    drawRect(
      startX, BAR_START_Y,
      BAR_WIDTH, BAR_HEIGHT,
      sf::Color{d(r), d(g), d(b), 180});

    drawRect(
      startX + BAR_PADDING, BAR_START_Y + BAR_PADDING,
      health * (BAR_WIDTH - BAR_PADDING * 2), BAR_HEIGHT - BAR_PADDING * 2,
      sf::Color{r, g, b, 180});
  };

  auto normalize = [](const int health)
  {
    return static_cast<float>(health) / MAX_HEALTH;
  };

  drawRect(0, 0, PLAY_AREA_WIDTH, UI_AREA_HEIGHT, sf::Color{90, 90, 90});

  drawText(PLAYER_BAR_START_X, TEXT_START_Y, 16, sf::Color::White, "Player");
  drawText(ENEMY_BAR_START_X, TEXT_START_Y, 16, sf::Color::White, "Enemy");

  drawHealthBar(PLAYER_BAR_START_X, 0, 255, 0, normalize(playerHealth));
  drawHealthBar(ENEMY_BAR_START_X, 255, 0, 0, normalize(enemyHealth));
}


void Game::renderGameEndMessage(const Party winningParty)
{
  const auto characterSize = 48;
  const auto message = winningParty == Party::Player
    ? "Victory!"
    : "Try again...";
  const auto textSize = measureText(48, message);

  drawText(
    PLAY_AREA_WIDTH / 2.0f - textSize.width / 2.0f,
    PLAY_AREA_HEIGHT / 2.0f - textSize.height / 2.0f,
    characterSize,
    sf::Color::White,
    message);
}


void Game::renderStateMachineVisualization(const GameWorld& world)
{
  drawRect(
    PLAY_AREA_WIDTH, 0,
    INFO_AREA_WIDTH, PLAY_AREA_HEIGHT,
    sf::Color{180, 180, 180});

  {
    sf::Sprite sprite;
    sprite.setTexture(mStateVisualizations[world.mEnemy.state().index()]);
    sprite.setPosition(PLAY_AREA_WIDTH, 25);
    sprite.setScale(0.5f, 0.5f);

    mWindow.draw(sprite);
  }

  {
    const auto message = world.mEnemy.playerInInnerZone()
      ? "Player in INNER zone"
      : "Player in OUTER zone";

    drawText(
      PLAY_AREA_WIDTH + 10,
      5,
      14,
      sf::Color::Black,
      message);
  }
}


void Game::drawText(
  const float x,
  const float y,
  const int characterSize,
  const sf::Color& color,
  std::string_view string)
{
  sf::Text text;
  text.setFont(mFont);

  text.setPosition(x, y);
  text.setCharacterSize(characterSize);
  text.setFillColor(color);
  text.setString(std::string(string));

  mWindow.draw(text);
}


sf::FloatRect Game::measureText(
  const int characterSize,
  const std::string_view string) const
{
  sf::Text text;
  text.setFont(mFont);

  text.setCharacterSize(characterSize);
  text.setString(std::string(string));

  return text.getLocalBounds();
}


void Game::drawRect(
  const float x,
  const float y,
  const float width,
  const float height,
  const sf::Color& color)
{
  sf::RectangleShape rect;
  rect.setPosition(x, y);
  rect.setSize(sf::Vector2f{width, height});
  rect.setFillColor(color);

  mWindow.draw(rect);
}


void pumpEvents(sf::Window& window, Game& game)
{
  sf::Event event;

  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
      window.close();
      return;
    }

    game.handleEvent(event);
  }
}

} // namespace


int main()
{
  namespace cr = std::chrono;
  using Clock = cr::high_resolution_clock;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;

  const auto desktopSize = sf::VideoMode::getDesktopMode();

  sf::RenderWindow window{
    desktopSize,
    "State Machine example",
    sf::Style::Default,
    settings};
  window.setVerticalSyncEnabled(true);

  sf::View gameView{sf::FloatRect(0.0f, 0.0f, VIEW_WIDTH, VIEW_HEIGHT)};
  window.setView(gameView);

  Game game{window};

  auto lastFrameTimeStamp = Clock::now();

  while (window.isOpen())
  {
    const auto now = Clock::now();
    const auto elapsedTime = now - lastFrameTimeStamp;
    lastFrameTimeStamp = now;

    pumpEvents(window, game);

    game.updateAndRender(cr::duration<double>{elapsedTime}.count());
  }

  return 0;
}
