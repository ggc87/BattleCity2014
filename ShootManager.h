#ifndef SHOOTMANAGER_H
#define SHOOTMANAGER_H

#include <Singleton.h>
#include "Messages.h"
#include <entityx.h>
//#include<Ogre.h>

class ShootManager : public Singleton<ShootManager>, public entityx::Receiver<ShootManager>
{
public:
    ShootManager();
    ~ShootManager();
    void init();
    void receive(const ShootEvent &event);

protected:
    Ogre::RaySceneQuery *mRaySceneQuery;
};

#endif // SHOOTMANAGER_H
