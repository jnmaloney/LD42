#include "MeshManager.h"
#include "Texture.h"


static MeshManager* s_singleton = 0;


MeshManager* MeshManager::get()
{
  return s_singleton;
}


MeshManager::MeshManager()
{
  s_singleton = this;
}


MeshManager::~MeshManager()
{

}


void MeshManager::load()
{
  std::vector<const char*> mesh_files;
  mesh_files.push_back("data/obj/Station0.obj");
  mesh_files.push_back("data/obj/SpaceBox.obj");
  mesh_files.push_back("data/obj/Ship0.obj");
  mesh_files.push_back("data/obj/square.obj");
  mesh_files.push_back("data/obj/Crate0.obj");
  mesh_files.push_back("data/obj/BigH.obj");
  mesh_files.push_back("data/obj/Bar.obj");
  mesh_files.push_back("data/obj/Tetris.obj");
  mesh_files.push_back("data/obj/BigC.obj");
  mesh_files.push_back("data/obj/Knob.obj");
  mesh_files.push_back("data/obj/Bucket.obj");
  mesh_files.push_back("data/obj/Station1.obj");
  mesh_files.push_back("data/obj/Station2.obj");
  mesh_files.push_back("data/obj/Station3.obj");
  mesh_files.push_back("data/obj/Empire1.obj");
  mesh_files.push_back("data/obj/Empire2.obj");
  mesh_files.push_back("data/obj/Repair.obj");

  std::vector<const char*> texture_files;
  texture_files.push_back("data/obj/Station0.png");
  texture_files.push_back("data/img/space.png");
  texture_files.push_back("data/obj/Ship0.png");
  texture_files.push_back("data/img/square.png");
  texture_files.push_back("data/obj/Crate0.png");
  texture_files.push_back("data/obj/BigH.png");
  texture_files.push_back("data/obj/Bar.png");
  texture_files.push_back("data/obj/Tetris.png");
  texture_files.push_back("data/obj/BigC.png");
  texture_files.push_back("data/obj/Knob.png");
  texture_files.push_back("data/obj/Bucket.png");
  texture_files.push_back("data/obj/Station1.png");
  texture_files.push_back("data/obj/Station2.png");
  texture_files.push_back("data/obj/Station3.png");
  texture_files.push_back("data/obj/Empire1.png");
  texture_files.push_back("data/obj/Empire2.png");
  texture_files.push_back("data/obj/Repair.png");

  for ( int i = 0; i < mesh_files.size(); ++i )
  {
    Mesh* mesh = new Mesh();

    mesh->load_obj(mesh_files[i]);
    mesh->upload_obj();

    if (texture_files[i])
    {
      mesh->texture = new Texture();
      mesh->texture->loadPng(texture_files[i]);
    }

    m_meshes.push_back(mesh);
  }

}


Mesh* MeshManager::getMesh(int index)
{
  if (index >= m_meshes.size()) return m_meshes[0];

  return m_meshes[index];
}
