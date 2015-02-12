#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H
#include <entityx.h>
#include "Singleton.h"
#include "Messages.h"
class PlayerManager : public Singleton<PlayerManager>
{
public:
    PlayerManager();
    void init();
    void setPlayerEntity(entityx::Entity player);
    entityx::Entity getPlayerEntity();
    void handleKeyPressed(const OIS::KeyEvent &event);
    void handleKeyReleased(const OIS::KeyEvent &event);
    void handleMouseMoved(const OIS::MouseEvent &event);
    void handleMousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID id);
    void handleMouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID id);
private:
    entityx::Entity player;
    void fire();
};

#endif // PLAYERMANAGER_H
