#include "ScreenManager.h"
#include "RenderManager.h"
#include "MessageManager.h"
#include <entityx.h>


ScreenManager::~ScreenManager()
{

    while (!mScreens.empty()){
        mScreens.back()->leave();
        mScreens.pop_back();
    }
}

void ScreenManager::init(Screen *initialScreen)
{
    RenderManager::getPtr()->setFrameListener(this);
    MessageManager::getPtr()->subscribe<KeyPressedEvent>(*this);
    MessageManager::getPtr()->subscribe<KeyReleasedEvent>(*this);
    MessageManager::getPtr()->subscribe<MouseMovedEvent>(*this);
    MessageManager::getPtr()->subscribe<MousePressedEvent>(*this);
    MessageManager::getPtr()->subscribe<MouseReleasedEvent>(*this);
    Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
    pushScreen(initialScreen);
}

void ScreenManager::pushScreen(Screen *screen)
{
    if(!mScreens.empty())
        mScreens.back()->pause();

    mScreens.push_back(screen);
    screen->enter();
}

void ScreenManager::popScreen()
{
    if(!mScreens.empty()){
        mScreens.back()->leave();
        mScreens.pop_back();
    }

    if(!mScreens.empty())
        mScreens.back()->resume();
}

void ScreenManager::changeScreen(Screen *screen)
{
    popScreen();
    pushScreen(screen);
}

bool ScreenManager::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if(!mScreens.empty()){
        OgreBites::SdkTrayManager *tray = mScreens.back()->getTrayMgr();
        tray->frameRenderingQueued(evt);
        mScreens.back()->update(evt.timeSinceLastFrame);
    }
    return true;
}

void ScreenManager::resetTo(Screen *screen)
{
    while (!mScreens.empty()){
        mScreens.back()->leave();
        mScreens.pop_back();
    }
    pushScreen(screen);
}

entityx::ptr<entityx::EntityManager> ScreenManager::getCurrentEntities()
{
    if(!mScreens.empty()){
        Screen* curr = mScreens.back();
        return curr->getEntities();
    }

}

void ScreenManager::update(double dt)
{
    if(!mScreens.empty()){
        mScreens.back()->update(dt);
    }
}

void ScreenManager::receive(const KeyPressedEvent &event)
{
    if(event.event.key == OIS::KC_SYSRQ)
        RenderManager::getPtr()->getRenderWindow()->writeContentsToTimestampedFile("Battlecity2014_",".png");

    if(!mScreens.empty()){
        mScreens.back()->keyPressed(event.event);
    }

}

void ScreenManager::receive(const KeyReleasedEvent &event)
{
    if(!mScreens.empty()){
        Screen* curr = mScreens.back();
        curr->keyReleased(event.event);
    }

}

void ScreenManager::receive(const MouseMovedEvent &event)
{
    if(!mScreens.empty()){
       Screen* curr = mScreens.back();
       curr->mouseMoved(event.event);
    }
}

void ScreenManager::receive(const MousePressedEvent &event)
{
    if(!mScreens.empty()){
        Screen* curr = mScreens.back();
        curr->mousePressed(event.event, event.buttonID);
    }
}

void ScreenManager::receive(const MouseReleasedEvent &event)
{
    if(!mScreens.empty()){
        mScreens.back()->mouseReleased(event.event, event.buttonID);
    }
}
