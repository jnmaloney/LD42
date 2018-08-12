#pragma once


#include "Mesh.h"
#include <vector>
#include <map>


class MeshManager
{
public:

  static MeshManager* get();

  MeshManager();
  ~MeshManager();

  void load();

  Mesh* getMesh(int index);

protected:

  std::vector<Mesh*> m_meshes;

};
