#ifndef DEBUGMANAGER_H
#define DEBUGMANAGER_H
#include <OgreLogManager.h>
#include "Messages.h"
#include <entityx.h>
#include <Singleton.h>

class DebugManager : public Singleton<DebugManager>, public entityx::Receiver<DebugManager>
{
public:
    DebugManager();
    void init(entityx::ptr<entityx::EventManager> events);
    void receive(const KeyPressedEvent &event);
    void receive(const KeyReleasedEvent &event);
    void receive(const MouseMovedEvent &event);
    void receive(const MoveEvent &event);
protected:
    Ogre::LogManager* logManager;

};

#endif // DEBUGMANAGER_H
