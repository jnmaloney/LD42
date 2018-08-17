#include "ActionManager.h"
#include <stdio.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <random>


ActionManager::ActionManager()
{
  memset(m_stationArray, 0, 15*15*sizeof(int));
  memset(m_shipArray, 0, 15*15*sizeof(int));

  m_footprints.push_back(Footprint(0));
  m_footprints.push_back(Footprint(1));
  m_footprints.push_back(Footprint(2));
  m_footprints.push_back(Footprint(3));
  m_footprints.push_back(Footprint(4));
  m_footprints.push_back(Footprint(5));
  m_footprints.push_back(Footprint(6));

  // for (int i = 0; i < m_footprints[1].getWidth(); ++i)
  // {
  //   for (int j = 0; j < m_footprints[1].getHeight(); ++j)
  //   {
  //     std::cout << m_footprints[1].getPoint(i, j);
  //   }
  //   std::cout << std::endl;
  // }

  doMask();
}


void ActionManager::loadShip()
{
  static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator(seed);
  static std::uniform_int_distribution<int> distribution(1, 600);
  static auto dice = std::bind ( distribution, generator );

  memset(m_shipArray, 0, 15*15*sizeof(int));

  std::vector<StationObject>& objectsOnShip = m_station->m_dockedShip.m_objects;

  int costs[] = {
    8,
    24,
    8,
    8,
    12,
    10,
    2,
  };

  int sells[] = {
    16,
    76,
    16,
    16,
    30,
    23,
    3
  };

  objectsOnShip.resize(0);
  for (int i = 0; i < 180; ++i)
  {
    StationObject o;
    o.id = ObjectIndexMaker::getNewIndex();
    o.object_type = dice() % 6;
    o.rotation = dice() % 4;
    o.colour = dice() % 24;
    o.cost = costs[o.object_type];
    o.sell_price = sells[o.object_type];

    if (m_station->m_dockedShip.encounter_mod == o.object_type) continue;

    int x = dice() % 8;
    int y = dice() % 4;

    if (canPlace(1, x, y, o))
    {
      placeItem(1, x, y, o);
      objectsOnShip.push_back(o);
    }
  }

  for (int i = 0; i < 8; ++i)
  {
    StationObject o;
    o.id = ObjectIndexMaker::getNewIndex();
    o.object_type = dice() % 7;
    o.rotation = dice() % 4;
    o.colour = dice() % 24;
    o.cost = costs[o.object_type];
    o.sell_price = sells[o.object_type];

    if (m_station->m_dockedShip.encounter_mod == o.object_type) continue;

    int x = dice() % 8;
    int y = dice() % 4;

    if (canPlace(1, x, y, o))
    {
      placeItem(1, x, y, o);
      objectsOnShip.push_back(o);
    }
  }

  //std::cout << m_station->m_dockedShip.m_objects.size() << std::endl;
}


void ActionManager::loadShipEmpire()
{
  static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator(seed);
  static std::uniform_int_distribution<int> distribution(1, 600);
  static auto dice = std::bind ( distribution, generator );

  memset(m_shipArray, 0, 15*15*sizeof(int));

  std::vector<StationObject>& objectsOnShip = m_station->m_dockedShip.m_objects;

  int sells[] = {
    16,
    76,
    16,
    16,
    30,
    23,
    3
  };

  objectsOnShip.resize(0);
  for (int i = 0; i < 8; ++i)
  {
    StationObject o;
    o.id = ObjectIndexMaker::getNewIndex();
    o.object_type = dice() % 6;
    o.rotation = dice() % 4;
    o.colour = 125;
    o.cost = 0;
    o.sell_price = sells[o.object_type];

    int x = dice() % 8;
    int y = dice() % 4;

    if (canPlace(1, x, y, o))
    {
      placeItem(1, x, y, o);
      objectsOnShip.push_back(o);
    }
  }
}


