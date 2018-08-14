#pragma once




class MenuLaunch
{
public:
  MenuLaunch();
  ~MenuLaunch();
  void frame();

  void gameOver();

  bool m_launch = false;
  bool showIntro = true;
  bool canLaunch = false;
  bool empireVisit1 = false;
  bool empireVisit2 = false;
  bool asteroidTime = false;
  bool repairVisit = false;

  int m_gameOver = false;
  bool m_restart = false;

  int m_clams = 0;
  int mod = 0;

  int holding = -1;
  int holdingCost;
  int holdingSell;

  bool m_trash = false;
  bool m_repair = false;

};
