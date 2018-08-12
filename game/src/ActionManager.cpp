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

  m_stationArray[0][0] = 1;
  m_stationArray[0][1] = 1;
  m_stationArray[1][0] = 1;
  m_stationArray[1][1] = 1;

  // setup
  footprints.resize(7);
  offsets.resize(7);
  //square_offsets.resize(6);

  float aaa[16] = {
     0, 0, 0, 0,
     0, 1, 1, 0,
     0, 1, 1, 0,
     0, 0, 0, 0
  };
  float bbb[16] = {
     1, 0, 0, 1,
     1, 1, 1, 1,
     1, 1, 1, 1,
     1, 0, 0, 1
  };
  float ccc[16] = {
     0, 1, 0, 0,
     0, 1, 0, 0,
     0, 1, 0, 0,
     0, 1, 0, 0
  };
  float ddd[16] = {
     0, 0, 1, 0,
     0, 1, 1, 0,
     0, 1, 0, 0,
     0, 0, 0, 0
  };
  float eee[16] = {
     0, 1, 1, 0,
     0, 1, 0, 0,
     0, 1, 0, 0,
     0, 1, 1, 0
  };
  float fff[16] = {
     0, 1, 0, 0,
     0, 1, 0, 0,
     0, 1, 1, 0,
     0, 1, 0, 0
  };
  float ggg[16] = {
     0, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 0, 0,
     0, 0, 0, 0
  };
  footprints[0] = glm::make_mat4(aaa);
  footprints[1] = glm::make_mat4(bbb);
  footprints[2] = glm::make_mat4(ccc);
  footprints[3] = glm::make_mat4(ddd);
  footprints[4] = glm::make_mat4(eee);
  footprints[5] = glm::make_mat4(fff);
  footprints[6] = glm::make_mat4(ggg);

  offsets[0].push_back(glm::vec3(0.375f, 0.375f, 0.f));
  offsets[0].push_back(glm::vec3(0.375f, 0.375f, 0.f));
  offsets[0].push_back(glm::vec3(0.375f, 0.375f, 0.f));
  offsets[0].push_back(glm::vec3(0.375f, 0.375f, 0.f));

  offsets[1].push_back(glm::vec3(0.375f, 0.375f, 0.f));
  offsets[1].push_back(glm::vec3(0.375f, 0.375f, 0.f));
  offsets[1].push_back(glm::vec3(0.375f, 0.375f, 0.f));
  offsets[1].push_back(glm::vec3(0.375f, 0.375f, 0.f));

  offsets[2].push_back(glm::vec3(0.375f, 0.25f, 0.f));
  offsets[2].push_back(glm::vec3(0.5f, 0.375f, 0.f));
  offsets[2].push_back(glm::vec3(0.375f, 0.5f, 0.f));
  offsets[2].push_back(glm::vec3(0.25f, 0.375f, 0.f));

  offsets[3].push_back(glm::vec3(0.45f, 0.25f, 0.f));
  offsets[3].push_back(glm::vec3(0.5f,  0.5f, 0.f));
  offsets[3].push_back(glm::vec3(0.2f, 0.5f, 0.f));
  offsets[3].push_back(glm::vec3(0.25f, 0.375f, 0.f));

  offsets[4].push_back(glm::vec3(0.375f, 0.25f, 0.f));
  offsets[4].push_back(glm::vec3(0.5f, 0.375f, 0.f));
  offsets[4].push_back(glm::vec3(0.375f, 0.5f, 0.f));
  offsets[4].push_back(glm::vec3(0.25f, 0.375f, 0.f));

  offsets[5].push_back(glm::vec3(0.375f, 0.25f, 0.f));
  offsets[5].push_back(glm::vec3(0.5f, 0.375f, 0.f));
  offsets[5].push_back(glm::vec3(0.375f, 0.5f, 0.f));
  offsets[5].push_back(glm::vec3(0.25f, 0.375f, 0.f));

  offsets[6].push_back(glm::vec3(0.f, 0.f, 0.f));
  offsets[6].push_back(glm::vec3(0.f, 0.f, 0.f));
  offsets[6].push_back(glm::vec3(0.f, 0.f, 0.f));
  offsets[6].push_back(glm::vec3(0.f, 0.f, 0.f));

}


