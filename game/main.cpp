#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
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
#include "imgui.h"


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
bool g_leftmousebutton = false;


//
// Drawing
//
void draw()
{
  // Touch?
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2((float)g_is->m_cursor_x, (float)g_is->m_cursor_y);
  io.MouseDown[0] = g_leftmousebutton;

    if (g_menuLaunch.showMenu ||
        g_menuLaunch.showIntro)
      g_rs->start( glm::vec3(0.f, 10.f, 15.f), glm::vec3(0.f, -3.f, 0.f) );
    else
      g_rs->start( glm::vec3(0.f, 3.5f, 5.9f), glm::vec3(0.f, -1.f, 0.f) );
    g_mapRenderer.tick();
    g_mapRenderer.m_renderQueue.draw(0, 0, g_rs);
    g_rs->end();

    int encounter = g_yourStation.m_dockedShip.encounter_type;
    g_menuLaunch.mod = g_yourStation.m_dockedShip.encounter_mod;

    g_menuLaunch.m_clams = g_yourStation.m_clams;
    g_menuLaunch.canLaunch = false;
    g_menuLaunch.empireVisit1 = false;
    g_menuLaunch.empireVisit2 = false;
    g_menuLaunch.asteroidTime = false;
    g_menuLaunch.repairVisit = false;
    if (encounter == 0)
    {
      g_menuLaunch.canLaunch = (g_yourStation.m_dockedShip.docked == 0);
    }
    if (encounter == 1)
    {
      g_menuLaunch.empireVisit1 = (g_yourStation.m_dockedShip.docked == 0);
      // Empty ship
      if (g_yourStation.m_dockedShip.m_objects.size() == 0 && !g_actionManager.m_holding)
      {
        g_menuLaunch.m_launch = true;
        g_menuLaunch.empireVisit1 = false;
      }
    }
    if (encounter == 2)
    {
      g_menuLaunch.asteroidTime = true;
    }
    if (encounter == 3)
    {
      g_menuLaunch.empireVisit2 = (g_yourStation.m_dockedShip.docked == 0);
    }
    if (encounter == 4)
    {
      g_menuLaunch.repairVisit = (g_yourStation.m_dockedShip.docked == 0);
      g_menuLaunch.m_repairEmpty = (g_actionManager.m_crashed1 == false && g_actionManager.m_crashed2 == false && g_actionManager.m_crashed3 == false);
    }

    if (g_actionManager.m_holding) g_menuLaunch.holding = g_actionManager.m_heldObject.object_type;
    else g_menuLaunch.holding = -1;

    // Set holding object cost
    if (g_yourStation.m_dockedShip.encounter_type == 0 &&
        g_actionManager.m_returnToShip &&
        g_actionManager.m_heldObject.object_type != g_yourStation.m_dockedShip.encounter_mod &&
        g_actionManager.cursor_place == 0)
    {
      g_menuLaunch.holdingCost = g_actionManager.m_heldObject.cost;
      g_menuLaunch.holdingSell = 0;
    }
    else if (g_yourStation.m_dockedShip.encounter_type == 0 &&
             g_actionManager.m_returnToStation  &&
             g_actionManager.m_heldObject.object_type == g_yourStation.m_dockedShip.encounter_mod &&
             g_actionManager.m_heldObject.colour < 25 &&
             g_actionManager.cursor_place == 1)
    {
      g_menuLaunch.holdingCost = 0;
      g_menuLaunch.holdingSell = g_actionManager.m_heldObject.sell_price;
    }
    else
    {
      g_menuLaunch.holdingCost = 0;
      g_menuLaunch.holdingSell = 0;
    }

    // Set trash open/close
    g_menuLaunch.holding_canTrash = (g_actionManager.m_holding && g_actionManager.m_returnToStation) ? 1 : 0;

    // Touch?
    io.MousePos = ImVec2((float)g_is->m_cursor_x, (float)g_is->m_cursor_y);
    io.MouseDown[0] = g_leftmousebutton;

    g_menuManager.predraw();
    io.MousePos = ImVec2((float)g_is->m_cursor_x, (float)g_is->m_cursor_y);
    io.MouseDown[0] = g_leftmousebutton;
    g_menuLaunch.frame();
    g_menuManager.postdraw(g_windowManager);

    if (g_menuLaunch.m_trash)
    {
      if (g_actionManager.m_returnToStation)
        g_actionManager.m_holding = false;
    }

    if (g_menuLaunch.m_repair)
    {
      if (g_yourStation.m_clams >= 42)
      {
        if (g_actionManager.m_crashed2) { g_actionManager.m_crashed2 = false; g_yourStation.m_clams -= 42; g_actionManager.doMask(); }
        else if (g_actionManager.m_crashed3) { g_actionManager.m_crashed3 = false; g_yourStation.m_clams -= 42; g_actionManager.doMask(); }
        else if (g_actionManager.m_crashed1) { g_actionManager.m_crashed1 = false; g_yourStation.m_clams -= 42; g_actionManager.doMask(); }
      }

      //if (g_actionManager.m_crashed1 == false && g_actionManager.m_crashed2 == false && g_actionManager.m_crashed3 == false) g_menuLaunch.m_launch = true;

    }

    g_menuLaunch.m_gameOver = g_actionManager.gameOver;
    if (g_menuLaunch.m_restart)
    {
      // g_actionManager.gameOver = false;
      // g_menuLaunch.m_launch = false;
      // g_menuLaunch.repairVisit = false;
      // g_menuLaunch.asteroidTime = false;
      // g_yourStation.m_clams = 42;
      // g_yourStation.m_empireCounter = 2;
      // g_yourStation.m_asteroidCounter = 1;
      // g_actionManager.m_crashed1 = false;
      // g_actionManager.m_crashed2 = false;
      // g_actionManager.m_crashed3 = false;
      // g_actionManager.m_objects.clear();
      // g_actionManager.m_holding = false;
      // memset(g_actionManager.m_stationArray, 0, 15*15*sizeof(int));
      // memset(g_actionManager.m_shipArray, 0, 15*15*sizeof(int));
      // g_actionManager.doMask();
      // g_menuLaunch.showIntro = true;

      // Restart !!!
      g_menuLaunch.m_restart = false;
      g_menuLaunch.showMenu = false;
      g_menuLaunch.showIntro = true;
      g_menuLaunch.m_launch = false;

      g_actionManager.reset();
      g_yourStation.reset();
    }

    if (g_menuLaunch.m_launch)
    {
      g_menuLaunch.m_launch = false;
      g_actionManager.m_holding = false;

      // Last encounter
      if (encounter == 2)
        g_actionManager.asteroid_strike();
      if (encounter == 3)
        g_actionManager.empire_collect();

      // Next encounter
      g_yourStation.launch();
      encounter = g_yourStation.m_dockedShip.encounter_type;

      if (encounter == 0)
        g_actionManager.loadShip();
      if (encounter == 1)
        g_actionManager.loadShipEmpire();
      if (encounter == 2)
        g_actionManager.asteroid();
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
  //std::cout << "Button " << button << ",  action " << action << ",  mods " << mods << std::endl;

  g_is->mouse_button_callback(button, action, mods);

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    g_actionManager.action();
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    // Drop on touch release
    if (g_actionManager.m_holding)
      g_actionManager.action();
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
  {
    g_actionManager.cancel();
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


void touch_callback_glfw(GLFWwindow* window, int touch, int action, double x, double y)
{

}


EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  // printf("%i, numTouches: %d %s%s%s%s\n",
  //   (eventType), e->numTouches,
  //   e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "");
  // for(int i = 0; i < e->numTouches; ++i)
  // {
  //   const EmscriptenTouchPoint *t = &e->touches[i];
  //   printf("  %ld: screen: (%ld,%ld), client: (%ld,%ld), page: (%ld,%ld), isChanged: %d, onTarget: %d, canvas: (%ld, %ld)\n",
  //     t->identifier, t->screenX, t->screenY, t->clientX, t->clientY, t->pageX, t->pageY, t->isChanged, t->onTarget, t->canvasX, t->canvasY);
  // }
  // if (e->numTouches == 0)
  // {
  //   //g_is->mouse_button_callback(0, 1, 0);
  //   mouse_button_callback(g_windowManager.g_window, 0, 1, 0);
  // }
  // if (e->numTouches == 2)
  // {
  //   //g_is->mouse_button_callback(0, 0, 0);
  //   mouse_button_callback(g_windowManager.g_window, 0, 0, 0);
  // }
  return 0;
}


EM_BOOL touch_cancel_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  return 0;
}


EM_BOOL touch_move_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  for(int i = 0; i < e->numTouches; ++i)
  {
    const EmscriptenTouchPoint *t = &e->touches[i];
    double xpos = t->canvasX;
    double ypos = t->canvasY;

    // "Mouse Move" event
    g_is->cursor_pos_callback(xpos, ypos);
    g_rs->setCursor(xpos, ypos);
    //glfwSetCursorPos(g_windowManager.g_window, xpos, ypos);
    //break;
  }
  return 0;
}


