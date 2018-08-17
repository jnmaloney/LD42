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

  void move_mouse();
  void action();
  void cancel();

  void reset()
  {
    gameOver = false;

    m_crashed1 = false;
    m_crashed2 = false;
    m_crashed3 = false;
    m_objects.clear();
    m_holding = false;
    cursor_place = -1;
    memset(m_stationArray, 0, 15*15*sizeof(int));
    memset(m_shipArray, 0, 15*15*sizeof(int));
    doMask();
  }

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

  int cursor_place = 0;
  int cursor_i = 0;
  int cursor_j = 0;

  std::vector<Footprint> m_footprints;

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
