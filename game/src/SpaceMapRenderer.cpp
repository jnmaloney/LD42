#include "SpaceMapRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <iostream>


SpaceMapRenderer::SpaceMapRenderer()
{


}


SpaceMapRenderer::~SpaceMapRenderer()
{

}


void SpaceMapRenderer::renderShip(SpaceShip& ship, glm::vec3& shipOffset)
{
  if (ship.encounter_type == -1) return;
  if (ship.encounter_type == 2) return;

  if (ship.docked == -1)
  {
    ship.timer += 1.f / 60.f;

    // Is docked?
    if (ship.timer > 0.f)
    {
      ship.timer = 0.f;
      ship.docked = 0;
    }

    shipOffset.x = -6.f * ship.timer;
  }

  // The launching ship
  if (ship.docked == 1)
  {
    ship.timer += 1.f / 60.f;

    // Is far?
    if (ship.timer > 1.f)
    {
      ship.timer = 1.f;
      //m_station->m_launchingShip.docked = -1;
    }

    shipOffset.x = -6.f * ship.timer;
  }

  if (ship.encounter_type == 0) // Trade
  {
    glm::mat4 local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
    m_renderQueue.setMesh(2);
    m_renderQueue.setMVP(local);
  }

  if (ship.encounter_type == 1) // Empire 1
  {
    glm::mat4 local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
    m_renderQueue.setMesh(14);
    m_renderQueue.setMVP(local);
  }

  if (ship.encounter_type == 3) // Empire 2
  {
    glm::mat4 local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
    m_renderQueue.setMesh(15);
    m_renderQueue.setMVP(local);
  }

  if (ship.encounter_type == 4) // Repair
  {
    glm::mat4 local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
    m_renderQueue.setMesh(16);
    m_renderQueue.setMVP(local);
  }

}


glm::vec3 diffuseOf(int i)
{
    if (i == 0) return glm::vec3(0.9f, 1.0f, 0.9f);
    if (i == 1) return glm::vec3(1.0f, 0.9f, 0.9f);
    if (i == 2) return glm::vec3(0.9f, 0.9f, 1.0f);
    if (i == 3) return glm::vec3(1.0f, 1.0f, 0.9f);
    if (i == 4) return glm::vec3(0.9f, 1.0f, 1.0f);
    if (i == 5) return glm::vec3(1.0f, 0.9f, 1.0f);
    if (i == 6) return glm::vec3(0.64f, 1.0f, 0.64f);
    if (i == 7) return glm::vec3(1.0f, 0.64f, 0.42f);
    if (i == 8) return glm::vec3(0.42f, 0.64f, 1.0f);

    if (i < 12) return glm::vec3(0.9f, 0.9f, 0.9f);
    if (i < 18) return glm::vec3(0.8f, 0.8f, 0.8f);
    if (i < 25) return glm::vec3(0.95f, 0.95f, 0.95f);

    return glm::vec3(0.38f, 0.38f, 0.38f);
}


