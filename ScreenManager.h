#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include <vector>
#include "Screen.h"
#include <Ogre.h>
#include "Messages.h"
#include "Singleton.h"

class ScreenManager : public entityx::Receiver<ScreenManager>, public Singleton<ScreenManager>, public Ogre::FrameListener
{
public:
    ScreenManager() {}
    ~ScreenManager();

    void init(Screen* initialScreen);


    void pushScreen(Screen* screen);
    void popScreen();
    void changeScreen(Screen *screen);
    void resetTo(Screen* screen);

    void update(double dt);

    bool frameRenderingQueued(const Ogre::FrameEvent &evt);

    entityx::ptr<entityx::EntityManager> getCurrentEntities();

    void receive(const KeyPressedEvent &event);
    void receive(const KeyReleasedEvent &event);
    void receive(const MouseMovedEvent &event);
    void receive(const MousePressedEvent &event);
    void receive(const MouseReleasedEvent &event);

private:
    std::vector<Screen*> mScreens;
};

#endif // SCREENMANAGER_H
