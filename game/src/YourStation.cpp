#include "YourStation.h"


YourStation::YourStation()
{
  m_shipQueue.resize(0);
}


YourStation::~YourStation()
{

}


void YourStation::launch()
{
  // Move up the ship queue

  m_launchingShip = m_dockedShip;
  m_launchingShip.docked = 1;
  m_launchingShip.timer = 0.f;

  m_dockedShip = m_shipQueue[0];
  m_dockedShip.docked = -1;
  m_dockedShip.timer = -1.f;

  // erase...
}
