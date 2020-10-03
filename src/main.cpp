#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Game: public olc::PixelGameEngine {
public:
  Game() {
    sAppName = "Looper";
  }

public:
  bool OnUserUpdate(float fElapsedTime) override {
    return true;
  }
};

int main() {
  Game game;
  if (game.Construct(256, 240, 4, 4))
    game.Start();

  return 0;
}
