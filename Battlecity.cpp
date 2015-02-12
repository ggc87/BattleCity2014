#include "Battlecity.h"
#include "DotSceneLoader.h"
#include "Factory.h"
#include "Components.h"
#include "MovementManager.h"
#include "PlayerManager.h"
#include "MapManager.h"
#include "AIManager.h"
#include "CollisionManager.h"
#include "MessageManager.h"
#include "HeatingManager.h"
#include "HealthManager.h"
#include "ShootManager.h"
#include "ScoreManager.h"
#include "ParticleManager.h"
#include "SoundManager.h"

using namespace entityx;
Battlecity::Battlecity()
{
}


void Battlecity::init()
{
    quitGame = false;
    RenderManager::getPtr()->init("Battlecity");
    std::cout << "RENDER configured" << std::endl;
    SoundManager::getPtr()->init();
    SoundManager::getPtr()->loadFiles();
    std::cout << "Sound configured" << std::endl;
    InputManager::getPtr()->init();
    std::cout << "INPUT configured" << std::endl;
    ScreenManager::getPtr()->init(MenuScreen::getPtr());
    std::cout << "SCREEN configured" << std::endl;
    RenderManager::getPtr()->setFrameListener(this);
    std::cout << "FRAME LISTENER SETTED" << std::endl;
    PlayerManager::getPtr()->init();
    std::cout << "PLAYER configured" << std::endl;
    MapManager::getPtr()->init();
    std::cout << "MAP configured" << std::endl;
    MovementManager::getPtr()->init();
    std::cout << "MOVEMENT configured" << std::endl;
    AIManager::getPtr()->init();
    std::cout << "AI configured" << std::endl;
    HealthManager::getPtr()->init();
    std::cout << "HEALTH configured" << std::endl;
    ShootManager::getPtr()->init();
    std::cout << "SHOOT configured" << std::endl;
    ScoreManager::getPtr()->init();
    std::cout << "SCORE configured" << std::endl;
    ParticleManager::getPtr()->init();
    std::cout << "PARTICLE configured" << std::endl;
    MessageManager::getPtr()->subscribe<ShutDownEvent>(*this);
}


void Battlecity::start()
{
    RenderManager::getPtr()->getRoot()->startRendering();

}

void Battlecity::receive(const ShutDownEvent &event)
{
    quitGame = true;
}

bool Battlecity::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if(RenderManager::getPtr()->getRenderWindow()->isClosed())
        return false;

    InputManager::getPtr()->update();

    ScreenManager::getPtr()->update(evt.timeSinceLastFrame);

    AIManager::getPtr()->update(evt.timeSinceLastFrame);

    HeatingManager::getPtr()->update();


    MovementManager::getPtr()->update(evt.timeSinceLastFrame);

    ParticleManager::getPtr()->update(evt.timeSinceLastFrame);


    RenderManager::getPtr()->update(evt.timeSinceLastFrame);
    return !quitGame;
}
