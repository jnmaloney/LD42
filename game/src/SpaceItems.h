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


// Need one of these for every object type
class Footprint
{
public:
  Footprint(int id);
  ~Footprint();

  int getHeight(int rotation);
  int getWidth(int rotation);
  int getPoint(int i, int j, int rotation);

protected:
  int height;
  int width;
  std::vector<int> array;
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
  int cost = 0;
  int sell_price = 0;
};


// Docking Ships
class SpaceShip
{
public:
  SpaceShip();
  ~SpaceShip();

  float timer = -1.f;
  int docked = -1;

  int encounter_type = -1;
  int encounter_mod = 0;

  std::vector<StationObject> m_objects;
};
