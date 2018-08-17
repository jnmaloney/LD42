#include "MenuLaunch.h"
#include "WindowManager.h"
#include "imgui.h"
#include <string>


MenuLaunch::MenuLaunch()
{

}


MenuLaunch::~MenuLaunch()
{


}


void MenuLaunch::gameOver()
{

}


void MenuLaunch::frame()
{
  WindowManager& window = *WindowManager::getInstance();
  ImVec2 pos(8.f, 8.f);
  ImVec2 pivot(0, 0);

  // 0.
  if (showMenu)
  {
    pos = ImVec2(0.5 * window.width - 90.f, 0.33 * window.height - 75.f);
    ImGui::SetNextWindowPos(pos, 0, pivot);
    ImGui::Begin("Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("");
    ImGui::Text("Restart?");
    ImGui::Text("");
    ImGui::Text("");

    m_restart = ImGui::Button("Yes");
    ImGui::SameLine(120.f);
    showMenu = !ImGui::Button("No");

    ImGui::End();

    return;
  }
  else if (!showIntro)
  {
    pos = ImVec2(window.width - 90.f, 8.f);
    ImGui::SetNextWindowPos(pos, 0, pivot);
    ImGui::Begin("Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    showMenu = ImGui::Button("Menu");

    ImGui::End();
  }

  // 1.
  pos = ImVec2(0.5f * window.width - 50.f, 8.f);
  if (!showIntro)
  {
    ImGui::SetNextWindowPos(pos, 0, pivot);
    ImGui::Begin("Clams", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("%i Clams", m_clams);

    ImGui::End();
  }
  else
  {
    pos = ImVec2(0.5f * window.width - 150.f, 8.f);
    ImGui::SetNextWindowPos(pos, 0, pivot);
    ImGui::Begin("Intro", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("Some people say space is humungously,");
    ImGui::Text("incredibly, mind-bogglingly big.");
    ImGui::Text("");
    ImGui::Text("Some people say space is ever");
    ImGui::Text("expanding and growing larger. ");
    ImGui::Text("");
    ImGui::Text("I say that there's never enough space,");
    ImGui::Text("especially when you're running a");
    ImGui::Text("warehouse at the end of the universe.");
    ImGui::Text("");
    ImGui::Text("Instrunctions: Buy, Sell and Survive.");
    ImGui::Text("Don't run out of space!");

    ImGui::End();
  }

  // 2.
  pos = ImVec2(window.width - 150.f, 64.f);
  ImGui::SetNextWindowPos(pos, 0, pivot);

  if (showIntro)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("Ready?");
    ImGui::Text("");
    ImGui::Text("");
    ImGui::Text("");

    if (ImGui::Button("Let's go!"))
    {
      m_launch = true;
      showIntro = false;
    }
    ImGui::End();
  }

  if (canLaunch)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("Ready for launch");
    ImGui::Text("");
    ImGui::Text("");
    ImGui::Text("");

    if (ImGui::Button("Launch now >"))
    {
      m_launch = true;
    }
    ImGui::End();

    pos = ImVec2(15.f, 14.f);
    ImGui::SetNextWindowPos(pos, 0, pivot);
    ImGui::Begin("Buying", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("Buying");

    if (mod == 0)
    {
      ImGui::Text("Box");
    }
    if (mod == 1)
    {
      ImGui::Text("Big H");
    }
    if (mod == 2)
    {
      ImGui::Text("Beam");
    }
    if (mod == 3)
    {
      ImGui::Text("Squggly");
    }
    if (mod == 4)
    {
      ImGui::Text("Big C");
    }
    if (mod == 5)
    {
      ImGui::Text("Space Fork");
    }
    if (mod == 6)
    {
      ImGui::Text("Bucket");
    }


    ImGui::End();
  }

  if (empireVisit1)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("\"Comrade. The");
    ImGui::Text("Empire has");
    ImGui::Text("requested that");
    ImGui::Text("these goods need");
    ImGui::Text("storage here.\"");


    ImGui::End();
  }

  if (empireVisit2)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("\"We are collecting");
    ImGui::Text("the Empire's");
    ImGui::Text("goods. Your service");
    ImGui::Text("is vital, Comrade.\"");

    if (ImGui::Button("Okay"))
    {
      m_launch = true;
    }
    ImGui::End();
  }

  if (asteroidTime)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("Asteroid incoming");
    ImGui::Text("");
    ImGui::Text("Prepare for impact");
    ImGui::Text("");

    if (ImGui::Button("Find Cover"))
    {
      m_launch = true;
    }
    ImGui::End();
  }

  m_repair = false;
  if (repairVisit)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (m_repairEmpty)
    {
      ImGui::Text("I'm the  mobile repair");
      ImGui::Text("ship. I'll come ");
      ImGui::Text("back when you need ");
      ImGui::Text("some repairs.");
      ImGui::Text("");

      if (ImGui::Button("See ya"))
      {
        m_launch = true;
      }
    }
    else
    {
      ImGui::Text("Looks like you need");
      ImGui::Text("some repairs.");
      ImGui::Text("");
      ImGui::Text("");

      if (ImGui::Button("Repair . . . 42 Clams"))
      {
        m_repair = true;
      }
      else ImGui::Text("");

      if (ImGui::Button("Take Off"))
      {
        m_launch = true;
      }
    }

    ImGui::End();
  }

  if (m_gameOver)
  {
    ImGui::Begin("Launch", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("You were destroyed...");
    ImGui::Text("");
    ImGui::Text("");
    ImGui::Text("");

    if (ImGui::Button("Restart"))
    {
      m_restart = true;
    }

    ImGui::End();
  }

  // 3. Mouse
  ImGuiIO& io = ImGui::GetIO();
  pos = ImVec2(io.MousePos.x - 25.f, io.MousePos.y - 78.f);
  if (holdingCost || holdingSell) pos = ImVec2(io.MousePos.x - 25.f, io.MousePos.y - 100.f);
  ImGui::SetNextWindowPos(pos, 0, pivot);

  if (holding > -1)
  {

    ImGui::Begin("Mouse", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    //std::string cost = std::to_string(holdingCost) + std::string(" Clams");

    if (holding == 0) ImGui::Text("Box");
    if (holding == 1) ImGui::Text("Big H");
    if (holding == 2) ImGui::Text("Beam");
    if (holding == 3) ImGui::Text("Squigly");
    if (holding == 4) ImGui::Text("Big C");
    if (holding == 5) ImGui::Text("Space Fork");
    if (holding == 6) ImGui::Text("Bucket");
    if (holdingCost) ImGui::Text("Buy: %i Clams", holdingCost);
    if (holdingSell) ImGui::Text("Sell: %i Clams", holdingSell);
    ImGui::End();
  }

  // 4. Trash
  m_trash = false;
  if (holding_canTrash)
  {
    pos = ImVec2(15.f, 94.f);
    ImGui::SetNextWindowPos(pos, 0, pivot);
    ImGui::Begin("Junk", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    m_trash = ImGui::Button("Space Junk\n\nDump Item");
    ImGui::End();
  }
}
