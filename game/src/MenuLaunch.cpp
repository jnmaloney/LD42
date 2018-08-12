#include "MenuLaunch.h"
#include "WindowManager.h"
#include "imgui.h"


MenuLaunch::MenuLaunch()
{

}


MenuLaunch::~MenuLaunch()
{


}



void MenuLaunch::frame()
{
  WindowManager& window = *WindowManager::getInstance();

  m_launch = false;

  ImVec2 pos(window.width - 150.f, 64.f);
  ImVec2 pivot(0, 0);
  ImGui::SetNextWindowPos(pos, 0, pivot);
  ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::Text("Launching in");
  ImGui::Text("");
  ImGui::Text("...");
  ImGui::Text("");

  if (canLaunch)
  {
    if (ImGui::Button("Launch now >"))
    {
      m_launch = true;
    }
  }
  ImGui::End();
}
