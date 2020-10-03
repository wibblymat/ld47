#pragma once

#include "olcPixelGameEngine.h"
#include <string>
#include <vector>
#include <memory>

enum class EntityType {
  Button,
  InPortal,
  OutPortal,
  Bee,
};

struct Entity {
  EntityType type;
  olc::vd2d start;
};

struct Loop {
  std::shared_ptr<olc::Sprite> background;
  std::vector<Entity> entities;
};

struct Level {
  std::string name;
  unsigned int buttonCount;
  std::vector<Loop> loops;
};
