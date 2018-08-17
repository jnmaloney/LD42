#include "SpaceItems.h"


int ObjectIndexMaker::index = 100;


Footprint::Footprint(int id)
{
  if (id == 0) // Box
  {
    width = 2;
    height = 2;
    array =
    {1, 1,
     1, 1};
  }
  if (id == 1) // Big H
  {
    width = 4;
    height = 4;
    array =
    {1, 1, 1, 1,
     0, 1, 1, 0,
     0, 1, 1, 0,
     1, 1, 1, 1};
  }
  if (id == 2) // beam
  {
    width = 4;
    height = 1;
    array =
    {1, 1, 1, 1};
  }
  if (id == 3) // squiggly
  {
    width = 3;
    height = 2;
    array =
    {0, 1, 1,
     1, 1, 0};
  }
  if (id == 4) // Big C
  {
    width = 4;
    height = 2;
    array =
    {1, 1, 1, 1,
     1, 0, 0, 1 };
  }
  if (id == 5) // Knob
  {
    width = 4;
    height = 2;
    array =
    {1, 1, 1, 1,
     0, 0, 1, 0 };
  }
  if (id == 6) // Bucket
  {
    width = 1;
    height = 1;
    array =
    {1};
  }
  if (id == 8)
  {

  }
}


Footprint::~Footprint()
{

}


int Footprint::getHeight(int rotation)
{
  if (rotation == 0 || rotation == 2) return height;
  else return width;
}


int Footprint::getWidth(int rotation)
{
  if (rotation == 0 || rotation == 2) return width;
  else return height;
}


int Footprint::getPoint(int i, int j, int rotation)
{
  int i1 = i, j1 = j;
  if (rotation == 1) { i1 = j;              j1 = height - 1 - i; }
  if (rotation == 2) { i1 = width - 1 - i;  j1 = height - 1 - j; }
  if (rotation == 3) { i1 = width - 1 - j;  j1 = i;              }
  return array[j1 * width + i1];
}




SpaceShip::SpaceShip()
{

}


SpaceShip::~SpaceShip()
{

}