EM_BOOL touch_end_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  if (e->numTouches <= 1)
  {
    mouse_button_callback(g_windowManager.g_window, GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0);
    g_leftmousebutton = false;
  }
  //std::cout << "touch end  " << e->numTouches << std::endl;
  return 0;
}


EM_BOOL touch_start_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  g_leftmousebutton = true;

  for(int i = 0; i < e->numTouches; ++i)
  {
    const EmscriptenTouchPoint *t = &e->touches[i];
    double xpos = t->canvasX;
    double ypos = t->canvasY;

    // "Mouse Move" event
    g_is->cursor_pos_callback(xpos, ypos);
    g_rs->setCursor(xpos, ypos);
    //glfwSetCursorPos(g_windowManager.g_window, xpos, ypos);
    break;
  }

  if (e->numTouches == 1)
  {
    mouse_button_callback(g_windowManager.g_window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
  }
  else
  {
    // Rotate
    if (g_actionManager.m_holding)
    {
      g_actionManager.m_heldObject.rotation = (g_actionManager.m_heldObject.rotation + 1) % 4;
    }
  }

  return 0;
}


void loop()
{
  input();
  draw();
  g_audio.tick();
}


int init()
{
  // std::cout << "init 1" << std::endl;

  g_windowManager.width = 80;
  g_windowManager.height = 60;
  g_windowManager.init("LD42");
  g_menuManager.init(g_windowManager);

  // std::cout << "init 2" << std::endl;

  g_rs = new RenderSystem();
  g_rs->m_cameraX = 128.0;
  g_rs->m_cameraY = 128.0;
  // std::cout << "init 2b" << std::endl;
  g_rs->init(g_windowManager);

  // std::cout << "init 3" << std::endl;

  g_is = new InputSystem(g_windowManager);

  g_meshManager.load();
  g_mapRenderer.m_rs = g_rs;
  g_mapRenderer.m_am = &g_actionManager;
  g_mapRenderer.m_station = &g_yourStation;

  // std::cout << "init 4" << std::endl;

  g_actionManager.m_rs = g_rs;
  g_actionManager.m_station = &g_yourStation;

  // std::cout << "init 5" << std::endl;

  g_audio.init();
  g_audio.save = false;
  g_audio.load_ogg("data/audio/LD42.ogg");
  g_audio.play(0, true);

  // std::cout << "init 6" << std::endl;

  // Window size callback
  glfwSetWindowSizeCallback(g_windowManager.g_window, window_size_callback);
  glfwSetFramebufferSizeCallback(g_windowManager.g_window, window_size_callback);

  // Cursor callbacks
  // glfwSetCursorPosCallback(g_windowManager.g_window, cursor_pos_callback);
  // glfwSetMouseButtonCallback(g_windowManager.g_window, mouse_button_callback);
  // glfwSetScrollCallback(g_windowManager.g_window, scroll_callback);

  // Key callbacks
  //glfwSetKeyCallback(g_windowManager.g_window, key_callback);
  //glfwSetCharCallback(g_windowManager.g_window, ImGui_ImplGlfw_CharCallback);

  // TODO Touch callbacks
  EMSCRIPTEN_RESULT ret;
  //glfwSetTouchCallback(g_windowManager.g_window, touch_callback);
  ret = emscripten_set_touchstart_callback(0, 0, 1, touch_start_callback);
  //TEST_RESULT(emscripten_set_touchstart_callback);
  ret = emscripten_set_touchend_callback(0, 0, 1, touch_end_callback);
  //TEST_RESULT(emscripten_set_touchend_callback);
  ret = emscripten_set_touchmove_callback(0, 0, 1, touch_move_callback);
  //TEST_RESULT(emscripten_set_touchmove_callback);
  ret = emscripten_set_touchcancel_callback(0, 0, 1, touch_cancel_callback);
  //TEST_RESULT(emscripten_set_touchcancel_callback);

  //glfwSetTouchCallback

  return 0;
}


void quit()
{
  glfwTerminate();
}


extern "C" int main(int argc, char** argv)
{
  std::cout << "Initialising" << std::endl;

  if (init() != 0) return 1;

  std::cout << "Starting Main Loop" << std::endl;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}