void SpaceMapRenderer::tick()
{
  glm::mat4 local(1.f);

  m_renderQueue.clear();
  m_renderQueue.setProgram(m_rs->programObject);

  // Background meshes
  m_renderQueue.setMesh(1);
  m_renderQueue.setMVP(local);

  // Station
  if (!m_am->gameOver)
  {
    m_renderQueue.setMesh(0);
    m_renderQueue.setMVP(local);
  }
  if (!m_am->m_crashed1)
  {
    m_renderQueue.setMesh(11);
    m_renderQueue.setMVP(local);
  }
  if (!m_am->m_crashed2)
  {
    m_renderQueue.setMesh(12);
    m_renderQueue.setMVP(local);
  }
  if (!m_am->m_crashed3)
  {
    m_renderQueue.setMesh(13);
    m_renderQueue.setMVP(local);
  }

  // The docking ship
  glm::vec3 shipOffset(0, 0, 0);
  renderShip(m_station->m_dockedShip, shipOffset);

  // local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
  // m_renderQueue.setMesh(2);
  // m_renderQueue.setMVP(local);

  // The launching ship
  glm::vec3 shipOffset2(0, 0, 0);
  renderShip(m_station->m_launchingShip, shipOffset2);

  // local = glm::translate(glm::mat4(1.f), shipOffset2 + glm::vec3(0.f, -2.f, -0.04f));
  // m_renderQueue.setMesh(2);
  // m_renderQueue.setMVP(local);

  // Squares (station)
  // m_renderQueue.setMesh(3);
  // for (int i = 0; i < 15; ++i)
  // {
  //   for (int j = 0; j < 15; ++j)
  //   {
  //     if (m_am->m_stationArray[i][j])
  //     {
  //       glm::vec3 translate;
  //       m_am->pointFromIndex(0, i, j, translate);
  //       local = glm::translate(glm::mat4(1.f), translate);
  //       local = glm::scale(local, glm::vec3(0.25f));
  //       m_renderQueue.setDiffuse(glm::vec3(0.9f, 1.0f, 0.9f));
  //       m_renderQueue.setMVP(local);
  //     }
  //   }
  // }

  // Squares (Ship)
  // m_renderQueue.setMesh(3);
  // for (int i = 0; i < 15; ++i)
  // {
  //   for (int j = 0; j < 15; ++j)
  //   {
  //     if (m_am->m_shipArray[i][j])
  //     {
  //       glm::vec3 translate;
  //       m_am->pointFromIndex(1, i, j, translate);
  //       translate = translate + shipOffset;
  //       local = glm::translate(glm::mat4(1.f), translate);
  //       local = glm::scale(local, glm::vec3(0.25f));
  //       m_renderQueue.setDiffuse(glm::vec3(0.9f, 1.0f, 0.9f));
  //       m_renderQueue.setMVP(local);
  //     }
  //   }
  // }

  // Squares (Asteroid Preview)
  if (m_station->m_dockedShip.encounter_type == 2)
  {
    m_renderQueue.setMesh(3);
    for (int i = 0; i < 15; ++i)
    {
      for (int j = 0; j < 7; ++j)
      {
        if ((m_station->m_dockedShip.encounter_mod == 1 && m_am->m_crash1Mask[i][j]) ||
          (m_station->m_dockedShip.encounter_mod == 2 && m_am->m_crash2Mask[i][j]) ||
          (m_station->m_dockedShip.encounter_mod == 3 && m_am->m_crash3Mask[i][j]))
        {
          glm::vec3 translate;
          m_am->pointFromIndex(0, i, j, translate);
          translate = translate + shipOffset;
          local = glm::translate(glm::mat4(1.f), translate);
          local = glm::scale(local, glm::vec3(0.25f));
          m_renderQueue.setDiffuse(glm::vec3(1.0f, 0.9f, 0.9f));
          m_renderQueue.setMVP(local);
        }
      }
    }
  }

  // Boxes (Station)
  for (int i = 0; i < m_am->m_objects.size(); ++i)
  {
    m_renderQueue.setMesh(4 + m_am->m_objects[i].object_type);
    //if (m_am->m_holding && m_am->m_holdItem == m_am->m_objects[i].id) continue;

    glm::vec3 translate;
    m_am->pointFromIndex(0, m_am->m_objects[i].i, m_am->m_objects[i].j, translate);

    glm::vec3 offset = m_am->offsets[m_am->m_objects[i].object_type][m_am->m_objects[i].rotation];
    local = glm::translate(glm::mat4(1.f), translate);
    local = glm::translate(local, offset);
    local = glm::rotate(local,
      1.57079632679f * m_am->m_objects[i].rotation,
      glm::vec3(0, 0, 1));
    //m_renderQueue.setDiffuse(glm::vec3(0.9f, 1.0f, 0.9f));
    m_renderQueue.setDiffuse(diffuseOf(m_am->m_objects[i].colour));
    m_renderQueue.setMVP(local);
  }

  // Boxes (Ship)
  //std::cout << m_station->m_dockedShip.m_objects.size() << std::endl;
  for (int i = 0; i < m_station->m_dockedShip.m_objects.size(); ++i)
  {
    m_renderQueue.setMesh(4 + m_station->m_dockedShip.m_objects[i].object_type);
    //if (m_am->m_holding && m_am->m_holdItem == m_station->m_dockedShip.m_objects[i].id) continue;

    glm::vec3 translate;
    m_am->pointFromIndex(1, m_station->m_dockedShip.m_objects[i].i, m_station->m_dockedShip.m_objects[i].j, translate);
    translate = translate + shipOffset;

    //translate += m_am->offsets[m_station->m_dockedShip.m_objects[i].object_type];
    glm::vec3 offset = m_am->offsets[m_station->m_dockedShip.m_objects[i].object_type][m_station->m_dockedShip.m_objects[i].rotation];
    translate += offset;

    local = glm::translate(glm::mat4(1.f), translate);
    local = glm::rotate(local,
      1.57079632679f * m_station->m_dockedShip.m_objects[i].rotation,
      glm::vec3(0, 0, 1));
    //m_renderQueue.setDiffuse(glm::vec3(0.9f, 1.0f, 0.9f));
    m_renderQueue.setDiffuse(diffuseOf(m_station->m_dockedShip.m_objects[i].colour));
    m_renderQueue.setMVP(local);
  }

  // Boxes (Ship Launching)
  for (int i = 0; i < m_station->m_launchingShip.m_objects.size(); ++i)
  {
    m_renderQueue.setMesh(4 + m_station->m_launchingShip.m_objects[i].object_type);

    glm::vec3 translate;
    m_am->pointFromIndex(1, m_station->m_launchingShip.m_objects[i].i, m_station->m_launchingShip.m_objects[i].j, translate);
    translate = translate + shipOffset2;

    glm::vec3 offset = m_am->offsets[m_station->m_launchingShip.m_objects[i].object_type][m_station->m_launchingShip.m_objects[i].rotation];
    translate += offset;

    local = glm::translate(glm::mat4(1.f), translate);
    local = glm::rotate(local,
      1.57079632679f * m_station->m_launchingShip.m_objects[i].rotation,
      glm::vec3(0, 0, 1));
    m_renderQueue.setDiffuse(diffuseOf(m_station->m_launchingShip.m_objects[i].colour));
    m_renderQueue.setMVP(local);
  }

  // Action

  if (m_am->m_holding)
  {
    m_renderQueue.setMesh(4 + m_am->m_heldObject.object_type);

    local = glm::translate(glm::mat4(1.f), glm::vec3(m_rs->m_cursorX, m_rs->m_cursorY, 0.f));
    local = glm::rotate(local,
      1.57079632679f * m_am->m_heldObject.rotation,
      glm::vec3(0, 0, 1));

    //local = glm::translate(glm::mat4(1.f), glm::vec3(m_rs->m_cursorX, m_rs->m_cursorY, 0.f));
    m_renderQueue.setDiffuse(diffuseOf( m_am->m_heldObject.colour));
    m_renderQueue.setMVP(local);
  }
  else
  {
    // Mouse
    m_renderQueue.setMesh(3);
    int place, i, j;
    if (m_am->indexFromPoint(place, i, j, m_rs->m_cursorX, m_rs->m_cursorY))
    {
      glm::vec3 translate;
      m_am->pointFromIndex(place, i, j, translate);
      translate.z += 0.001f;
      local = glm::translate(glm::mat4(1.f), translate);
      local = glm::scale(local, glm::vec3(0.25f));
      m_renderQueue.setDiffuse(glm::vec3(0.7f, 0.7f, 0.7f));
      m_renderQueue.setMVP(local);
    }
  }

  m_renderQueue.setMesh(3);

  int i0;
  int j0;
  int place;
  m_am->indexFromPoint(place, i0, j0, m_rs->m_cursorX, m_rs->m_cursorY);
  glm::vec3 translate;

  // Held object footprint
  if (m_am->m_holding)
  {
    i0 += m_am->square_offsets[m_am->m_heldObject.object_type][m_am->m_heldObject.rotation].x;
    j0 += m_am->square_offsets[m_am->m_heldObject.object_type][m_am->m_heldObject.rotation].y;

    glm::mat4 footprint = m_am->footprints[m_am->m_heldObject.object_type];

    const float* pFootprint = (const float*)glm::value_ptr(footprint);
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        float x = m_am->getFootprint(i, j, m_am->m_heldObject);
        if (x > 0.5f)
        {
          m_am->pointFromIndex(place, i0+i, j0+j, translate);
          local = glm::translate(glm::mat4(1.f), translate);
          local = glm::scale(local, glm::vec3(0.25f));

          if (place == 0)
          {
            if (i0+i < 0 || i0+i >= 15 || j+j0 < 0 || j+j0 >= 7)
              m_renderQueue.setDiffuse(glm::vec3(1.0f, 0.69f, 0.69f));
            else if (m_am->m_stationArray[i0+i][j0+j])
              m_renderQueue.setDiffuse(glm::vec3(1.0f, 0.69f, 0.69f));
            else
              m_renderQueue.setDiffuse(glm::vec3(0.69f, 1.0f, 0.69f));
          }
          else
          {
            if (i0+i < 0 || i0+i >= 8 || j+j0 < 0 || j+j0 >= 4)
              m_renderQueue.setDiffuse(glm::vec3(1.0f, 0.69f, 0.69f));
            else if (m_am->m_shipArray[i0+i][j0+j])
              m_renderQueue.setDiffuse(glm::vec3(1.0f, 0.69f, 0.69f));
            else
              m_renderQueue.setDiffuse(glm::vec3(0.69f, 1.0f, 0.69f));
          }

          m_renderQueue.setMVP(local);
        }
      }
    }
  }
}
