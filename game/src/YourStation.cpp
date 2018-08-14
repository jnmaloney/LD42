#include "YourStation.h"
#include <chrono>
#include <random>


YourStation::YourStation()
{
  m_shipQueue.resize(0);
}


YourStation::~YourStation()
{

}


void YourStation::launch()
{
  static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator(seed);
  static std::uniform_int_distribution<int> distribution(1, 5);
  static std::uniform_int_distribution<int> distribution2(0, 6);
  static auto dice = std::bind ( distribution, generator );
  static auto dice2 = std::bind ( distribution2, generator );



  // Move up the ship queue

  m_launchingShip = m_dockedShip;
  m_launchingShip.docked = 1;
  m_launchingShip.timer = 0.f;

  m_dockedShip = m_shipQueue[0];
  m_dockedShip.docked = -1;
  m_dockedShip.timer = -1.f;


  // Set up the next encounter
  int d = dice();
  if (d <= 3)
  {
    // Normal Trade
    m_dockedShip.encounter_type = 0;

    m_dockedShip.encounter_mod = dice2(); // item type to sell
  }
  else if (d == 4)
  {
    // Empire time
    --m_empireCounter;
    if (m_empireCounter == 0)
    {
      m_empireCounter = 2 + dice2()%2 + dice2()%2;
      m_dockedShip.encounter_type = 3;  // Pick up encounter
      if (m_asteroidCounter == 1) m_asteroidCounter = 2;
    }
    else
      m_dockedShip.encounter_type = 1;  // Drop off encounter
  }
  else if (d == 5)
  {
    // Asteroid event
    --m_asteroidCounter;
    if (m_asteroidCounter == 0)
    {
      m_asteroidCounter = 2;
      m_dockedShip.encounter_type = 2; // Take Cover
    }
    else
    {
      m_dockedShip.encounter_type = 4; // Repairs
    }
  }

  // erase...
}
