#pragma once


#include <vector>


class SpaceShip;
class StationObject;


// Helper class
class ObjectIndexMaker
{
public:
  static int getNewIndex() { return ++index; }

protected:
  static int index;
};


// An object that you can move around
class StationObject
{
public:
  StationObject() {}
  ~StationObject() {}

  int id = 0;
  int object_type = 0;
  int i = 0;
  int j = 0;
  int rotation = 0;
  int colour = 0;
};


// Docking Ships
class SpaceShip
{
public:
  SpaceShip();
  ~SpaceShip();

  float timer = -1.f;
  int docked = -1;

  std::vector<StationObject> m_objects;
};
