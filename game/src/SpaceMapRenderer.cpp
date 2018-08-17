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

    //shipOffset.x = -6.f * ship.timer;
    float t1 = ship.timer;
    float t2 = t1*t1;
    shipOffset.x = -3.f * t1 + 3.f * t2;
    shipOffset.y = -0.9f * t2;
    shipOffset.z = 0.125f * t2;
  }

  // The launching ship
  if (ship.docked == 1)
  {
    if (ship.timer <= 1.f)
    {
      ship.timer += 1.f / 60.f;

      // Is far?
      // if (ship.timer > 1.f)
      // {
      //   ship.timer = 1.f;
      //   //m_station->m_launchingShip.docked = -1;
      // }

      //shipOffset.x = -6.f * ship.timer;
      float t1 = ship.timer;
      float t2 = t1*t1;
      shipOffset.x = -3.f * t1 - 3.f * t2;
      shipOffset.y = -0.9f * t2;
      shipOffset.z = 0.125f * t2;
    }
  }

  if (ship.timer < 1.f)
  {
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
      m_renderQueue.setDiffuse(glm::vec3(0.6f, 0.6f, 0.65f));
      m_renderQueue.setMVP(local);
    }

    if (ship.encounter_type == 3) // Empire 2
    {
      glm::mat4 local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
      m_renderQueue.setMesh(15);
      m_renderQueue.setDiffuse(glm::vec3(0.55f, 0.55f, 0.55f));
      m_renderQueue.setMVP(local);
    }

    if (ship.encounter_type == 4) // Repair
    {
      glm::mat4 local = glm::translate(glm::mat4(1.f), shipOffset + glm::vec3(0.f, -2.f, -0.04f));
      m_renderQueue.setMesh(16);
      m_renderQueue.setDiffuse(glm::vec3(1.f, 0.98f, 0.88f));
      m_renderQueue.setMVP(local);
    }
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

  // The launching ship
  glm::vec3 shipOffset2(0, 0, 0);
  renderShip(m_station->m_launchingShip, shipOffset2);

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
    StationObject& o = m_am->m_objects[i];
    m_renderQueue.setMesh(4 + o.object_type);
    //if (m_am->m_holding && m_am->m_holdItem == m_am->m_objects[i].id) continue;

    glm::vec3 translate;
    m_am->pointFromIndex(0, o.i, o.j, translate);

    //glm::vec3 offset(0,0,0);// = m_am->offsets[m_am->m_objects[i].object_type][m_am->m_objects[i].rotation];
    translate.x += -0.125f + 0.5f * 0.25f * m_am->m_footprints[o.object_type].getWidth(o.rotation);
    translate.y += -0.125f + 0.5f * 0.25f * m_am->m_footprints[o.object_type].getHeight(o.rotation);
    local = glm::translate(glm::mat4(1.f), translate);
    //local = glm::translate(local, offset);
    local = glm::rotate(local,
      1.57079632679f * o.rotation,
      glm::vec3(0, 0, 1));
    //m_renderQueue.setDiffuse(glm::vec3(0.9f, 1.0f, 0.9f));
    m_renderQueue.setDiffuse(diffuseOf(o.colour));
    m_renderQueue.setMVP(local);
  }

  // Boxes (Ship)
  //std::cout << m_station->m_dockedShip.m_objects.size() << std::endl;
  for (int i = 0; i < m_station->m_dockedShip.m_objects.size(); ++i)
  {
    StationObject& o = m_station->m_dockedShip.m_objects[i];

    m_renderQueue.setMesh(4 + m_station->m_dockedShip.m_objects[i].object_type);
    //if (m_am->m_holding && m_am->m_holdItem == m_station->m_dockedShip.m_objects[i].id) continue;

    glm::vec3 translate;
    m_am->pointFromIndex(1, m_station->m_dockedShip.m_objects[i].i, m_station->m_dockedShip.m_objects[i].j, translate);
    translate = translate + shipOffset;
    translate.x += -0.125f + 0.5f * 0.25f * m_am->m_footprints[o.object_type].getWidth(o.rotation);
    translate.y += -0.125f + 0.5f * 0.25f * m_am->m_footprints[o.object_type].getHeight(o.rotation);

    //translate += m_am->offsets[m_station->m_dockedShip.m_objects[i].object_type];
    //glm::vec3 offset = m_am->offsets[m_station->m_dockedShip.m_objects[i].object_type][m_station->m_dockedShip.m_objects[i].rotation];
    //translate += offset;

    local = glm::translate(glm::mat4(1.f), translate);
    local = glm::rotate(local,
      1.57079632679f * m_station->m_dockedShip.m_objects[i].rotation,
      glm::vec3(0, 0, 1));
    //m_renderQueue.setDiffuse(glm::vec3(0.9f, 1.0f, 0.9f));
    m_renderQueue.setDiffuse(diffuseOf(m_station->m_dockedShip.m_objects[i].colour));
    m_renderQueue.setMVP(local);
  }

  // Boxes (Ship Launching)
  if (m_station->m_launchingShip.timer < 1.f)
  {
    for (int i = 0; i < m_station->m_launchingShip.m_objects.size(); ++i)
    {
      StationObject& o =m_station->m_launchingShip.m_objects[i];

      m_renderQueue.setMesh(4 + m_station->m_launchingShip.m_objects[i].object_type);

      glm::vec3 translate;
      m_am->pointFromIndex(1, m_station->m_launchingShip.m_objects[i].i, m_station->m_launchingShip.m_objects[i].j, translate);
      translate = translate + shipOffset2;
      translate.x += -0.125f + 0.5f * 0.25f * m_am->m_footprints[o.object_type].getWidth(o.rotation);
      translate.y += -0.125f + 0.5f * 0.25f * m_am->m_footprints[o.object_type].getHeight(o.rotation);

      //glm::vec3 offset = m_am->offsets[m_station->m_launchingShip.m_objects[i].object_type][m_station->m_launchingShip.m_objects[i].rotation];
      //translate += offset;

      local = glm::translate(glm::mat4(1.f), translate);
      local = glm::rotate(local,
        1.57079632679f * m_station->m_launchingShip.m_objects[i].rotation,
        glm::vec3(0, 0, 1));
      m_renderQueue.setDiffuse(diffuseOf(m_station->m_launchingShip.m_objects[i].colour));
      m_renderQueue.setMVP(local);
    }
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
    if (m_am->indexFromPoint(place, i, j, m_rs->m_cursorX - 0.125f, m_rs->m_cursorY - 0.125f)) // counter std::round ?
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



  // Held object footprint
  if (m_am->m_holding)
  {
    m_am->move_mouse();
    if (m_am->cursor_place != -1)
    {
      int i0 = m_am->cursor_i;
      int j0 = m_am->cursor_j;
      int place = m_am->cursor_place;

      glm::vec3 translate;

      // i0 += m_am->square_offsets[m_am->m_heldObject.object_type][m_am->m_heldObject.rotation].x;
      // j0 += m_am->square_offsets[m_am->m_heldObject.object_type][m_am->m_heldObject.rotation].y;
      StationObject& o = m_am->m_heldObject;

      Footprint& footprint =  m_am->m_footprints[o.object_type];

      for (int i = 0; i < footprint.getWidth(o.rotation); ++i)
      {
        for (int j = 0; j < footprint.getHeight(o.rotation); ++j)
        {
          if (footprint.getPoint(i, j, o.rotation))
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
}
