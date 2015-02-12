#ifndef GAMETEST_H
#define GAMETEST_H
#include "Game.h"

class Battlecity : public Game
{
public:
    Battlecity();
    void init();
    void start();
    bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    void receive(const ShutDownEvent &event);

private:
    bool quitGame;
};

#endif // GAMETEST_H