void ActionManager::asteroid()
{
  static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator(seed);
  static std::uniform_int_distribution<int> distribution(1, 3);
  static auto dice = std::bind ( distribution, generator );

  int i = dice();

  if (i == 1 && m_crashed1) ++i;
  if (i == 2 && m_crashed2) ++i;
  if (i == 3 && m_crashed3) i = 1;
  if (i == 1 && m_crashed1) ++i;
  if (i == 2 && m_crashed2) ++i;
  if (i == 3 && m_crashed3) gameOver = true;

  m_station->m_dockedShip.encounter_mod = i;
}


void ActionManager::asteroid_strike()
{
  int i = m_station->m_dockedShip.encounter_mod;

  if (i == 1) m_crashed1 = true;
  if (i == 2) m_crashed2 = true;
  if (i == 3) m_crashed3 = true;
  doMask();
}


ActionManager::~ActionManager()
{

}


void ActionManager::cancel()
{
  // Return home
  if (m_holding)
  {
    m_heldObject.rotation = m_return_rotation;
    if (m_returnToStation)
    {
      placeItem(0, m_return_i, m_return_j, m_heldObject);
      m_objects.push_back(m_heldObject);
      //std::cout << " -> Station"<< std::endl;
    }
    else if (m_returnToShip)
    {
      placeItem(1, m_return_i, m_return_j, m_heldObject);
      m_station->m_dockedShip.m_objects.push_back(m_heldObject);
      //std::cout << " -> Ship"<< std::endl;
    }
    m_holding = false;
  }
}


void ActionManager::move_mouse()
{
  if (m_holding)
  {
    // Offset the cursor based on the footprint
    float dx = -0.5f * 0.25f * m_footprints[m_heldObject.object_type].getWidth(m_heldObject.rotation);// + 0.125f;
    float dy = -0.5f * 0.25f * m_footprints[m_heldObject.object_type].getHeight(m_heldObject.rotation);// + 0.125f;

    // Get values for Place and Array Index from cursor xy
    indexFromPoint(cursor_place, cursor_i, cursor_j, m_rs->m_cursorX + dx, m_rs->m_cursorY + dy);
  }
}


void ActionManager::action()
{
  bool onShip = false;
  bool onStation = false;

  //std::cout << "Action" << i << " "<< j << std::endl;
  //std::cout << "Action " << m_rs->m_cursorX << " " << m_rs->m_cursorY << std::endl;

  std::vector<StationObject>& objectsOnShip = m_station->m_dockedShip.m_objects;

  // Put down anywhere
  if (m_holding)
  {
    move_mouse();
    if (cursor_place == -1) return;

    onStation = (cursor_place == 0);
    onShip = (cursor_place == 1);

    // First.. can it fit here
    if (canPlace(cursor_place, cursor_i, cursor_j, m_heldObject))
    {
      // Now... do we allow it
      bool buying = (cursor_place == 0 && m_returnToShip);
      bool selling = (cursor_place == 1 && m_returnToStation);
      bool canSell = m_station->m_dockedShip.encounter_type == 0 && m_heldObject.cost && m_heldObject.object_type == m_station->m_dockedShip.encounter_mod;
      bool canBuy = (m_station->m_dockedShip.encounter_type == 0 && m_station->m_clams >= m_heldObject.cost && m_heldObject.object_type != m_station->m_dockedShip.encounter_mod) || // && (m_heldObject.cost==0 || m_heldObject.object_type != m_station->m_dockedShip.encounter_mod))
                    (m_station->m_dockedShip.encounter_type == 1);
      if ((cursor_place == 1 && m_returnToShip) ||   // stay on ship
          (cursor_place == 0 && m_returnToStation) || // stay on platform
          (buying && canBuy) || // purchase
          (selling && canSell)) // sell
      {
        // Buy from ship ?
        if (buying)
        {
          m_station->m_clams -= m_heldObject.cost;
        }

        // Sell to ship
        if (selling)
        {
          m_station->m_clams += m_heldObject.sell_price;
        }

        // Place it
        placeItem(cursor_place, cursor_i, cursor_j, m_heldObject);
        m_holding = false;

        // move lists
        if (cursor_place == 0) // Station -> Ship
        {
          m_objects.push_back(m_heldObject);
          //std::cout << " -> Station"<< std::endl;
        }
        if (cursor_place == 1) // Ship -> Station
        {
          objectsOnShip.push_back(m_heldObject);
          //std::cout << " -> Ship"<< std::endl;
        }
      }
    }
    // doesn't fit
    else
    {

    }
  }
  else
  {
    // Pick up ?

    // Get values for Place and Array Index from cursor xy
    int i = 0;
    int j = 0;
    int place = 0;
    indexFromPoint(place, i, j, m_rs->m_cursorX - 0.125, m_rs->m_cursorY - 0.125); // counter std::round
    onStation = (place == 0);
    onShip = (place == 1);
    if (place == -1) return;

    // Check the  array
    int a = onStation ? m_stationArray[i][j] : m_shipArray[i][j];
    if (a < 100) a = 0;
    if (a)
    {
      // Pick up
      m_holding = true;
      m_holdItem = a;
      m_heldObject = nullObject;
      for (int i = 0; i < m_objects.size(); ++i)
      {
        if (m_objects[i].id == m_holdItem)
        {
          m_heldObject = m_objects[i];
          m_objects.erase(m_objects.begin() + i);
          //std::cout << "Station ->"<< std::endl;
          break;
        }
      }
      //if (m_heldObject == nullObject)
      for (int i = 0; i < objectsOnShip.size(); ++i)
      {
        if (objectsOnShip[i].id == m_holdItem)
        {
          m_heldObject = objectsOnShip[i];
          objectsOnShip.erase(objectsOnShip.begin() + i);
          //std::cout << "Ship ->"<< std::endl;
          break;
        }
      }

      clearItem(onStation ? 0 : 1, m_heldObject);
      m_returnToStation = onStation;
      m_returnToShip = onShip;
      m_return_i = m_heldObject.i;
      m_return_j = m_heldObject.j;
      m_return_rotation = m_heldObject.rotation;
    }
  }
}


