#ifndef GAME_H
#define GAME_H
#include <entityx.h>
#include "RenderManager.h"
#include "Messages.h"
#include "InputManager.h"

class Game : public Ogre::FrameListener, public entityx::Receiver<Game> {
public:

    virtual void init() = 0;

    virtual void start() = 0;

    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) = 0;

    virtual void receive(const ShutDownEvent &event) = 0;

};

#endif // GAME_H
