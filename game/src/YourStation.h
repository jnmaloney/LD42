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

  void reset()
  {
    m_empireCounter = 2;
    m_asteroidCounter = 2;
    m_clams = 42;
    m_shipQueue.clear();
    m_dockedShip = SpaceShip();
    m_launchingShip = SpaceShip();
  }

  // Ships are requesting to dock
  std::vector<SpaceShip> m_shipQueue;
  SpaceShip m_dockedShip;
  SpaceShip m_launchingShip;

  int m_empireCounter = 2;
  int m_asteroidCounter = 2;

  int m_clams = 42;
};
