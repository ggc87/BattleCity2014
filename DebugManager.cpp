#include "DebugManager.h"
#include "RenderManager.h"
DebugManager::DebugManager(){}



void DebugManager::init(entityx::ptr<entityx::EventManager> events)
{
    events->subscribe<KeyPressedEvent>(*this);
    logManager = Ogre::LogManager::getSingletonPtr();
    logManager->createLog("Battlecity.log",false,false,false);
}

void DebugManager::receive(const KeyPressedEvent &event)
{
    std::cout << "Premuto " << event.event.key << std::endl;
}

void DebugManager::receive(const MoveEvent &event)
{
    std::cout << event.x << "\t" << event.y << "\t" << event.z << std::endl;
}