void ActionManager::loadShip()
{
  static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator(seed);
  static std::uniform_int_distribution<int> distribution(1, 600);
  static auto dice = std::bind ( distribution, generator );

  memset(m_shipArray, 0, 15*15*sizeof(int));

  std::vector<StationObject>& objectsOnShip = m_station->m_dockedShip.m_objects;

  objectsOnShip.resize(0);
  for (int i = 0; i < 180; ++i)
  {
    StationObject o;
    o.id = ObjectIndexMaker::getNewIndex();
    o.object_type = dice() % 7;
    o.rotation = dice() % 4;
    o.colour = dice() % 24;

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


ActionManager::~ActionManager()
{

}


void ActionManager::action()
{
  int i = 0;
  int j = 0;
  bool onShip = false;
  bool onStation = false;

  // Get values for Place and Array Index from cursor xy
  int place = 0;
  indexFromPoint(place, i, j, m_rs->m_cursorX, m_rs->m_cursorY);
  onStation = (place == 0);
  onShip = (place == 1);

  //std::cout << "Action" << i << " "<< j << std::endl;
  //std::cout << "Action " << m_rs->m_cursorX << " " << m_rs->m_cursorY << std::endl;


  std::vector<StationObject>& objectsOnShip = m_station->m_dockedShip.m_objects;

  // Put down anywhere
  if (m_holding)
  {
    // First.. can it fit here
    if (canPlace(place, i-2, j-2, m_heldObject))
    {
      // Place it
      placeItem(place, i-2, j-2, m_heldObject);
      m_holding = false;

      //std::cout << " i " << m_heldObject.i << " j " << m_heldObject.j << std::endl;

      // move lists
      if (place == 0) // Station -> Ship
      {
        m_objects.push_back(m_heldObject);
        std::cout << " -> Station"<< std::endl;
      }
      if (place == 1) // Ship -> Station
      {
        objectsOnShip.push_back(m_heldObject);
        std::cout << " -> Ship"<< std::endl;
      }
    }
    // doesn't fit
    else
    {
      // Return home
      m_heldObject.rotation = m_return_rotation;
      if (m_returnToStation)
      {
        placeItem(0, m_return_i, m_return_j, m_heldObject);
        m_objects.push_back(m_heldObject);
        std::cout << " -> Station"<< std::endl;
      }
      else if (m_returnToShip)
      {
        placeItem(1, m_return_i, m_return_j, m_heldObject);
        objectsOnShip.push_back(m_heldObject);
        std::cout << " -> Ship"<< std::endl;
      }
      m_holding = false;
    }



  }
  else
  {
    // Pick up ?

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
          std::cout << "Station ->"<< std::endl;
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
          std::cout << "Ship ->"<< std::endl;
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
  glm::mat4 footprint = footprints[o.object_type];

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

  const float* pFootprint = (const float*)glm::value_ptr(footprint);
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      float x = getFootprint(i, j, o);
      if (x > 0.5f)
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

  glm::mat4 footprint = footprints[o.object_type];

  int(& array)[15][15] = ( place == 0 ) ? m_stationArray : m_shipArray;

  const float* pFootprint = (const float*)glm::value_ptr(footprint);
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      float x = getFootprint(i, j, o);
      if (x > 0.5f)
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
  glm::mat4 footprint = footprints[o.object_type];

  int(& array)[15][15] = ( place == 0 ) ? m_stationArray : m_shipArray;

  const float* pFootprint = (const float*)glm::value_ptr(footprint);
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      float x = getFootprint(i, j, o);
      if (x > 0.5f)
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
  float s = 0.25f;
  float s_inv = 4.f;

  for (int k = 0; k <= 1; ++k)
  {
    if (k == 0) // Station
    {
      x0 = -1.88f;
      y0 = -0.88f;
      w = 15;
      h = 7;
    }
    else // Ship
    {
      x0 = -1.f;
      y0 =  -2.5f;
      w = 8;
      h = 4;
    }

    if (x > x0 && x < x0+s*(float)w && y > y0 && y < y0+s*(float)h)
    {
      i = (int)((x - x0) * s_inv);
      j = (int)((y - y0) * s_inv);
      place = k;
      if (i > w) i = w;
      if (j > h) j = h;
      if (i < 0) i = 0;
      if (j < 0) j = 0;
      //std::cout << " i " << i << " j " << j << std::endl;
      return true;
    }
  }

  return false;
}


void ActionManager::pointFromIndex(int place, int i, int j, glm::vec3& translation)
{
  float x0;
  float y0;
  float z0;
  float s = 0.25f;

  if (place == 0) // Station
  {
    x0 = -1.88f;
    y0 = -0.88f;
    z0 =  0.f;
  }
  else // Ship
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
  glm::mat4 footprint = footprints[o.object_type];
  const float* pFootprint = (const float*)glm::value_ptr(footprint);

  float x = 0.f;
  if (o.rotation == 0) x = pFootprint[i*4 + j];
  if (o.rotation == 1) x = pFootprint[(j)*4 + (3-i)];
  if (o.rotation == 2) x = pFootprint[(3-i)*4 + (3-j)];
  if (o.rotation == 3) x = pFootprint[(3-j)*4 + (i)];
  return x;
}