bool ActionManager::canPlace(int place, int i0, int j0, StationObject& o)
{
  Footprint& footprint = m_footprints[o.object_type];

  int(& array)[15][15] = ( place == 0 ) ? m_stationArray : m_shipArray;

  int w, h;
  if (place == 0)
  {
    w = 15;
    h = 7;
  }
  else
  {
    w = 8;
    h = 4;
  }

  // Test footprint
  for (int i = 0; i < footprint.getWidth(o.rotation); ++i)
  {
    for (int j = 0; j < footprint.getHeight(o.rotation); ++j)
    {
      if (footprint.getPoint(i, j, o.rotation))
      {
        if (i0+i < 0) return false;
        if (j0+j < 0) return false;
        if (i0+i >= w) return false;
        if (j0+j >= h) return false;
        if (array[i0+i][j0+j]) return false;
      }
    }
  }
  return true;
}


void ActionManager::placeItem(int place, int i0, int j0, StationObject& o)
{
  o.i = i0;
  o.j = j0;

  Footprint& footprint = m_footprints[o.object_type];

  int(& array)[15][15] = ( place == 0 ) ? m_stationArray : m_shipArray;

  for (int i = 0; i < footprint.getWidth(o.rotation); ++i)
  {
    for (int j = 0; j < footprint.getHeight(o.rotation); ++j)
    {
      if (footprint.getPoint(i, j, o.rotation))
      {
        array[i0+i][j0+j] = o.id;
      }
    }
  }
}


void ActionManager::clearItem(int place, int i, int j)
{
  if (place == 0)
  {
    m_stationArray[i][j] = 0;
    m_stationArray[i+1][j] = 0;
    m_stationArray[i][j+1] = 0;
    m_stationArray[i+1][j+1] = 0;
  }
  if (place == 1)
  {
    m_shipArray[i][j] = 0;
    m_shipArray[i+1][j] = 0;
    m_shipArray[i][j+1] = 0;
    m_shipArray[i+1][j+1] = 0;
  }
}


void ActionManager::clearItem(int place, StationObject& o)
{
  Footprint& footprint = m_footprints[o.object_type];

  int(& array)[15][15] = ( place == 0 ) ? m_stationArray : m_shipArray;

  for (int i = 0; i < footprint.getWidth(o.rotation); ++i)
  {
    for (int j = 0; j < footprint.getHeight(o.rotation); ++j)
    {
      if (footprint.getPoint(i, j, o.rotation))
      {
          array[o.i+i][o.j+j] = 0;
      }
    }
  }
}


