#pragma once


#include "RenderSystem.h"
#include "RenderQueue.h"
#include "ActionManager.h"
#include "YourStation.h"


class SpaceMapRenderer
{
public:
  SpaceMapRenderer();
  ~SpaceMapRenderer();

  void tick();

  RenderQueue m_renderQueue;
  RenderSystem* m_rs;
  ActionManager* m_am;
  YourStation* m_station;

protected:

  void renderShip(SpaceShip& ship, glm::vec3& shipOffset);

};
