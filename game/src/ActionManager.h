#pragma once


#include "RenderSystem.h"
#include "YourStation.h"
#include "SpaceItems.h"
#include <vector>


class ActionManager
{
public:

  ActionManager();
  ~ActionManager();

  void action();
  void cancel();

  RenderSystem* m_rs;
  YourStation* m_station;

  bool m_holding = false;
  int m_holdItem = 0;

  bool m_returnToShip = false;
  bool m_returnToStation = false;
  int m_return_i = 3;
  int m_return_j = 3;
  int m_return_rotation = 0;

  int m_stationArray[15][15];
  int m_shipArray[15][15];

  // Station object list
  std::vector<StationObject> m_objects;

  // Ship object list
  //std::vector<StationObject> m_objectsOnShip;

  bool indexFromPoint(int& place, int& i, int& j, float x, float y);
  void pointFromIndex(int place, int i, int j, glm::vec3& translation);

  std::vector<glm::mat4> footprints;
  std::vector<std::vector<glm::vec3 > > offsets;
  std::vector<std::vector<glm::vec3 > > square_offsets;

  StationObject nullObject;
  StationObject m_heldObject = nullObject;

  void loadShip();
  void loadShipEmpire();
  void asteroid();
  void asteroid_strike();
  void empire_collect();

  float getFootprint(int i, int j, StationObject& o);

  void doMask();

  bool gameOver = false;

  bool m_crashed1 = false;
  bool m_crashed2 = false;
  bool m_crashed3 = false;

  int m_crash1Mask[15][7] =
  {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 0, 0, 0 },
  };

  int m_crash2Mask[15][7] =
  {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1 },
    { 0, 0, 1, 1, 1, 1, 1 },
    { 0, 0, 1, 1, 1, 1, 1 },
    { 0, 0, 0, 1, 1, 1, 1 },
    { 0, 0, 0, 1, 1, 1, 1 },
    { 0, 0, 0, 1, 1, 1, 1 },
    { 0, 0, 0, 1, 1, 1, 1 },
  };

  int m_crash3Mask[15][7] =
  {
    { 0, 0, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 1, 1, 1, 1 },
    { 0, 0, 0, 1, 1, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 1 },
    { 0, 0, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
  };

protected:

  bool canPlace(int place, int i, int j, StationObject& o);
  void placeItem(int place, int i, int j, StationObject& o);
  void clearItem(int place, int i, int j);
  void clearItem(int place, StationObject& o);

  void placeArray(std::vector<StationObject>& array, int i, int j, int pattern, int id);

  void crushItem(int id);

};
