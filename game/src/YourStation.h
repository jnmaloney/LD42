#pragma once


#include "SpaceItems.h"
#include <vector>


// Your Station
class YourStation
{
public:

  YourStation();
  ~YourStation();

  void launch();

  // Ships are requesting to dock
  std::vector<SpaceShip> m_shipQueue;
  SpaceShip m_dockedShip;
  SpaceShip m_launchingShip;
};