bool ActionManager::indexFromPoint(int& place, int& i, int& j, float x, float y)
{
  float x0;
  float y0;
  int w;
  int h;
  float s = 0.25f; // Grid side length
  float s_inv = 4.f;

  for (int k = 0; k <= 1; ++k)
  {
    // CO-ordinate
    if (k == 0) // Station
    {
      x0 = -1.88f;
      y0 = -0.88f;
      w = 15;
      h = 7;
    }
    else if (k == 1) // Ship
    {
      x0 = -1.f;
      y0 = -2.5f;
      w = 8;
      h = 4;
    }

    // Check grid place
    if (x > x0 - 0.125f &&
        x < x0+s*(float)w - 0.125f &&
        y > y0 - 0.125f
        && y < y0+s*(float)h - 0.125f)
    {
      i = std::round((x - x0) * s_inv);
      j = std::round((y - y0) * s_inv);
      place = k;
      if (i > w) i = w;
      if (j > h) j = h;
      if (i < 0) i = 0;
      if (j < 0) j = 0;
      //std::cout << " i " << i << " j " << j << std::endl;
      return true;
    }
  }

  place = -1;
  return false;
}


void ActionManager::pointFromIndex(int place, int i, int j, glm::vec3& translation)
{
  float x0 = 0.f;
  float y0 = 0.f;
  float z0 = 0.f;
  float s = 0.25f;

  if (place == 0) // Station
  {
    x0 = -1.88f;
    y0 = -0.88f;
    z0 =  0.f;
  }
  else if (place == 1) // Ship
  {
    x0 = -1.f;
    y0 = -2.5f;
    z0 = -0.03f;
  }

  translation = glm::vec3(
    x0 + s*((float)i+0.5f),
    y0 + s*((float)j+0.5f),
    z0);
}


void ActionManager::placeArray(std::vector<StationObject>& array, int i, int j, int pattern, int x)
{

}


float ActionManager::getFootprint(int i, int j, StationObject& o)
{
  // glm::mat4 footprint = footprints[o.object_type];
  // const float* pFootprint = (const float*)glm::value_ptr(footprint);

  float x = 0.f;
  // if (o.rotation == 0) x = pFootprint[i*4 + j];
  // if (o.rotation == 1) x = pFootprint[(j)*4 + (3-i)];
  // if (o.rotation == 2) x = pFootprint[(3-i)*4 + (3-j)];
  // if (o.rotation == 3) x = pFootprint[(3-j)*4 + (i)];
  return x;
}


void ActionManager::doMask()
{
  //memset(m_stationArray, 0, 15*15*sizeof(int));

  for (int i = 0; i < 15; ++i)
  {
    for (int j = 0; j < 7; ++j)
    {
      if (m_stationArray[i][j] == 1) m_stationArray[i][j] = 0;

      if ((m_crashed1 && m_crash1Mask[i][j]) ||
        (m_crashed2 && m_crash2Mask[i][j]) ||
        (m_crashed3 && m_crash3Mask[i][j]))
      {
        if (m_stationArray[i][j]) crushItem(m_stationArray[i][j]);
        m_stationArray[i][j] = 1;
      }
    }
  }

  m_stationArray[0][0] = 1;
  m_stationArray[0][1] = 1;
  m_stationArray[1][0] = 1;
  m_stationArray[1][1] = 1;
}


void ActionManager::crushItem(int id)
{
  for (int i = 0; i < m_objects.size(); ++i)
  {
    if (m_objects[i].id == id)
    {
      //std::cout << id << std::endl;
      clearItem(0, m_objects[i]);
      m_objects.erase(m_objects.begin() + i);
      return;
    }
  }
}


void ActionManager::empire_collect()
{
  for (int i = 0; i < m_objects.size(); ++i)
  {
    if (m_objects[i].colour > 25)
    {
      m_station->m_clams += m_objects[i].sell_price;
      crushItem(m_objects[i].id);
      --i;
    }
  }
}
