#ifndef PLAYER_DATA_STORAGE_HPP_
#define PLAYER_DATA_STORAGE_HPP_

#include "game/ecs/components/player_data_component.hpp"

namespace dib {
class PlayerDataStorage
{
public:
  static PlayerData Load()
  {
    PlayerData data{};
    data.name = "Rully";
    data.body = 3;
    data.direction = 1;
    data.player_class = 2;
    data.speed = 1.37;
    data.xp = 72117;
    return data;
  }

private:
};
}

#endif // PLAYER_DATA_STORAGE_HPP_