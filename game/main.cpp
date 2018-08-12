#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#endif
#include "graphics.h"


#include "InputSystem.h"
#include "RenderSystem.h"
#include "WindowManager.h"
#include "MenuManager.h"
#include "InputSystem.h"
#include "MeshManager.h"
#include "MenuLaunch.h"
#include "SpaceMapRenderer.h"
#include "ActionManager.h"
#include "YourStation.h"
#include "AudioManager.h"


RenderSystem* g_rs = 0;
InputSystem* g_is = 0;
WindowManager g_windowManager;
MenuManager g_menuManager;
MenuLaunch g_menuLaunch;
SpaceMapRenderer g_mapRenderer;
RenderQueue g_actionRenderQueue;
MeshManager g_meshManager;
ActionManager g_actionManager;
YourStation g_yourStation;
AudioManager g_audio;


//
// Drawing
//
void draw()
{
    g_rs->start();
    g_mapRenderer.tick();
    g_mapRenderer.m_renderQueue.draw(0, 0, g_rs);
    g_rs->end();

    g_menuLaunch.canLaunch = (g_yourStation.m_dockedShip.docked == 0);

    g_menuManager.predraw();
    g_menuLaunch.frame();
    g_menuManager.postdraw(g_windowManager);

    if (g_menuLaunch.m_launch)
    {
      g_yourStation.launch();
      g_actionManager.loadShip();
    }
}


//
// Input
//
void input()
{
  static int timer = 0;
  ++timer;
  if (timer < 60) return;
  timer = 0;
}


void window_size_callback(GLFWwindow* window, int width, int height)
{
  g_windowManager.width = width;
  g_windowManager.height = height;
}


bool m_mouseDrag = false;
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
  //if (ImGui::GetIO().WantCaptureMouse) return;

  g_is->cursor_pos_callback(xpos, ypos);
  g_rs->setCursor(xpos, ypos);

  //g_actionManager.cursorPos((int)(0.5f + g_rs->m_cursorX), (int)(0.5f + g_rs->m_cursorY));
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  //if (ImGui::GetIO().WantCaptureMouse) return;

  g_is->mouse_button_callback(button, action, mods);

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    g_actionManager.action();
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
  }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  // if (yoffset > 0.0) g_rs->zoom(-1);
  // if (yoffset < 0.0) g_rs->zoom( 1);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if (key == GLFW_KEY_E && action == GLFW_PRESS)
    //     activate_airship();

    if (action == GLFW_PRESS)
    {
      if (g_actionManager.m_holding)
      {
        g_actionManager.m_heldObject.rotation = (g_actionManager.m_heldObject.rotation + 1) % 4;
      }
    }
}


void loop()
{
  input();
  draw();
  g_audio.tick();
}


int init()
{
  g_windowManager.width = 800;
  g_windowManager.height = 600;
  g_windowManager.init("LD42");
  g_menuManager.init(g_windowManager);

  g_rs = new RenderSystem();
  g_rs->m_cameraX = 128.0;
  g_rs->m_cameraY = 128.0;
  g_rs->init(g_windowManager);

  g_is = new InputSystem(g_windowManager);

  g_meshManager.load();
  g_mapRenderer.m_rs = g_rs;
  g_mapRenderer.m_am = &g_actionManager;
  g_mapRenderer.m_station = &g_yourStation;

  g_actionManager.m_rs = g_rs;
  g_actionManager.m_station = &g_yourStation;

  g_audio.init();
  g_audio.save = false;
  g_audio.load_ogg("data/audio/LD42.ogg");
  g_audio.play(0, true);

  // Window size callback
  glfwSetWindowSizeCallback(g_windowManager.g_window, window_size_callback);
  glfwSetFramebufferSizeCallback(g_windowManager.g_window, window_size_callback);

  // Cursor callbacks
  glfwSetCursorPosCallback(g_windowManager.g_window, cursor_pos_callback);
  glfwSetMouseButtonCallback(g_windowManager.g_window, mouse_button_callback);
  glfwSetScrollCallback(g_windowManager.g_window, scroll_callback);

  // Key callbacks
  glfwSetKeyCallback(g_windowManager.g_window, key_callback);
  //glfwSetCharCallback(g_windowManager.g_window, ImGui_ImplGlfw_CharCallback);

  // TODO Touch callbacks

  return 0;
}


void quit()
{
  glfwTerminate();
}


extern "C" int main(int argc, char** argv)
{
  if (init() != 0) return 1;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}
