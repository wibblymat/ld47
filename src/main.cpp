#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "levels.h"

enum class GameState {
  Loading,
  Menu,
  Help,
  Credits,
  Level,
};

unsigned int worldWidth = 512;
unsigned int worldHeight = 480;

class Game: public olc::PixelGameEngine {
public:
  Game() {
    sAppName = "Looper";
    state = GameState::Loading;
    loadingCountdown = 5.0;
  }

private:
  bool upPressed() {
    return GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed;
  }

  bool downPressed() {
    return GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed;
  }

  bool leftPressed() {
    return GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::A).bPressed;
  }

  bool rightPressed() {
    return GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::D).bPressed;
  }

  bool selectPressed() {
    return GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::ENTER).bPressed;
  }

  bool upHeld() {
    return GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld;
  }

  bool downHeld() {
    return GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld;
  }

  bool leftHeld() {
    return GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld;
  }

  bool rightHeld() {
    return GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld;
  }

  bool selectHeld() {
    return GetKey(olc::Key::SPACE).bHeld || GetKey(olc::Key::ENTER).bHeld;
  }

  bool skipPressed() {
    return GetKey(olc::Key::ESCAPE).bPressed || selectPressed();
  }

  void drawLoadingScreen(float dT) {
    loadingCountdown -= dT;
    Clear(menuBackground);
    DrawStringDecal({160, 200}, "Looper", olc::BLUE, {4.0f, 4.0f});
    DrawStringDecal({104, 232}, "A game by Mat Scales for Ludum Dare 47", olc::BLUE);
    DrawStringDecal({0, 472}, "Made with olcPixelGameEngine");

    if (loadingCountdown <= 0 || selectPressed()) {
      switchToMenu();
    }
  }

  void drawCredits(float dT) {
    Clear(menuBackground);
    DrawStringDecal({100, 200}, "This game was made entirely by");
    DrawStringDecal({100, 208}, "Mat Scales", olc::YELLOW, {4.0, 4.0});
    DrawStringDecal({100, 240}, "Copyright 2020");

    if (skipPressed()) {
      switchToMenu();
    }
  }

  void drawHelp(float dT) {
    Clear(menuBackground);
    DrawStringDecal({100, 200}, "TODO: Make this help more... helpful");

    if (skipPressed()) {
      switchToMenu();
    }
  }

  void drawLevel(float dT) {
    Clear(menuBackground);
    Level level = levels[currentLevel];


    int loopHeight = worldHeight / level.loops.size();

    for (int i = 0; i < level.loops.size(); i++) {
      DrawSprite({0, i * loopHeight}, level.loops[i].background.get());
    }

    playerAcc = {0, 0};
    if (grounded) {
      playerAcc += (olc::vd2d({-8, 0}) * playerVel); // Friction
    } else {
      playerAcc += {0, 200}; // Gravity. Positive, since down is positive
    }

    if (rightHeld() && grounded) {
      playerAcc += {1000, 0};
    }
    if (leftHeld() && grounded) {
      playerAcc += {-1000, 0};
    }
    if (upPressed() && grounded) {
      playerVel += {0, -150};
    }

    playerVel += playerAcc * dT;
    playerPos += playerVel * dT;

    if (playerPos.y >= 400) {
      grounded = true;
      playerPos.y = 400;
      playerVel.y = 0;
    } else {
      grounded = false;
    }

    SetPixelMode(olc::Pixel::MASK);
    DrawPartialSprite(playerPos, sprites.get(), {0, 0}, {32, 84});
    SetPixelMode(olc::Pixel::NORMAL);

    if (skipPressed()) {
      switchToMenu();
    }
  }

  void drawMenu(float dT) {
    loadingCountdown -= dT;
    Clear(menuBackground);
    DrawStringDecal({100, 200}, "New Game");
    DrawStringDecal({100, 208}, "Help");
    DrawStringDecal({100, 216}, "Credits");
    DrawStringDecal({100, 224}, "Quit");

    if (downPressed()) {
      selectedItem++;
      selectedItem = selectedItem % 4;
    } else if (upPressed()) {
      selectedItem--;
      selectedItem = selectedItem % 4;
    }

    if (selectPressed()) {
      switch (selectedItem) {
        case 0:
          switchToLevel(0);
          break;
        case 1:
          switchToHelp();
          break;
        case 2:
          switchToCredits();
          break;
        case 3:
          exit(0);
          break;
      }
    }

    FillCircle({90, 203 + ((int)selectedItem * 8)}, 4, olc::CYAN);
  }

  void switchToMenu() {
    state = GameState::Menu;
    selectedItem = 0;
  }

  void switchToLevel(unsigned int level) {
    state = GameState::Level;
    currentLevel = level;
    inProgress = true;
    playerPos = {0, 0};
    playerVel = {0, 0};
    playerAcc = {0, 0};
  }

  void switchToHelp() {
    state = GameState::Help;
  }

  void switchToCredits() {
    state = GameState::Credits;
  }

public:
  bool OnUserCreate() override {
    menuBackground = olc::Pixel(48, 0, 48);

    background1 = std::make_shared<olc::Sprite>("assets/background1.png");
    sprites = std::make_shared<olc::Sprite>("assets/sprites.png");

    Level level1;
    level1.buttonCount = 1;

    Loop loop1_1;
    loop1_1.background = background1;

    Loop loop1_2;
    loop1_2.background = background1;

    level1.loops.push_back(loop1_1);
    level1.loops.push_back(loop1_2);

    levels.push_back(level1);
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    switch (state) {
      case GameState::Loading:
        drawLoadingScreen(fElapsedTime);
        break;
      case GameState::Help:
        drawHelp(fElapsedTime);
        break;
      case GameState::Credits:
        drawCredits(fElapsedTime);
        break;
      case GameState::Level:
        drawLevel(fElapsedTime);
        break;
      case GameState::Menu:
      default:
        drawMenu(fElapsedTime);
        break;
    }
    return true;
  }

private:
  GameState state;
  olc::Pixel menuBackground;

  // Loading screen
  double loadingCountdown;

  // Menu
  unsigned int selectedItem = 0;

  // Levels
  std::vector<Level> levels;
  unsigned int currentLevel = 0;
  bool inProgress = false;
  bool grounded = false;
  olc::vd2d playerPos;
  olc::vd2d playerVel;
  olc::vd2d playerAcc;

  // Assets
  std::shared_ptr<olc::Sprite> sprites;
  std::shared_ptr<olc::Sprite> background1;
};

int main() {
  Game game;
  if (game.Construct(worldWidth, worldHeight, 2, 2))
    game.Start();

  return 0;
}
